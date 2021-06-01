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
    vecS fail({"-1"});
    auto tp=train.FindByKey(i);
    if(!tp.second) return fail;
    auto &t=tp.first;
    if(d<t.date().first || t.date().second<d) return fail;
    return t.query_train(d,seat);
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
            if(train_t.check_date(d,s) && train_t.if_release() && train_t.check_sequence(s,t))
            {
                int value;
                if(If_time) value=train_t.get_time(s,t);
                else value=train_t.get_price(s,t);
                list.push_back({value,i});
            }
        }
    }
    if(list.empty()) return vecS({"0"});
    sort(list.begin(),list.end());

    vecS output;
    output.push_back(to_string(list.size()));
    for(int k=0; k<list.size(); ++k) output.push_back(trains2[list[k].second].information(s,t,d,seat));
    return output;
}

int TrainManager::write_log(int id,STATUS s,const string &u,const string &i,const string &f,const string &t,const RealTime &d,const RealTime &a,int p,int n)
{
    Log temp(id,s,u,i,f,t,d,a,p,n);
    auto main_key=u+to_string(id);
    log.insert(main_key,temp);
    log.AddTag(main_key,u);
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
    for(int i=temp.size()-1;i>=0;--i) output.push_back(temp[i].display());
    return output;
}

vecS TrainManager::query_transfer(const string &s,const string &t,Date d,bool If_time)
{
    auto trains1=train.FindByTag(s);
    auto trains2=train.FindByTag(t);

    bool If_find=false;
    int cost_minimal,train1_id,train2_id;
    string transfer_station;
    Date date_of_transfer;

    for(int i=0;i<trains1.size();++i)
    {
        auto &train1=trains1[i];
        if(!train1.if_release() || !train1.check_date(d,s)) continue;

        unordered_map<string,int> stations1;
        bool start_add=false;
        for(int k=0;k<trains1[i].station_number();++k)
        {
            if(start_add) stations1.insert({trains1[i].station_name(k),k});
            if(trains1[i].station_name(k)==s) start_add=true;
        }

        for(int j=0;j<trains2.size();++j)
        {
            auto &train2=trains2[j];
            if(!train2.if_release()) continue;
            for(int k=0;k<trains2[j].station_number();++k)
            {
                const string &st=trains2[j].station_name(k);
                if(st==t) break;
                if(stations1.count(st)!=0)
                {
                    auto temp=train1.check_if_later(train2,d,s,st);
                    if(temp.first<0) continue;

                    int cost=0;
                    if(!If_time) cost=train1.get_price(s,st)+train2.get_price(st,t);
                    else cost=train1.get_time(s,st)+temp.first+train2.get_time(st,t);

                    if(!If_find || cost<cost_minimal)
                    {
                        If_find=true;
                        cost_minimal=cost;
                        train1_id=i; train2_id=j;
                        transfer_station=st;
                        date_of_transfer=temp.second;
                    }
                }
            }
        }
    }
    vecS output;
    if(!If_find) output.push_back("0");
    else
    {
        output.push_back(trains1[train1_id].information(s,transfer_station,d,seat));
        output.push_back(trains2[train2_id].information(transfer_station,t,date_of_transfer,seat));
    }
    return output;
}

int TrainManager::buy_ticket(const string &i,Date d,const string &f,const string &t,int n,int id,const string &u,bool q)
{
    auto search=train.FindByKey(i);
    if(!search.second) return -404;
    auto &train_find=search.first;
    if(!train_find.if_release()) return -404;
    if(!train_find.check_date(d,f)) return -404;

    auto date=train_find.date_for_record(f,d);
    auto seat_remain=train_find.check_seat(f,t,date,seat);
    int total_price=train_find.get_price(f,t,n);
    auto time=train_find.obtain_time(f,t,date);

    if(i=="LeavesofGrass" && u=="Shaw" && n==20244) printf("[Debug]:remained seat=%d\n",seat_remain);

    if(seat_remain>=n)
    {
        train_find.decrease_seat(f,t,date,n,seat);
        write_log(id,SUCCESS,u,i,f,t,time.first,time.second,total_price/n,n);
        return total_price;
    }
    else
    {
        if(q) write_log(id,PENDING,u,i,f,t,time.first,time.second,total_price/n,n);
        return 0;
    }
}

pair<string,int> TrainManager::refund_ticket(const string &u,const int &n)
{
    pair<string,int> output("fail",-1);
    string main_key=u+to_string(n);
    auto temp_log=log.FindByKey(main_key);
    if(!temp_log.second) return output;
    auto &log1=temp_log.first;
    if(log1.status_now()==REFUNDED) return output;

    if(log1.status_now()==PENDING) output.second=0;
    else output.second=1;

    log1.modify_status(REFUNDED);
    log.Update(main_key,log1);
    auto t=train.FindByKey(log1.train()).first;
    output.first=t.train_id();
    if(output.second==0) return output;

    auto stations=log1.stations();
    auto date=log1.times().first.date();
    t.increase_seat(stations.first,stations.second,date,log1.number(),seat);
    train.Update(t.train_id(),t);// consider: This line is redundant.
    return output;
}

bool TrainManager::re_buy_ticket(const string &i,Date d,const string &f,const string &t,int n,int id,const string &u)
{
    auto train_find=train.FindByKey(i).first;
    auto seat_remain=train_find.check_seat(f,t,d,seat);
    if(seat_remain>=n)
    {
        auto date=train_find.date_for_record(f,d);
        train_find.decrease_seat(f,t,date,n,seat);
        update_log(u,id,SUCCESS);
        return true;
    }
    return false;
}











