//
// Created by Fourest on 2021/5/10.
//

#include "TrainManager.h"

bool TrainManager::release_train(const string &i)
{
    auto temp=train.FindByKey(i);
    if(!temp.second) return false;
    if(!temp.first.release(seat)) return false;
    train.Update(i,temp.first);
    return true;
}

bool TrainManager::clean()
{
    train.clean();
    seat.clean();
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
    return true;
}

vecS TrainManager::query_train(const string &i,Date d)
{
    vecS fail;
    auto tp=train.FindByKey(i);
    if(!tp.second) return fail;
    if(d<tp.first.date().first || tp.first.date().second<d) return fail;
    return tp.first.query_train(d,seat);
}

vecS TrainManager::query_ticket(const string &s,const string &t,Date d,bool If_time)
{
    auto trains1=train.FindByTag(s);
    auto trains2=train.FindByTag(t);
    sort(trains1.begin(),trains1.end());
    sort(trains2.begin(),trains2.end());

    int i=0,j=0;
    vector<Train> list;
    while(i<trains1.size() && j<trains2.size())
    {
        if(trains1[i].train_id()==trains2[j].train_id())
        {
            if(trains1[i].check_date(d)) list.push_back(trains1[i]);
            ++i; ++j;
            continue;
        }
        if(trains1[i].train_id()<trains2[j].train_id()) ++i;
        else ++j;
    }
    /* todo: sort the vector by the order
     */
    vecS output;
    for(int k=0; k<list.size(); ++k) output.push_back(list[k].information(s,t,d));
    return output;
}
