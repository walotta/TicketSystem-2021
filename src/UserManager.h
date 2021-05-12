//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USERMANAGER_H
#define TICKETSYSTEM_2021_USERMANAGER_H

#include "User.h"
#include "TrainManager.h"

class UserManager
{
    FileStorage<User> user;

public:
    UserManager()=default;
    ~UserManager()=default;
    UserManager(str address);

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u);// todo
    vector<string> query_order(str u); // If query failed, return an empty vector.
    bool refund_ticket(str u);// todo
    void clean();



};


#endif //TICKETSYSTEM_2021_USERMANAGER_H
