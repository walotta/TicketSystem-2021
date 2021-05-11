//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_TRAINMANAGER_H
#define TICKETSYSTEM_2021_TRAINMANAGER_H

#include "Train.h"

class TrainManager
{
    vector<Train> train;

public:
    TrainManager()=default;
    ~TrainManager()=default;
    bool add_train();//todo
    bool release_train();//todo
    void query_train();//todo
    void delete_train();//todo
    void query_ticket();//todo
    void query_transfer();//todo
    void refund_ticket();//todo
    void clean();//todo


};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
