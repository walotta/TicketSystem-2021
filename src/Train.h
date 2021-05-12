//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_TRAIN_H
#define TICKETSYSTEM_2021_TRAIN_H

#include "include/FileStorage.h"

typedef const string & str;

struct Time
{
    int day=0,hour=0,minute=0;
    Time()=default;
    ~Time()=default;
    Time(int h,int m):hour(h),minute(m){}
    Time(const Time &t)=default;

    Time &operator+=(int m)
    {
        minute+=m;
        if(minute>=60)
        {
            hour+=minute/60;
            minute%=60;
        }
        if(hour>=24)
        {
            day+=hour/24;
            hour%=24;
        }
        return *this;
    }
    Time operator+(int m) const
    {
        Time temp(*this);
        temp+=m;
        return temp;
    }
    Time &operator=(const Time &t)
    {
        if(this==&t) return *this;
        day=t.day;
        hour=t.hour;
        minute=t.minute;
        return *this;
    }
};
struct Date
{
    const int month=0,day=0;
    Date()=default;
    ~Date()=default;
    Date(int m,int d):month(m),day(d){}
    Date(const Date &d)=default;
};

class Train
{
    struct Station
    {
        string name;
        int price;
        int arrival,departure;
    };

    vector<Station> station;
    string trainID;
    int stationNum;// Z,(1,100]
    int seatNum;// <1e5
    Time startTime;
    Date sale_beg,sale_end;
    bool If_release;
    char type;

public:
    Train()=default;
    ~Train()=default;



};


#endif //TICKETSYSTEM_2021_TRAIN_H
