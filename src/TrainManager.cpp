//
// Created by Fourest on 2021/5/10.
//

#include "TrainManager.h"

bool TrainManager::release_train(const string &i)
{
    auto temp=train.FindByKey(i);
    if(!temp.second) return false;
    if(!temp.first.release()) return false;

    train.Update(i,temp.first);
    return true;
}

bool TrainManager::clean()
{
    train.clean();
    return true;
}

bool TrainManager::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,Time x,const vector<int> &t,const vector<int> &o,Date d_beg,Date d_end,char y)
{
    auto tp=train.FindByKey(i);
    if(tp.second) return false;
    Train temp(i,n,m,s,p,x,t,o,d_beg,d_end,y);
    train.insert(i,temp);
    return true;
}

bool TrainManager::delete_train(const string &i)
{
    auto temp=train.FindByKey(i);
    if(!temp.second) return false;
    if(temp.first.if_release()) return false;

    train.Remove(i);
    //todo: Remove tag.
    return true;
}

vecS TrainManager::query_train(const string &i,Date d)
{
    vecS fail;
    auto tp=train.FindByKey(i);
    if(!tp.second) return fail;
    if(d<tp.first.date().first || tp.first.date().second<d) return fail;
    return tp.first.query_train();
}
