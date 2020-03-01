//
// Created by Alperen Bağ on 3.11.2018.
//

#include "simulation.h"



template <class T>
void print_queue(priority_queue<Passenger, vector<Passenger>, T> q) {
    cout<<"que start"<<endl;
    while(!q.empty()) {
        cout << q.top();
        q.pop();
    }
    cout<<"que end"<<endl;

   }

void print_vector(vector<Passenger> vec){

    cout<<"vec start"<<endl;
    for (vector<Passenger>::iterator it = vec.begin(); it != vec.end(); ++it){
        cout << *it;
    }
    cout<<"vec end"<<endl;

}



void timer(int vip_mode, int online_ticket_mode, priority_queue<Passenger, vector<Passenger>, ArrivalComparator>& first_que, priority_queue<Passenger>& luggage_que, priority_queue<Passenger>& security_que,
           vector<Passenger>& in_luggage_counters, vector<Passenger>& in_security_counters,
           const int& L, const int& S, int& global_time, int& total_passengers, Calculator& calculator) {

    for (vector<Passenger>::iterator it = in_security_counters.begin(); it != in_security_counters.end(); ++it) {
        if (it->security_time_t == it->time_in_security + 1) {
            if(it->boarding_time<global_time){
                calculator.late_passengers++;
            }
            in_security_counters.erase(it);
            it--;
            total_passengers--;
        } else {
            it->time_in_security++;
        }
    }

    while (true) {
        if (in_security_counters.size() < S && !security_que.empty()) {
            in_security_counters.push_back(security_que.top());
            security_que.pop();
        } else {
            break;
        }
    }

    for (vector<Passenger>::iterator it = in_luggage_counters.begin(); it != in_luggage_counters.end(); ++it) {
        if (it->luggage_time_t == it->time_in_luggage + 1) {
            if(vip_mode && it->is_vip){
                if(it->boarding_time<global_time){
                    calculator.late_passengers++;
                }
                total_passengers--;
            }
            else if(in_security_counters.size()<S) {
                in_security_counters.push_back(*it);
            }else{
                it->luggage_leaving_time = global_time;
                security_que.push(*it);
            }
            in_luggage_counters.erase(it);
            it--;
        } else {
            it->time_in_luggage++;
        }
    }

    while (true){
        if(in_luggage_counters.size() < L && !luggage_que.empty()) {
            in_luggage_counters.push_back(luggage_que.top());
            luggage_que.pop();
        } else {
            break;
        }
    }

    while(true){
        if(!first_que.empty() && first_que.top().arrival_time==global_time){
            total_passengers++;

            if(online_ticket_mode && !first_que.top().have_luggage){
                if(vip_mode && first_que.top().is_vip){
                    total_passengers--;
                    if(first_que.top().boarding_time<global_time){
                        calculator.late_passengers++;
                    }
                }
                else if(in_security_counters.size()<S){
                    in_security_counters.push_back(first_que.top());
                }else {
                    Passenger temp = first_que.top();
                    temp.luggage_leaving_time = global_time;
                    security_que.push(temp);
                }
            }

            else if(in_luggage_counters.size()<L){
                in_luggage_counters.push_back(first_que.top());
            }else {
                luggage_que.push(first_que.top());
            }
            first_que.pop();
        }else{
            break;
        }
    }
    calculator.all_waiting_time+=total_passengers;
/*
    cout<<"FIRST QUE"<<endl;
    print_queue(first_que);

    cout<<"LUGGAGE QUE"<<endl;
    print_queue(luggage_que);

    cout<<"LUGGAGE COUNTERS"<<endl;
    print_vector(in_luggage_counters);

    cout<<"SECURITY QUE"<<endl;
    print_queue(security_que);

    cout<<"SECURITY COUNTERS"<<endl;
    print_vector(in_security_counters);
    cout << endl;
*/
    global_time++;

}
