//
// Created by Alperen Bağ on 3.11.2018.
//
#ifndef PROJECT2_SIMULATION_H
#define PROJECT2_SIMULATION_H


#include "utilities.h"

void timer(int vip_mode, int online_ticket_mode ,priority_queue<Passenger, vector<Passenger>, ArrivalComparator>& first_que ,priority_queue<Passenger>& luggage_que, priority_queue<Passenger>& security_que,
        vector<Passenger>& in_luggage_counters, vector<Passenger>& in_security_counters,
        const int& L ,const int& S, int& global_time, int& total_passengers, Calculator& calculator);



#endif //PROJECT2_SIMULATION_H
