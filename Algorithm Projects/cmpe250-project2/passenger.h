//
// Created by Alperen Bağ on 30.10.2018.
//

#ifndef PROJECT2_PASSENGER_H
#define PROJECT2_PASSENGER_H

#include "queue"
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;


class Passenger{
public:
    Passenger():time_in_luggage(0),time_in_security(0){};
    Passenger(int arrival, int boarding, int luggage_time,
            int security_time, bool _is_vip, bool _have_luggage);
    static int passenger_fffs_mode;
    int arrival_time;
    int boarding_time;
    int luggage_time_t;
    int security_time_t;
    bool is_vip;
    bool have_luggage;
    int luggage_leaving_time;
    int time_in_luggage;
    int time_in_security;


    friend bool operator<(const Passenger& p1, const Passenger& p2){
        if(passenger_fffs_mode==1) {
            if (p1.boarding_time > p2.boarding_time)
                return 1;
            else if (p1.boarding_time == p2.boarding_time)
                return p1.arrival_time > p2.arrival_time;
            else{
                return 0;
            }
        }else{
    //        if((p1.time_in_luggage == 0 && p2.time_in_luggage == 0))      //FQ->LQ or FQ->SQ for both passengers
      //          return p1.arrival_time > p2.arrival_time;

            if(p1.luggage_leaving_time > p2.luggage_leaving_time){
                return 1;
            } else if(p1.luggage_leaving_time == p2.luggage_leaving_time){
                return p1.arrival_time > p2.arrival_time;
            } else {
                return 0;
            }
        }
    }
};

ostream &operator<<(std::ostream &os, const Passenger &x);


#endif //PROJECT2_PASSENGER_H

/*
   friend bool operator>(const Passenger& p1, const Passenger& p2){
        if(p1.boarding_time < p2.boarding_time)
            return 1;
        else if(p1.boarding_time==p2.boarding_time)
            return p1.arrival_time < p2.boarding_time;
        return 0;
    }
    Passenger(const Passenger& other){
        this->arrival_time = other.arrival_time;
        this->boarding_time = other.boarding_time;
        this->luggage_time_t = other.luggage_time_t;
        this->security_time_t = other.security_time_t;
        this->is_vip = other.is_vip;
        this->have_luggage = other.have_luggage;
        this->time_in_luggage = other.time_in_luggage;
        this->time_in_security = other.time_in_security;
        this->is_late = other.is_late;
        this->total_time_spent = other.total_time_spent;
    }
    Passenger(const Passenger&& other){
        this->arrival_time = other.arrival_time;
        this->boarding_time = other.boarding_time;
        this->luggage_time_t = other.luggage_time_t;
        this->security_time_t = other.security_time_t;
        this->is_vip = other.is_vip;
        this->have_luggage = other.have_luggage;
        this->time_in_luggage = other.time_in_luggage;
        this->time_in_security = other.time_in_security;
        this->is_late = other.is_late;
        this->total_time_spent = other.total_time_spent;
    }
    Passenger& operator=(const Passenger& other){
        this->arrival_time = other.arrival_time;
        this->boarding_time = other.boarding_time;
        this->luggage_time_t = other.luggage_time_t;
        this->security_time_t = other.security_time_t;
        this->is_vip = other.is_vip;
        this->have_luggage = other.have_luggage;
        this->time_in_luggage = other.time_in_luggage;
        this->time_in_security = other.time_in_security;
        this->is_late = other.is_late;
        this->total_time_spent = other.total_time_spent;
        return *this;
    }
    Passenger& operator=(const Passenger&& other){
        this->arrival_time = other.arrival_time;
        this->boarding_time = other.boarding_time;
        this->luggage_time_t = other.luggage_time_t;
        this->security_time_t = other.security_time_t;
        this->is_vip = other.is_vip;
        this->have_luggage = other.have_luggage;
        this->time_in_luggage = other.time_in_luggage;
        this->time_in_security = other.time_in_security;
        this->is_late = other.is_late;
        this->total_time_spent = other.total_time_spent;
        return *this;
    }
*/

