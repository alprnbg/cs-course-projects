#include "simulation.h"
#include <fstream>
#include <string>
#include <iterator>

int main(int argc, char* argv[]) {
//    clock_t start = clock();

    int N, L, S, global_time = 0, total_passengers = 0;
    priority_queue<Passenger> luggage_que, security_que;
    priority_queue<Passenger, vector<Passenger>, ArrivalComparator>  passengers, first_que;
    vector<Passenger> passengers_in_luggage_counters;
    vector<Passenger> passengers_in_security_counters;
    float results[8][2];
    int modes[8][3] = {{0,0,0}, {1,0,0}, {0,1,0}, {1,1,0}, {0,0,1}, {1,0,1}, {0,1,1}, {1,1,1}};
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1]);
    output_file.open(argv[2]);
    input_file>>N>>L>>S;
    Calculator calculator(N);
    int i1,i2,i3, i4, i5 = 0, i6 = 0;
    char c5,c6;
    for(int i = 0; i < N; i++){
        input_file>>i1>>i2>>i3>>i4>>c5>>c6;
        if(c5=='V')
            i5 = 1;
        if(c6=='L')
            i6 = 1;
        Passenger passenger(i1, i2, i3, i4, i5, i6);
        i5 = i6 = 0;
        passengers.push(passenger);
    }
    input_file.close();
    first_que = passengers;

    for(int i = 0 ; i < 8 ; i++) {
        int fffs_mode = modes[i][0];
        int vip_mode = modes[i][1];
        int online_ticket_mode = modes[i][2];
        Passenger::passenger_fffs_mode = fffs_mode;
        while (!first_que.empty() || !luggage_que.empty() || !security_que.empty() ||
               !passengers_in_luggage_counters.empty() || !passengers_in_security_counters.empty()) {
        //    cout << "TIME = " << global_time << endl;
            timer(vip_mode, online_ticket_mode, first_que, luggage_que, security_que,
                  passengers_in_luggage_counters,
                  passengers_in_security_counters, L, S, global_time, total_passengers, calculator);
        //    cout << "Total passengers : " << total_passengers << endl;
        //    cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
        }
        results[i][0] =calculator.average();
        results[i][1] =calculator.late_passengers;
        calculator.reset();
        first_que = passengers;
        global_time = total_passengers = 0;
      }

    for(int i = 0 ; i < 8 ; i++){
        output_file<<results[i][0]<<" "<<results[i][1]<<endl;
    }
    output_file.close();
//    clock_t stop = clock();
//    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
//    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}