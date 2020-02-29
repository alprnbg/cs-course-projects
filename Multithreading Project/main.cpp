/* 
 * Alperen Bag
 * 2018400279
 * Compilation and Running Status: Yes
 * CMPE 322 Project 3
 */


#include <iostream>
#include <pthread.h>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>

#define MAX_CUSTOMERS 300

using namespace std;


/* ATM and Customer threads executes the following functions. */
void *atm_serve_func(void *ptr);
void *customer_func(void *ptr);

/* This function reads the input file and parses the information. It is executed by the main thread. */
void parse_input(ifstream& infile, pair<int,int> customer_info[], pair<string,int> bill_info[], int& TOTAL_CUSTOMERS);


/* Global variables for communication between threads. */
int atm_busy[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};  // -1 -> not busy, i (not -1) -> customer i using the ATM
int atm_ids[10] = {0,1,2,3,4,5,6,7,8,9};  // ATM IDs which are used by ATM threads to recognize itself
int customer_thread_ids[MAX_CUSTOMERS];  // Customer IDs which are used by customer threads to recognize itself
int customer_count = 0;  // # of customers that have been served until now

bool ALL_CUSTOMERS_SERVED = false;  // This is used as a signal for ATM threads which terminates when this variable 
                                    // becomes true

pair<int,int> customer_info[MAX_CUSTOMERS];  // Each pair.first is the ATM ID used by the customer, pair.second is the 
                                             // sleep time

pair<string,int> bill_info[MAX_CUSTOMERS];  // Each pair.first is the bill type, pair.second is the bill amount 
                                            // (index of the bill indicates customer ID)

int balances[5] = {0};  // Each element stores the sum of paid bills for each bill type
int TOTAL_CUSTOMERS;  // # of customers (will be read from the input file)

ifstream infile;
ofstream outfile;

/* Mutex variables */
pthread_mutex_t atm_mutexes[10];  // Each ATM can be used by only one customer

pthread_mutex_t transaction_mutexes[5];  // Each bill type has its own balance and only one ATM can change the balance 
                                         // of a bill type. For example, two ATMs can change electricity and water  
                                         // balances simultaneously, however they both cannot change water balance at 
                                         // the same time.  

pthread_mutex_t customer_counter_mutex;  // Counter indicating the total # of customers that have been served should be 
                                         // changed by only one customer thread at the same time.

pthread_mutex_t log_mutex;  // Log file can be edited only one ATM thread.


int main(int argc, char* argv[]){

    int t_ret;  // Return code from thread creation

    /* File handling */
    string infile_name, outfile_name;
    infile_name = argv[1];
    size_t pos = infile_name.find(".txt");
    outfile_name = infile_name.substr (0,pos) + "_log.txt";
    infile.open(infile_name);
    outfile.open(outfile_name);

    parse_input(infile, customer_info, bill_info, TOTAL_CUSTOMERS);

    /* Mutex initialization */
    for (int i = 0; i < 10; i++)
        pthread_mutex_init(&atm_mutexes[i],0);

    for (int i = 0; i < 5; i++)
        pthread_mutex_init(&transaction_mutexes[i],0);
    
    pthread_mutex_init(&customer_counter_mutex,0);
    pthread_mutex_init(&log_mutex,0);

    /* Containers for threads */
    vector<pthread_t> atm_threads;
    vector<pthread_t> client_threads;

    /* Creation of ATM threads */
    for(int i = 0; i < 10; i++){

        pthread_t thread_i;

        // Each thread can find its ID from atm_ids array.
        t_ret = pthread_create(&thread_i, NULL, atm_serve_func, &atm_ids[i]);

        if(t_ret){
            fprintf(stderr,"Error - pthread_create(thread_%d) return code: %d\n", i, t_ret);
            pthread_exit(NULL);
        }
        atm_threads.push_back(thread_i);
    }

    /* Creation of Customer threads */
    for(int i = 0; i < TOTAL_CUSTOMERS; i++){

        pthread_t thread_i;
        customer_thread_ids[i] = i;

        // Each thread can find its ID from customer_thread_ids array.
        t_ret = pthread_create(&thread_i, NULL, customer_func, &customer_thread_ids[i]);

        if(t_ret){
            fprintf(stderr,"Error - pthread_create(thread_%d) return code: %d\n", i, t_ret);
            pthread_exit(NULL);
        }
        client_threads.push_back(thread_i);
    }

    /* Waiting for all threads to finish */
    for(auto thread_i = atm_threads.begin(); thread_i != atm_threads.end(); thread_i++){
        pthread_join(*thread_i, NULL);
    }

    for(auto thread_i = client_threads.begin(); thread_i != client_threads.end(); thread_i++){
        pthread_join(*thread_i, NULL);
    }

    /* Deconstruction of mutexes */
    for (int i = 0; i < 10; i++)
        pthread_mutex_destroy(&atm_mutexes[i]);

    for (int i = 0; i < 5; i++)
        pthread_mutex_destroy(&transaction_mutexes[i]);

    pthread_mutex_destroy(&customer_counter_mutex);
    pthread_mutex_destroy(&log_mutex);

    /* Summary of transactions */
    outfile<<"All payments are completed."<<'\n';
    outfile<<"CableTV: "<<balances[0]<<'\n';
    outfile<<"Electricity: "<<balances[1]<<'\n';
    outfile<<"Gas: "<<balances[2]<<'\n';
    outfile<<"Telecommunication: "<<balances[3]<<'\n';
    outfile<<"Water: "<<balances[4]<<'\n';

    infile.close();
    outfile.close();
    
    pthread_exit(NULL);

}

/*
 * This function reads a text file using inline parameter and fills "customer_info" and "bill_info" arrays with the   
 * necessary information. Also, "TOTAL_CUSTOMERS" is set here.
 * 
 * @param infile ifstream object used to read input file.
 * @param customer_info array of pair<int,int> objects which stores, after the completion of this function, 
 * ATM ID and sleep time for each transaction
 * @param bill_infor array of pair<string,int> objects which stores, after the completion of this function, 
 * bill type and bill amount for each transaction
 * @param TOTAL_CUSTOMERS global variable indicating the total number of customers.
 * 
 * @return void
 */
void parse_input(ifstream& infile, pair<int,int> customer_info[], pair<string,int> bill_info[], int& TOTAL_CUSTOMERS){
        
    int sleep_time, ATM_instance, bill_amount; 
    int cust_id = 0;
    string line, bill_type;

    infile >> TOTAL_CUSTOMERS;
    getline(infile, line);  // move to second line

    /* Read each line */
    while (getline(infile, line)){

        stringstream linestream(line);
        string token;

        for (int i = 0; i < 4 ; i++) {
            
            getline(linestream, token, ',');
            
            if (i==0) {
                sleep_time = stoi(token);
            } 
            else if (i == 1) {
                ATM_instance = stoi(token);
            }
            else if (i == 2) {
                bill_type = token;
            }
            else if (i == 3) {
                bill_amount = stoi(token);
            }
        }

        /* Fill the arrays given as parameter */
        pair<int,int> cust_info_pair;
        pair<string, int> bill_info_pair;

        cust_info_pair.first = ATM_instance-1;
        cust_info_pair.second = sleep_time;
        bill_info_pair.first = bill_type;
        bill_info_pair.second = bill_amount;

        customer_info[cust_id] = cust_info_pair;
        bill_info[cust_id] = bill_info_pair;

        cust_id++;
    }
}


/*
 * This function is executed by each ATM thread. Basically, it realizes the transaction and changes the balance
 * of accounts. Finally, it logs the transactions.
 *
 * @return void
 */
void *atm_serve_func(void *ptr){
    
    int atm_id = *(int *) ptr;  // ID of the atm machine

    /* Transaction information */
    int cust_id, bill_amount, bill_type_i;
    string bill_type;

    /* ATM machine runs in infinite loop. When all customers are served, it breaks and thread terminates. */
    while(1){

        /* Thread waits here until a transaction order by a customer is arrived */ 
        while(atm_busy[atm_id] == -1 && !ALL_CUSTOMERS_SERVED){;}

        if (ALL_CUSTOMERS_SERVED)
            break;
        
        /* ATM thread obtains the necessary information from shared variables set by main thread. */
        cust_id = atm_busy[atm_id];
        bill_amount = bill_info[cust_id].second;
        bill_type = bill_info[cust_id].first;

        if(bill_type.compare("cableTV") == 0){
            bill_type_i = 0;
        }
        else if(bill_type.compare("electricity") == 0){
            bill_type_i = 1;
        }
        else if(bill_type.compare("gas") == 0){
            bill_type_i = 2;
        }
        else if(bill_type.compare("telecommunication") == 0){
            bill_type_i = 3;
        }
        else if(bill_type.compare("water") == 0){
            bill_type_i = 4;
        }

        /* For each bill type, different mutexes are used, because each type has different balances. */
        pthread_mutex_lock(&transaction_mutexes[bill_type_i]);

        balances[bill_type_i] += bill_amount;


        /* Loggins section must be critical section for all ATM threads. */
        pthread_mutex_lock(&log_mutex);
        outfile<<"Customer"<<cust_id+1<<","<<bill_amount<<"TL"<<","<<bill_type<<'\n';
        pthread_mutex_unlock(&log_mutex);


        pthread_mutex_unlock(&transaction_mutexes[bill_type_i]);

        atm_busy[atm_id] = -1;  // ATM machine makes itself available again after completed the transaction. This also
                                // sends the signal of completion to the customer thread. 
    }

    pthread_exit(NULL);
}


/*
 * This function is executed by each customer thread. Basically, it triggers the ATM machine after it is awaken and 
 * starts the transaction by changing "atm_busy" array.
 *
 * @return void
 */
void *customer_func(void *ptr){
    
    int cust_id = *(int *) ptr; // ID of the customer
    
    /* For sleeping, nanasleep is utilized, because usleep is out-of-date. */
    struct timespec ts = {0, customer_info[cust_id].second*1000000L };
    nanosleep(&ts, NULL);
    
    int atm_id = customer_info[cust_id].first;  // Customer finds the ID of ATM that it wants to use.


    /* Each ATM machine can be used by only one customer, so we need a mutex here. */ 
    pthread_mutex_lock(&atm_mutexes[atm_id]);
    atm_busy[atm_id] = cust_id;  // triggers the ATM machine
    while(atm_busy[atm_id]==cust_id){;}  // waiting for the atm to finish transaction 
    pthread_mutex_unlock(&atm_mutexes[atm_id]); 


    /* Customer counter critical section */
    pthread_mutex_lock(&customer_counter_mutex);
    customer_count++;
    if (customer_count==TOTAL_CUSTOMERS)
        ALL_CUSTOMERS_SERVED = true;
    pthread_mutex_unlock(&customer_counter_mutex);


    pthread_exit(NULL);
}
