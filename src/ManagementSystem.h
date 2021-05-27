//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
#define TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H

#include "TrainManager.h"
#include "UserManager.h"


class ManagementSystem
{

    TrainManager train;
    UserManager user;

public:
    ManagementSystem()=default;
    ~ManagementSystem()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);
    vecS query_train(str i,Date d);
    bool delete_train(str i);
    vecS query_ticket(str s,str t,Date d,bool If_time);
    vecS query_transfer(str s,str t,Date d,bool If_time);
    string buy_ticket(str u,str i,Date d,str f,str t,int n,bool q);


    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    vecS query_order(str u); // If query failed, return an empty vector.

    bool refund_ticket(str u,int n);
    bool clean();
};


#endif //TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
