//
// Created by Alperen Bağ on 30.10.2018.
//

#include "passenger.h"

Passenger::Passenger(int arrival, int boarding, int luggage_time, int security_time, bool _is_vip,
                     bool _have_luggage) : arrival_time(arrival), boarding_time(boarding),
            security_time_t(security_time),luggage_time_t(luggage_time),
            is_vip(_is_vip), have_luggage(_have_luggage), time_in_luggage(0), time_in_security(0){};


ostream &operator<<(ostream &os, const Passenger &x) {
    return os <<endl << "Passenger { Arrival : " << x.arrival_time << ", Boarding : "<< x.boarding_time<<endl
              <<"Luggage Time Need : "<< x.luggage_time_t<<" - "<<"Luggage Time Spent : "<<x.time_in_luggage<<endl
              <<"Security Time Need : "<<x.security_time_t<<" - "<<"Security Time Spent : "<<x.time_in_security<<endl
              <<"VIP : "<<x.is_vip<<" - "<<"Have Luggage : "<<x.have_luggage<<endl;
}


int Passenger::passenger_fffs_mode = 0;