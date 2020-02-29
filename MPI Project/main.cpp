// export OMPI_MCA_btl=self,tcp
// sudo launchctl limit maxfiles 65536 200000
// ulimit -n 65536


#include <iostream>
#include <fstream>
#include "mpi.h"
#include "string"
#include <iterator>
#include <algorithm>
#include <cmath>

#define MAP_SIZE 360    // Size of the input map


/**
 * @brief Initializes the map reading the input file
 * 
 * @param map Container for the map
 * @param input_file File from which the input is read
 */
void create_map(int map[MAP_SIZE][MAP_SIZE], std::ifstream& input_file){

    int data;

    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++) {
            input_file >> data;
            map[i][j] = data;
        }
    }
}

/**
 * @brief Finds the coordinates of the top left box on the area for which core with rank "r" is responsible
 * 
 * @param rank Rank of the core
 * @param sqrt_size Square root of the number of worker cores
 * @return std::pair<int,int> Coordinates of the top left box
 */
std::pair<int,int> rank_2_area(int rank, int sqrt_size){

    int rank_row, rank_column;
    rank_row = (rank-1) / sqrt_size;
    rank_column = (rank-1) % sqrt_size;
    std::pair<int,int> top_left(rank_row*(MAP_SIZE/sqrt_size),rank_column*(MAP_SIZE/sqrt_size));

    return top_left;
}

/**
 * @brief Calculates the position of the area for which core with rank "r" is responsible
 * 
 * For example, if the number of worker cores is 16, then the core with rank 1 is responsible from the area whose position is (0,0).
 * Likewise the core with rank 2 is responsible from the area whose position is (0,1). It is (0,2) for the rank 3 core.
 * 
 * @param rank Rank of the core
 * @param sqrt_size Square root of the number of worker cores 
 * @return std::pair<int,int> Position of the area
 */
std::pair<int,int> rank_2_row_col(int rank, int sqrt_size){
    int rank_row, rank_column;
    rank_row = (rank-1) / sqrt_size;
    rank_column = (rank-1) % sqrt_size;
    std::pair<int,int> p(rank_row,rank_column);
    return p;
}


int main(int argc, char* argv[]) {

    MPI_Init(&argc,&argv);

    /**
     * size : # of worker cores
     * rank : rank of the core
     * sqrt_size : square root of # of worker cores
     * T : # of iterations
	 */
    int size, rank, sqrt_size, T;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_workers = size-1;
    sqrt_size = sqrt(num_workers);

    // Length of one side of the area for which one core is responsible
    int segment_sqrt_size = MAP_SIZE/sqrt_size;
    T = std::stoi(argv[3]);

    if(rank==0) {
    	
    	// Manager Core
    	// Handles input-output and creates the initial map before send the initial information to worker cores

        std::string input_file_name = argv[1];
        std::string output_file_name = argv[2];

        std::ifstream input_file;
        std::ofstream output_file;

        int map[MAP_SIZE][MAP_SIZE];

        input_file.open(input_file_name);
        output_file.open(output_file_name);

        create_map(map, input_file);


        // Sends initial info to every processor

        for (int dest = 1; dest <= num_workers; dest++) {

        	// Initializes the array which will be sent to workers
        	int map_to_send[segment_sqrt_size][segment_sqrt_size];

        	// We need to find the area for which worker "dest" is responsible
            std::pair<int,int> top_left = rank_2_area(dest, sqrt_size);

            for (int i = top_left.first; i<top_left.first+segment_sqrt_size; i++) {
                for (int j = top_left.second; j < top_left.second + segment_sqrt_size; j++) {
                    map_to_send[i - top_left.first][j - top_left.second] = map[i][j];
                }
            }

            // Sends "map_to_send" to worker "dest"
            MPI_Send(&map_to_send, segment_sqrt_size * segment_sqrt_size, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }


        // Receives the final maps and constructs "map" 

        int recv_map[segment_sqrt_size][segment_sqrt_size];
        std::pair<int,int> top_left_coor;

        for(int worker_r = 1; worker_r <= num_workers; worker_r++){
            MPI_Recv(&recv_map, segment_sqrt_size*segment_sqrt_size, MPI_INT, worker_r, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            top_left_coor = rank_2_area(worker_r, sqrt_size);
            for(int i = top_left_coor.first; i < top_left_coor.first+segment_sqrt_size; i++){
                for(int j = top_left_coor.second; j < top_left_coor.second+segment_sqrt_size; j++){
                    map[i][j] = recv_map[i-top_left_coor.first][j-top_left_coor.second];
                }
            }
        }

        // Prints the final map to the output file  
        for(int i = 0; i < MAP_SIZE; i++){
            for (int j = 0; j < MAP_SIZE; j++){
                output_file << map[i][j] << " ";
            }
            output_file << '\n';
        }

        input_file.close();
        output_file.close();
    }

    else{

    	// Worker Cores (rank = r)

        // Receives the initial info from the manager core
        int local_map[segment_sqrt_size][segment_sqrt_size];
        MPI_Recv(&local_map, segment_sqrt_size*segment_sqrt_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Worker learns its position
        std::pair<int,int> rank_row_col;
        rank_row_col = rank_2_row_col(rank, sqrt_size);

        // Necessary containers for the information which will be sent and recieved.
        int recv_top_row[segment_sqrt_size];
        int recv_bottom_row[segment_sqrt_size];
        int recv_left_row[segment_sqrt_size];
        int recv_right_row[segment_sqrt_size];

        // recv_[top|bottom]_corners[1] -> [top|bottom]-left corner, recv_[top|bottom]_corners[2] -> [top|bottom]-right corner
        int recv_top_corners[2], recv_bottom_corners[2];	

        int send_top_row[segment_sqrt_size];
        int send_bottom_row[segment_sqrt_size];
        int send_left_row[segment_sqrt_size];
        int send_right_row[segment_sqrt_size];

        // send_[top|bottom]_corners[1] -> [top|bottom]-left corner, send_[top|bottom]_corners[2] -> [top|bottom]-right corner
        int send_top_corners[2], send_bottom_corners[2];


        // Calculates the ranks of neighbor cores

        int top_neigh = rank - sqrt_size;
        if (top_neigh < 1)
            top_neigh += num_workers;

        int bottom_neigh = rank + sqrt_size;
        if (bottom_neigh > num_workers)
            bottom_neigh -= num_workers;

        int left_neigh = rank - 1;
        if (rank % sqrt_size == 1)
            left_neigh += sqrt_size;

        int right_neigh = rank + 1;
        if (rank % sqrt_size == 0)
            right_neigh -= sqrt_size;


        // Main Part for the worker

        for(int step = 0; step < T; step++) {

        	// Prepares the arrays that will be sent to neighbors

            std::copy(local_map[0], local_map[0] + segment_sqrt_size, send_top_row);
            std::copy(local_map[segment_sqrt_size - 1], local_map[segment_sqrt_size - 1] + segment_sqrt_size,
                      send_bottom_row);
            for (int i = 0; i < segment_sqrt_size; i++)
                send_left_row[i] = local_map[i][0];
            for (int i = 0; i < segment_sqrt_size; i++)
                send_right_row[i] = local_map[i][segment_sqrt_size - 1];


            // Communication Part

            // even -> odd (top-to-bottom)
            if (rank_row_col.first % 2 == 0) {
                MPI_Send(&send_bottom_row, segment_sqrt_size, MPI_INT, bottom_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<bottom_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_top_row, segment_sqrt_size, MPI_INT, top_neigh, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<top_neigh<<" recv "<<rank<<std::endl;
            }

            // odd -> even (top-to-bottom)
            if (rank_row_col.first % 2 != 0) {
                MPI_Send(&send_bottom_row, segment_sqrt_size, MPI_INT, bottom_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<bottom_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_top_row, segment_sqrt_size, MPI_INT, top_neigh, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<top_neigh<<" recv "<<rank<<std::endl;
            }

            // odd -> even (bottom-to-top)
            if (rank_row_col.first % 2 != 0) {
                MPI_Send(&send_top_row, segment_sqrt_size, MPI_INT, top_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<top_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_bottom_row, segment_sqrt_size, MPI_INT, bottom_neigh, 2, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                //std::cout<<bottom_neigh<<" recv "<<rank<<std::endl;
            }

            // even -> odd (bottom-to-top)
            if (rank_row_col.first % 2 == 0) {
                MPI_Send(&send_top_row, segment_sqrt_size, MPI_INT, top_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<top_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_bottom_row, segment_sqrt_size, MPI_INT, bottom_neigh, 2, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                //std::cout<<bottom_neigh<<" recv "<<rank<<std::endl;
            }


            // even -> odd (left-to-right)
            if (rank_row_col.second % 2 == 0) {
                MPI_Send(&send_right_row, segment_sqrt_size, MPI_INT, right_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<right_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_left_row, segment_sqrt_size, MPI_INT, left_neigh, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<left_neigh<<" recv "<<rank<<std::endl;
            }

            // odd -> even (left-to-right)
            if (rank_row_col.second % 2 != 0) {
                MPI_Send(&send_right_row, segment_sqrt_size, MPI_INT, right_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<right_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_left_row, segment_sqrt_size, MPI_INT, left_neigh, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<left_neigh<<" recv "<<rank<<std::endl;
            }

            // odd -> even (right-to-left)
            if (rank_row_col.second % 2 != 0) {
                MPI_Send(&send_left_row, segment_sqrt_size, MPI_INT, left_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<left_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_right_row, segment_sqrt_size, MPI_INT, right_neigh, 2, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                //std::cout<<right_neigh<<" recv "<<rank<<std::endl;
            }

            // even -> odd (right-to-left)
            if (rank_row_col.second % 2 == 0) {
                MPI_Send(&send_left_row, segment_sqrt_size, MPI_INT, left_neigh, 2, MPI_COMM_WORLD);
                //std::cout<<rank<<" sent "<<left_neigh<<std::endl;
            } else {
                MPI_Recv(&recv_right_row, segment_sqrt_size, MPI_INT, right_neigh, 2, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                //std::cout<<right_neigh<<" recv "<<rank<<std::endl;
            }


    		// Now that the worker got the information of neighbor sides, 
    		// we can prepare the corner information for sending

            send_top_corners[0] = recv_left_row[0];
            send_top_corners[1] = recv_right_row[0];
            send_bottom_corners[0] = recv_left_row[segment_sqrt_size-1];
            send_bottom_corners[1] = recv_right_row[segment_sqrt_size-1];


	        // Now we can transfer the corner info
            // top-to-bottom twice, then bottom-to-top twice

            // even -> odd (top-to-bottom)
            if (rank_row_col.first % 2 == 0){
                MPI_Send(&send_bottom_corners, 2, MPI_INT, bottom_neigh, 3, MPI_COMM_WORLD);
                //std::cout<<3<<std::endl;
            }else{
                MPI_Recv(&recv_top_corners, 2, MPI_INT, top_neigh, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<3<<std::endl;
            }

            // odd -> even (top-to-bottom)
            if (rank_row_col.first % 2 != 0){
                MPI_Send(&send_bottom_corners, 2, MPI_INT, bottom_neigh, 3, MPI_COMM_WORLD);
                //std::cout<<3<<std::endl;
            }else{
                MPI_Recv(&recv_top_corners, 2, MPI_INT, top_neigh, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<3<<std::endl;
            }

            // odd -> even (bottom-to-top)
            if (rank_row_col.first % 2 != 0){
                MPI_Send(&send_top_corners, 2, MPI_INT, top_neigh, 3, MPI_COMM_WORLD);
                //std::cout<<3<<std::endl;
            }else{
                MPI_Recv(&recv_bottom_corners, 2, MPI_INT, bottom_neigh, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<3<<std::endl;
            }

            // even -> odd (bottom-to-top)
            if (rank_row_col.first % 2 == 0){
                MPI_Send(&send_top_corners, 2, MPI_INT, top_neigh, 3, MPI_COMM_WORLD);
                //std::cout<<3<<std::endl;
            }else{
                MPI_Recv(&recv_bottom_corners, 2, MPI_INT, bottom_neigh, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //std::cout<<3<<std::endl;
            }

            // All necessary information is transferred

            // Simulation Part

            // Container for the new map after one iteration
            int new_local_map[segment_sqrt_size][segment_sqrt_size];
            int count_neigh;
            
            // Iterates all boxes in the area which is in its responsibility
            for (int i = 0; i < segment_sqrt_size; i++){
                for (int j = 0; j < segment_sqrt_size; j++){

                	// Creates the new local map according to the rules of the game
                	// Neighbor information are obtained from the local map when it is available
                	// If it is not, the recieved arrays from the neighbors are used

                    if (i==0 || j == 0 || i == segment_sqrt_size-1 || j == segment_sqrt_size-1){
                    	
                    	// When the i,j on one of the sides of the area

                        if (i == 0 && j == 0){
                            count_neigh = recv_top_corners[0] + recv_top_row[0] + recv_top_row[1] +
                                          recv_left_row[0] + local_map[i][j+1] + recv_left_row[1] + local_map[i+1][j] +
                                          local_map[i+1][j+1];
                        }
                        else if (i == 0 && j == segment_sqrt_size-1) {
                            count_neigh = recv_top_row[segment_sqrt_size-2] + recv_top_row[segment_sqrt_size-1] + recv_top_corners[1] +
                                    local_map[i][j-1] + recv_right_row[0] + local_map[i+1][j-1] + local_map[i+1][j] +
                                    recv_right_row[1];
                        }
                        else if (i == segment_sqrt_size-1 && j == 0) {
                            count_neigh = recv_left_row[segment_sqrt_size-2] + local_map[i-1][j] + local_map[i-1][j+1] +
                                    recv_left_row[segment_sqrt_size-1] + local_map[i][j+1] + recv_bottom_corners[0] + recv_bottom_row[0] +
                                    recv_bottom_row[1];
                        }
                        else if (i == segment_sqrt_size-1 && j == segment_sqrt_size-1) {
                            count_neigh = local_map[i-1][j-1] + local_map[i-1][j] + recv_right_row[segment_sqrt_size-2] +
                                          local_map[i][j-1] + recv_right_row[segment_sqrt_size-1] + recv_bottom_row[segment_sqrt_size-2]
                                          + recv_bottom_row[segment_sqrt_size-1] + recv_bottom_corners[1];
                        }
                        else if(i==0){
                            // top row
                            count_neigh = recv_top_row[j-1] + recv_top_row[j] + recv_top_row[j+1] + local_map[i][j-1] +
                                    local_map[i][j+1] + local_map[i+1][j-1] + local_map[i+1][j] + local_map[i+1][j+1];
                        }
                        else if(j==0){
                            // left row
                            count_neigh = recv_left_row[i-1] + local_map[i-1][j] + local_map[i-1][j+1] +
                                    recv_left_row[i] + local_map[i][j+1] + recv_left_row[i+1] + local_map[i+1][j] +
                                    local_map[i+1][j+1];
                        }
                        else if(i==segment_sqrt_size-1){
                            // bottom row
                            count_neigh = local_map[i-1][j-1] + local_map[i-1][j] + local_map[i-1][j+1] +
                                    local_map[i][j-1] + local_map[i][j+1] + recv_bottom_row[j-1] +
                                    recv_bottom_row[j] + recv_bottom_row[j+1];
                        }
                        else if(j==segment_sqrt_size-1){
                            // right row
                            count_neigh = local_map[i-1][j-1] + local_map[i-1][j] + recv_right_row[i-1] +
                                    local_map[i][j-1] + recv_right_row[i] + local_map[i+1][j-1] + local_map[i+1][j] +
                                    recv_right_row[i+1];
                        }
                    }

                    else{

                    	// When i,j inside the area, which means we don't need any information from the neighbors

                        count_neigh = local_map[i-1][j-1] + local_map[i-1][j] + local_map[i-1][j+1] +
                                local_map[i][j-1] + local_map[i][j+1] + local_map[i+1][j-1] + local_map[i+1][j] +
                                local_map[i+1][j+1];
                    }

                    // Calculates the new value of i,j according to the rules

                    if (count_neigh <= 1 || count_neigh >= 4) {
                        new_local_map[i][j] = 0;
                    }
                    else if (count_neigh == 3) {
                        new_local_map[i][j] = 1;
                    }
                    else{
                        new_local_map[i][j] = local_map[i][j];
                    }
                }
            }

            // Updates the local map
            for (int i = 0; i < segment_sqrt_size; i++){
                for (int j = 0; j < segment_sqrt_size; j++){
                    local_map[i][j] = new_local_map[i][j];
                }
            }

        }

        // Sends the final map to manager
        MPI_Send(&local_map, segment_sqrt_size*segment_sqrt_size, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
