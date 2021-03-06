//
// Created by Fourest on 2021/5/13.
//

#ifndef TICKETSYSTEM_2021_BASE_H
#define TICKETSYSTEM_2021_BASE_H

#include "include/BPlusTree.h"
#include "include/StoragePool.h"
#include "include/MyString.hpp"
#include "include/STL.h"
//#include <unordered_set>
//#include <unordered_map>
#include <algorithm>
#include <string>


typedef const string & str;
typedef const vector<int> & vecI;
typedef vector<string> vecS;
typedef long long lint;
typedef std::pair<int,lint> ex_index;

struct Time;
struct Date;
struct RealTime;
const int base_mon=6;
constexpr static int days[4]={30,31,31,30};

inline unsigned long long to_int(const string& input)
{
    unsigned long long ans=0;
    for(int i=0; i<input.size(); ++i) ans=10*ans+(input[i]-'0');
    return ans;
}

static unsigned long long hash_int(str input)
{
    unsigned long long res=0;
    for(auto it:input) res=(res<<16)+res+(unsigned long long)it;
    return res;
}


struct Time
{
    int hour=0,minute=0;

    Time()=default;
    ~Time()=default;
    explicit Time(int h,int m):hour(h),minute(m){}
    Time(const Time &t)=default;
    explicit Time(str t):hour(to_int(t.substr(0,2))),minute(to_int(t.substr(3,2))){}

    Time &operator+=(int m)
    {
        minute+=m;
        if(minute>=60)
        {
            hour+=minute/60;
            minute%=60;
        }
        hour%=24;
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
        hour=t.hour;
        minute=t.minute;
        return *this;
    }

    string display() const
    {
        string h=std::to_string(hour),m=std::to_string(minute);
        if(h.size()==1) h="0"+h;
        if(m.size()==1) m="0"+m;
        return h+":"+m;
    }
    bool operator<(const Time &t) const
    {
        if(hour!=t.hour) return hour<t.hour;
        return minute<t.minute;
    }
};

struct Date
{
    int month=base_mon,day=1;

    Date()=default;
    ~Date()=default;
    explicit Date(int m,int d):month(m),day(d){}
    Date(const Date &d)=default;
    explicit Date(str date):month(to_int(date.substr(0,2))),day(to_int(date.substr(3,2))){}

    string display() const
    {
        string m=std::to_string(month),d=std::to_string(day);
        if(m.size()==1) m="0"+m;
        if(d.size()==1) d="0"+d;
        return m+"-"+d;
    }
    int dayNum() const
    {
        int output=0;
        for(int i=0;i<month-base_mon;++i) output+=days[i];
        return output+day-1;
    }

    bool operator<(const Date &d) const
    {
        if(month!=d.month) return month<d.month;
        return day<d.day;
    }
    bool operator==(const Date &d) const {return month==d.month && day==d.day;}
    bool operator<=(const Date &d) const {return *this<d || *this==d;}
    int operator-(const Date &d) const
    {
        return dayNum()-d.dayNum();
    }
    Date operator-(const int &d) const
    {
        Date temp(*this);
        temp.day-=d;
        while(temp.day<=0)
        {
            temp.day+=days[temp.month-1-base_mon];
            --temp.month;
        }
        return temp;
    }
    Date &operator++()
    {
        ++day;
        if(day>days[month-base_mon])
        {
            day=1; ++month;
        }
        return *this;
    }
    Date operator+(const int &d) const
    {
        Date temp(*this);
        temp.day+=d;
        while(temp.day>days[month-base_mon])
        {
            temp.day-=days[temp.month-base_mon];
            ++temp.month;
        }
        return temp;
    }
};
// This structure records the minutes passed after 6.1 00:00.
struct RealTime
{
    long long minutes=-404;

    RealTime()=default;
    RealTime(const RealTime &r):minutes(r.minutes){}
    explicit RealTime(const long long &m):minutes(m){}
    explicit RealTime(const Date &d,const Time &t)
    {
        minutes=0;
        for(int i=base_mon;i<d.month;++i) minutes+=days[i-base_mon]*24*60;
        minutes+=(d.day-1)*24*60;
        minutes+=t.hour*60+t.minute;
    }
    explicit RealTime(const Date &d)
    {
        minutes=0;
        for(int i=base_mon;i<d.month;++i) minutes+=days[i-base_mon]*24*60;
        minutes+=(d.day-1)*24*60;
    }
    explicit RealTime(const Time &t):minutes(t.hour*60+t.minute) {}


    Date date() const
    {
        auto day=minutes/(24*60)+1;
        int month=0;
        while(day>days[month]) day-=days[month++];
        return Date(month+base_mon,day);
    }
    Time time() const
    {
        int min=minutes%(24*60);
        return Time(min/60,min%60);
    }
    string display() const
    {
        if(minutes<0) return "xx-xx xx:xx";
        return date().display()+" "+time().display();
    }

    RealTime &operator=(const RealTime &r)
    {
        minutes=r.minutes;
        return *this;
    }
    RealTime operator+(const long long &l) const
    {
        RealTime temp(minutes+l);
        return temp;
    }
    RealTime operator+(const RealTime &r) const
    {
        RealTime temp(minutes+r.minutes);
        return temp;
    }
    long long operator-(const RealTime &r) const
    {
        return minutes-r.minutes;
    }

    bool operator< (const RealTime &t) const {return minutes<t.minutes;}
    bool operator==(const RealTime &t) const {return minutes==t.minutes;}
    bool operator<=(const RealTime &t) const {return minutes<=t.minutes;}
};


#endif //TICKETSYSTEM_2021_BASE_H
