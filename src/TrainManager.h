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

    int write_log(int id,STATUS s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,int p,int n);
    bool update_log(str u,int id,STATUS s);
    //todo: To process these two private function.
public:
    TrainManager():train("train.dat",true),seat("seat.dat",false),log("log.dat",false){}
    ~TrainManager()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);// Only after we release the train when we could check its seat number.
    vecS query_train(str i,Date d);
    bool delete_train(str i);
    vecS query_ticket(str s,str t,Date d,bool If_time);
    vecS query_transfer(str s,str t,Date d,bool If_time);
    vecS query_order(str u); // If query failed, return an empty vector.
    int  buy_ticket(str i,Date d,str f,str t,int n,int id,str u);
    bool refund_ticket();//todo: Add the parameter.
    bool clean();


};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
