//
// Created by Alperen Bağ on 4.11.2018.
//

#include "utilities.h"

bool ArrivalComparator::operator()(Passenger& first, Passenger& second){
    return first.arrival_time > second.arrival_time;
}

float Calculator::average(){
    return all_waiting_time/N;
}

void Calculator::reset(){
    all_waiting_time = 0;
    late_passengers = 0;
}
