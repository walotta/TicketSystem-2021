//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_TRAINMANAGER_H
#define TICKETSYSTEM_2021_TRAINMANAGER_H

#include "Train.h"

class TrainManager
{
    TR train;
    ST seat;
    LG log;


    bool update_log(str u,int id,Status s);
    int write_log(int id,Status s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,lint p,int n);
public:
    TrainManager():train("train.dat",true),seat("seat.dat",false),log("log.dat",true){}
    ~TrainManager()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);// Only after we release the train when we could check its seat number.
    void query_train(str i,Date d,vecS &out);
    bool delete_train(str i);
    void query_ticket(str s,str t,Date d,bool If_time,vecS &out);
    void query_transfer(str s,str t,Date d,bool If_time,vecS &out);
    void query_order(str u,vecS &out); // If query failed, return an empty vector.
    lint buy_ticket(str i,Date d,str f,str t,int n,int id,str u,bool q);// If but_ticket failed, return 0; If no such train, return -404;
    pair<string,int> refund_ticket(str u,const int &n);
    bool clean();

    bool re_buy_ticket(str i,Date d,str f,str t,int n,int id,str u);// If success, return true;
};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
