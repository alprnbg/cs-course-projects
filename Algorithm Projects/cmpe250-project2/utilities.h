//
// Created by Alperen Bağ on 4.11.2018.
//

#ifndef PROJECT2_UTILITIES_H
#define PROJECT2_UTILITIES_H

#include "passenger.h"

class ArrivalComparator{
public:
    bool operator()(Passenger& first, Passenger& second);
};

class Calculator{
public:
    Calculator(int _N):N(_N),all_waiting_time(0),late_passengers(0){};
    float all_waiting_time ;
    int late_passengers;
    int N;
    float average();
    void reset();
};


/*
template <class T>
void print_queue(priority_queue<Passenger, vector<Passenger>, T> q);

void print_vector(vector<Passenger> vec);
*/




#endif //PROJECT2_UTILITIES_H

