//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
#define TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H

#include "TrainManager.h"
#include "UserManager.h"


class ManagementSystem
{

    struct Order
    {
        int serial_number;
        int id;// Represent the number of the order in its user's list.
        MyString user;
        MyString trainID;// The tag
        Date date;
        MyString start;
        MyString arrive;
        int number;

        Order()=default;
        Order(int k,int _id,str u,str i,const Date &d,str f,str t,const int &n):serial_number(k),id(_id),user(u),trainID(u),date(d),start(f),arrive(t),number(n){}
        bool operator<(const Order &o) const {return serial_number<o.serial_number;}
    };
    typedef StorageManger<Order,300,300,300,300,300> OD;


    TrainManager train;
    UserManager user;
    OD order;

    void add_order(int id,str u,str i,const Date &d,str f,str t,const int &n);

public:
    ManagementSystem():order("order.dat",true){}
    ~ManagementSystem()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);
    void query_train(str i,Date d,vecS &out);
    bool delete_train(str i);
    void query_ticket(str s,str t,Date d,bool If_time,vecS &out);
    void query_transfer(str s,str t,Date d,bool If_time,vecS &out);
    string buy_ticket(str u,str i,const Date &d,str f,str t,int n,bool q);

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    void query_order(str u,vecS &out); // If query failed, return an empty vector.

    int query_user_priority(str u);
    bool refund_ticket(str u,int n);
    bool clean();
};


#endif //TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
