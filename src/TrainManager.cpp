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
    log.clean();
    return true;
}

bool TrainManager::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,Time x,const vector<int> &t,const vector<int> &o,Date d_beg,Date d_end,char y)
{
    auto tp=train.FindByKey(i);
    if(tp.second) return false;
    Train temp(i,n,m,s,p,x,t,o,d_beg,d_end,y);
    train.insert(i,temp);
    train.AddTag(i,s);
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

    unordered_set<string> station1;
    vector<pair<int,int>> list; // The first one is value, the second one is id.
    for(int i=0;i<trains1.size();++i) station1.insert(trains1[i].train_id());
    for(int i=0;i<trains2.size();++i)
    {
        auto &train_t=trains2[i];
        if(station1.count(train_t.train_id())!=0)
        {
            if(train_t.check_date(d) && train_t.if_release())
            {
                int value;
                if(If_time) value=train_t.get_time(s,t);
                else value=train_t.get_price(s,t);
                list.push_back({value,i});
            }
        }
    }
    sort(list.begin(),list.end());

    vecS output;
    if(!list.empty()) output.push_back(to_string(list.size()));
    else output.push_back("-1");
    for(int k=0; k<list.size(); ++k) output.push_back(trains2[list[k].second].information(s,t,d,seat));
    return output;
}

int TrainManager::write_log(int id,STATUS s,const string &u,const string &i,const string &f,const string &t,const RealTime &d,const RealTime &a,int p,int n)
{
    Log temp(id,s,u,i,f,t,d,a,p,n);
    log.insert(u+to_string(id),temp);
    return 0;
}

bool TrainManager::update_log(const string &u,int id,STATUS s)
{
    string mainKey=u+to_string(id);
    auto temp=log.FindByKey(mainKey);
    if(!temp.second) return false;
    temp.first.modify_status(s);
    log.Update(mainKey,temp.first);
    return true;
}

vecS TrainManager::query_order(const string &u)
{
    auto temp=log.FindByTag(u);
    vecS output;
    output.push_back(to_string(temp.size()));
    for(int i=0;i<temp.size();++i) output.push_back(temp[i].display());
    return output;
}

vecS TrainManager::query_transfer(const string &s,const string &t,Date d,bool If_time)
{
    auto trains1=train.FindByTag(s);
    auto trains2=train.FindByTag(t);


    bool If_find=false;
    int cost_minimal;
    string train1_name,train2_name;

    for(int i=0;i<trains1.size();++i)
    {
        unordered_map<string,int> station1;
        for(int k=0;k<trains1[i].station_number();++k) station1.insert({trains1[i].station_name(k),k});
        for(int j=0;j<trains2.size();++j)
        {
            for(int k=0;k<trains2[j].station_number();++k)
            {
                if(station1.count(trains2[j].station_name(k))!=0)
                {
                    int cost=0;
                    //todo: Process after select
                    if(!If_find)
                    {
                        If_find=true;
                    }
                }
            }


        }

    }


    if(!If_find)
    {
        vecS temp;
        temp.push_back("0");
        return temp;
    }

    return vecS();
}

