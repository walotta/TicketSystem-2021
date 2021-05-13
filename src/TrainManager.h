//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_TRAINMANAGER_H
#define TICKETSYSTEM_2021_TRAINMANAGER_H

#include "Train.h"

class TrainManager
{
    FileStorage<Train> train;

public:
    TrainManager()=default;
    ~TrainManager()=default;
    TrainManager(str address);

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);
    vecS query_train(str i,Date d);
    bool delete_train(str i);
    vecS query_ticket(str s,str t,Date d,str p);
    vecS query_transfer(str s,str t,Date d,str p);
    int  buy_ticket(str i,Date d,str f,str t,int n);
    bool refund_ticket();//todo
    bool clean();//todo


};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
