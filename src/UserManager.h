//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USERMANAGER_H
#define TICKETSYSTEM_2021_USERMANAGER_H

#include "User.h"
#include "TrainManager.h"

class UserManager
{
    StorageManger<User,300,300,300,300,300> user;
    StorageManger<Log,300,300,300,300,300> log;
    map<string,int> loggedUser;

public:
    UserManager():user("user.dat",false),log("log.dat",false){}
    ~UserManager()=default;
//    UserManager(const string &address);

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    vecS query_order(str u); // If query failed, return an empty vector.
    string buy_ticket(str u,str i,Date d,str f,str t,int n,bool q);
    bool refund_ticket(str u,int n);// todo
    void clean();



};


#endif //TICKETSYSTEM_2021_USERMANAGER_H
