//
// Created by Fourest on 2021/5/13.
//

#ifndef TICKETSYSTEM_2021_BASE_H
#define TICKETSYSTEM_2021_BASE_H

#include "include/StorageManger.h"
#include <unordered_set>
#include <set>


typedef const string & str;
typedef const vector<int> & vecI;
typedef vector<string> vecS;

struct Time
{
    int day=0,hour=0,minute=0;
    Time()=default;
    ~Time()=default;
    explicit Time(int h,int m):hour(h),minute(m){}
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
    int month=0,day=0;
    Date()=default;
    ~Date()=default;
    explicit Date(int m,int d):month(m),day(d){}
    Date(const Date &d)=default;
    string display() const
    {
        string m=to_string(month),d=to_string(day);
        if(m.size()==1) m="0"+m;
        if(d.size()==1) d="0"+d;
        return m+"-"+d;
    }
};

// This structure records the minutes passed after 6.1 00:00.
struct RealTime
{
    constexpr static int days[4]={30,31,31,30};
    static const int base_mon=6;

    long long minutes=0;

    RealTime()=default;
    RealTime(const Date &d,const Time &t)
    {
        for(int i=base_mon;i<d.month;++i) minutes+=days[i-base_mon]*24*60;
        minutes+=(d.day-1)*24*60;
        minutes+=t.hour*60+t.minute;
    }
    Date date() const
    {
        auto day=minutes/(24*60);
        int month=0;
        while(day>days[month]) day-=days[month++];
        return Date(month+base_mon,day);
    }
    Time time() const
    {
        int min=minutes%(24*60);
        return Time(min/60,min%60);
    }
};








#endif //TICKETSYSTEM_2021_BASE_H
