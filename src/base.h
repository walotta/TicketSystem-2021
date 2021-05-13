//
// Created by Fourest on 2021/5/13.
//

#ifndef TICKETSYSTEM_2021_BASE_H
#define TICKETSYSTEM_2021_BASE_H

#include "include/FileStorage.h"
typedef const string & str;
typedef const vector<int> & vecI;
typedef vector<string> vecS;

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

    string display() const
    {
        string h=to_string(hour),m=to_string(minute);
        if(h.size()==1) h="0"+h;
        if(m.size()==1) m="0"+m;
        return h+":"+m;
    }
};
struct Date
{
    const int month=0,day=0;
    Date()=default;
    ~Date()=default;
    Date(int m,int d):month(m),day(d){}
    Date(const Date &d)=default;
    string display() const
    {
        string m=to_string(month),d=to_string(day);
        if(m.size()==1) m="0"+m;
        if(d.size()==1) d="0"+d;
        return m+"-"+d;
    }
};










#endif //TICKETSYSTEM_2021_BASE_H
