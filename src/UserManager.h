//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USERMANAGER_H
#define TICKETSYSTEM_2021_USERMANAGER_H

#include "User.h"
#include "TrainManager.h"

class UserManager
{
    US user;
    // map<username,pair<priority,orderNum>>
    unordered_map<string,pair<int,int>> logged_users;

public:
    UserManager():user("user.dat",false){}
    ~UserManager(){logged_users.clear();}

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    bool clean();

    int check_priority(str u) const;// If the user is logged, return its priority; else return -404;
    int query_user_priority(str u);
    int query_order_number(str u) const;
    bool add_order_number(str u);
};


#endif //TICKETSYSTEM_2021_USERMANAGER_H
