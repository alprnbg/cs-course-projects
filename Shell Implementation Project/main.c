#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>


#define MAX_INP_LEN 500		// maximum number of characters for a command
#define MAX_STACK_LEN 15	// length of the history stack

// for the ease of pipe use
#define PIPE_READ 0			
#define PIPE_WRITE 1		


// declares the circular stack type for the footprint command
struct circular_stack {
    int head_index;
    char stack_arr[MAX_STACK_LEN][MAX_INP_LEN];
};


// pushes cmd_str to history_stack
void push_stack(struct circular_stack *history_stack, char cmd_str[]) {

	// copies the cmd_str to appropriate position of history stack.
    strcpy((*history_stack).stack_arr[(*history_stack).head_index], cmd_str);			 
    
    // increments the head_index of the history stack by applying mode to stay in limits.
    (*history_stack).head_index = ((*history_stack).head_index + 1) % MAX_STACK_LEN;	
}


// prints out the lastly pushed 15 elements of the history stack.
void print_stack(struct circular_stack *history_stack) {

    int cur_cmd_index = (*history_stack).head_index;	// index of first element to be printed
    unsigned int cmd_history_index = 1;					// order of the command (this will be printed, too)

    // iterates over the history_stack and prints out commands with the proper order. 
    for(unsigned int i = 1; i <= MAX_STACK_LEN; i++){

    	// we need to come back to the beginning of the stack, because it is circular.
        cur_cmd_index %= MAX_STACK_LEN;
        
        // prints the command if it is not an empty string (number of commands might be less than 15)
        if(strcmp((*history_stack).stack_arr[cur_cmd_index], "") != 0) {
            printf("%d %s\n", cmd_history_index, (*history_stack).stack_arr[cur_cmd_index]);
            cmd_history_index++;
        }
        cur_cmd_index++;
    }
}


// splits the input command according to the some rules.
void split_cmd(const char cmd[], char last_cmd_splitted[][MAX_INP_LEN]){

	// quote_start used to check whether read char is inside quotes or not,
	// if it is inside quotes, space will not split anything. 
	// This is for especially "grep" command.
    int quote_start = 0;

    int space_start = (cmd[0] == ' ');	// checks if first char is space or not
    char c;								// read char
    
    // some integers to track arguments
    int cmd_i = 0;
    int arg_i = 0;
    int arg_num = 0;

    while(1){
        
        c = cmd[cmd_i];	// read the input

        // space will be used as delimiter if read char is not inside in quotes.
        if (c == ' ' && !quote_start) {
            if (space_start) {
                cmd_i++;
                continue;
            } else {
                space_start = 1;    // if more than one space come sequentially, they will be ignored.
                last_cmd_splitted[arg_num][arg_i] = '\0';
                arg_num++;
                arg_i = 0;
            }
        }

        // breaking condition for while loop, end of the command.
        else if (c == '\0') {
            last_cmd_splitted[arg_num][arg_i] = '\0';
            last_cmd_splitted[arg_num + 1][0] = '\0';
            return;
        }

        // in order to detect quotes for "grep" command.
        else if (c == '\"') {
            space_start = 0;
            quote_start = !quote_start;
        }

        // special space character, for files with space in its name
        else if (c == '\\' && cmd[cmd_i+1] == ' ') {
            last_cmd_splitted[arg_num][arg_i] = ' ';
            arg_i++;
            cmd_i++;
        }

        // ignore new line char.
        else if(c == '\n') {;}
        
        // store the read char in appropriate position in last_cmd_splitted.
        else {
            space_start = 0;
            last_cmd_splitted[arg_num][arg_i] = c;
            arg_i++;
        }
        cmd_i++;
    }
}


// This function runs the necessary commands via forking and exec functions.
int run_command(const char* cmd, const char* arg1, const char* arg2) {

    pid_t pid;
    pid_t pid2;
    int fd_pipe[2];

    pipe(fd_pipe); // we will need pipe to transfer output of listdir to grep
    pid = fork(); // creates a child process

    if (pid < 0) // if an error occurred, returns -1
    {
        fprintf(stderr, "Error occurred during forking!");
        return 1;
    }

    else if (pid == 0) // execution of the child process
    {
    	// one of the exec function is called
    	// if something goes wrong error msg is printed and code 1 is returned.

        if (strcmp(cmd,"listdir")==0){
            execlp("/bin/ls","ls",NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "ls");
            return 1;
        }

        if (strcmp(cmd,"listdir -a")==0){
            execlp("/bin/ls","ls","-a",NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "ls");
            return 1;
        }

        if (strcmp(cmd,"currentpath")==0){
            execlp("pwd","pwd",NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "pwd");
            return 1;
        }

        if (strcmp(cmd,"printfile")==0){
            execlp("/bin/cat","cat",arg1,NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "cat");
            return 1;
        }

        if (strcmp(cmd,"printfile >")==0) {
            int fd = open(arg2, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);	// open the file indicated by arg2
            dup2(fd, STDOUT_FILENO);   // stdout -> file
            close(fd);	// closing is necessary
            execlp("/bin/cat", "cat", arg1, NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "cat");
            return 1;
        }

        if (strcmp(cmd,"listdir |")==0) {
            dup2(fd_pipe[PIPE_WRITE], STDOUT_FILENO);	// stdout -> pipe
            close(fd_pipe[PIPE_WRITE]);	// closing is necessary
            close(fd_pipe[PIPE_READ]);	// closing is necessary
            execlp("/bin/ls","ls",NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "ls");
            return 1;
        }

        if (strcmp(cmd,"listdir -a |")==0) {
            dup2(fd_pipe[PIPE_WRITE], STDOUT_FILENO);	// stdout -> pipe
            close(fd_pipe[PIPE_WRITE]);	// closing is necessary
            close(fd_pipe[PIPE_READ]);	// closing is necessary
            execlp("/bin/ls","ls","-a",NULL);
            fprintf(stderr, "Failed to execute '%s'\n", "ls");
            return 1;
        }
    }

    else // execution of the parent process
    {
    	// special case for listdir | grep, because we run 2 processes.
        if (strcmp(cmd,"listdir |")==0 || strcmp(cmd,"listdir -a |")==0) { 

            pid2 = fork();	// creates another child to run grep

            if (pid2 == 0){
            	// child that will run grep
                dup2(fd_pipe[PIPE_READ], STDIN_FILENO); // pipe -> stdin
                close(fd_pipe[PIPE_READ]);	// closing is necessary
                close(fd_pipe[PIPE_WRITE]);	// closing is necessary
                execlp("/bin/grep", "grep", arg1, NULL);
                fprintf(stderr, "Failed to execute '%s'\n", "grep");
                return 1;

            } else {
                close(fd_pipe[PIPE_READ]);
                close(fd_pipe[PIPE_WRITE]);
                wait(NULL);
                wait(NULL);
            }
        }
        else {
            close(fd_pipe[PIPE_WRITE]);
            close(fd_pipe[PIPE_READ]);
            wait(NULL);
        }
    }
    return 0;
}



int main() {

    char last_cmd[MAX_INP_LEN];							// to store last read command from input
    char last_cmd_splitted[100][MAX_INP_LEN] = {0};		// array for splitted commands
    struct circular_stack history_stack = {0, {0}};		// declares the history stack
    char username[100];									// username variable
    getlogin_r(username, 100);							// gets username
    time_t t = time(NULL);								// login time variable
    struct tm tm = *localtime(&t);						// gets login time

    // Welcome message
    printf("Welcome %s (%d-%d-%d %d:%d:%d)\n", username, tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%s >>> ", username);

    // shell will work until user enters the exit command.
    while(1) {

        memset(last_cmd_splitted,0, sizeof(last_cmd_splitted)); // clears the array containing the recently read input.
        fgets(last_cmd, MAX_INP_LEN, stdin);	// reads the input.
        split_cmd(last_cmd, last_cmd_splitted);	// splits the input.

 		// If the first word is listdir
        if (strcmp(last_cmd_splitted[0], "listdir") == 0) {

        	// Checks for the pipe
            if (strcmp(last_cmd_splitted[1], "|") != 0 && strcmp(last_cmd_splitted[2], "|") != 0) {
                
                // checks the arguments of listdir
                if (strcmp(last_cmd_splitted[1], "\0") == 0) {
                	// listdir
                    push_stack(&history_stack, last_cmd); // update the history stack.
                    run_command("listdir", "", "");	// start the run_command procedure
                    printf("%s >>> ", username);
                    continue;
                }
                else if (strcmp(last_cmd_splitted[1], "-a") == 0 && strcmp(last_cmd_splitted[2], "\0") == 0) {
                    // listdir -a
                    push_stack(&history_stack, last_cmd); // update the history stack.
                    run_command("listdir -a", "", "");	// start the run_command procedure
                    printf("%s >>> ", username);
                    continue;
                }
            }

            // If pipe and grep is in the input
            else if (strcmp(last_cmd_splitted[2], "grep") == 0 && strcmp(last_cmd_splitted[4], "\0") == 0){
                // listdir | grep
                push_stack(&history_stack, last_cmd); // update the history stack.
                run_command("listdir |", last_cmd_splitted[3], "");	// start the run_command procedure
                printf("%s >>> ", username);
                continue;
            }
            else if (strcmp(last_cmd_splitted[3], "grep") == 0 && strcmp(last_cmd_splitted[5], "\0") == 0){
                // listdir -a | grep
                push_stack(&history_stack, last_cmd); // update the history stack.
                run_command("listdir -a |", last_cmd_splitted[4], "");	// start the run_command procedure
                printf("%s >>> ", username);
                continue;
            }
        }

        // If the command is footprint
        else if (strcmp(last_cmd_splitted[0], "footprint") == 0 && strcmp(last_cmd_splitted[1], "\0") == 0) {
            push_stack(&history_stack, last_cmd); // update the history stack.
            print_stack(&history_stack); // prints the last 15 commands.
            printf("%s >>> ", username);
            continue;
        }

        // If the command is currentpath
        else if (strcmp(last_cmd_splitted[0], "currentpath") == 0 && strcmp(last_cmd_splitted[1], "\0") == 0) {
            push_stack(&history_stack, last_cmd); // update the history stack.
            run_command("currentpath", "", "");	// start the run_command procedure
            printf("%s >>> ", username);
            continue;
        }

        // If the command is printfile
        else if (strcmp(last_cmd_splitted[0], "printfile") == 0) {

        	// checks for redirect
            if (strcmp(last_cmd_splitted[2],">")==0 && strcmp(last_cmd_splitted[4], "\0")==0){
                // printfile >
                push_stack(&history_stack, last_cmd); // update the history stack.
                run_command("printfile >", last_cmd_splitted[1], last_cmd_splitted[3]);	// start the run_command procedure
                printf("%s >>> ", username);
                continue;

            } else if (strcmp(last_cmd_splitted[2],"\0")==0) {
                // printfile
                push_stack(&history_stack, last_cmd); // update the history stack.
                run_command("printfile", last_cmd_splitted[1], "");	// start the run_command procedure
                printf("%s >>> ", username);
                continue;
            }
        }

        // If users enter new line, nothing is done, just prints new line like other shells.
        else if (strcmp(last_cmd, "\n") == 0) {
            printf("%s >>> ", username);
            continue;
        }

        // Exit procedure
        else if (strcmp(last_cmd_splitted[0], "exit") == 0) {
            printf("Shell session terminated!\n");
            return 0;
        }

        // If command could not recognized, none of continues will be run and code will reach here.
        printf("Command not recognized\n");
        printf("%s >>> ", username);
    }
}