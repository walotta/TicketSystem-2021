//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_TRAIN_H
#define TICKETSYSTEM_2021_TRAIN_H

#include "include/FileStorage.h"

class Train
{
    struct Station
    {
        string name;
        int price;
        int arrival,departure;
    };
    struct Time
    {
        int day,hour,minute;
    };
    struct Date
    {
        int month,day;
    };

    vector<Station> station;
    string trainID;
    int stationNum;// Z,(1,100]
    int seatNum;// <1e5
    Time startTime;
    Date sale_beg,sale_end;
    bool If_release;

public:
    Train()=default;
    ~Train()=default;


};


#endif //TICKETSYSTEM_2021_TRAIN_H
