//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USER_H
#define TICKETSYSTEM_2021_USER_H

//#include <string>
#include "base.h"


class User
{
    MyString username;// Consist of number, alphabet and '_', and its head must be alphabet, with length<=20.
    MyString password;// Consist of number, alphabet and '_', with length<=30.
    MyString name;// Consist of Chinese character, with 2<=length<=5.
    MyString mailAddr;// Consist of number, alphabet, '@' & '.', with length<=30, needless to check its legality.
    int privilege;// Z,[0,10]

public:
    User()=default;
    ~User()=default;
    User(str u,str p,str n,str m,int g): username(u),password(p),name(n),mailAddr(m),privilege(g){}

    bool check_pass(str p) const { return password==p; }
    int pri() const { return privilege; }
    string display() const
    {
        return (string)username+" "+(string)name+" "+(string)mailAddr+" "+to_string(privilege);
    }

};

class Log
{
    MyString status;
    MyString username;
    MyString trainID;
    MyString From,To;
    Date leave_date,arrive_date;
    Time leave_time,arrive_time;
    int price,num;
public:
    Log()=default;
    Log(str u,str s,str i,str f,str t,Date ld,Date ad,Time lt,Time at,int p,int n):leave_date(ld),arrive_date(ad)
    {
        username=u;
        status=s;
        trainID=i;
        From=f; To=t;
        leave_time=lt; arrive_time=at;
        price=p; num=n;
    }
    string display()
    {
        string output;
        output="["+(string)status+"] "+(string)trainID+" "+(string)From+" ";
        output+=leave_date.display()+" "+leave_time.display()+" -> ";
        output+=(string)To+" "+arrive_date.display()+" "+arrive_time.display()+" ";
        output+=to_string(price)+" "+to_string(num);
        return output;
    }
    void modify_status(str s)
    {
        status=s;
    }
};




#endif //TICKETSYSTEM_2021_USER_H
