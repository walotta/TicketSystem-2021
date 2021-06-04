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
    auto temp=train.FindByKey(i);
    if(temp.second) return false; // If find the train already existed, return false;
    Train new_train(i,n,m,s,p,x,t,o,d_beg,d_end,y);
    train.insert(i,new_train);
    train.AddTag(i,s);
    return true;
}

bool TrainManager::delete_train(const string &i)
{
    auto temp=train.FindByKey(i);
    if(!temp.second) return false; // If find the train do not existed, return false;
    if(temp.first.if_release()) return false;
    static vector<string> stations;
    stations.clear();
    temp.first.stations(stations);
    train.RemoveTag(i,stations);
    train.Remove(i);
    return true;
}

void TrainManager::query_train(const string &i,Date d,vecS &out)
{
    vecS fail({"-1"});
    auto tp=train.FindByKey(i);
    if(!tp.second) {out=fail; return;}
    auto &t=tp.first;
    if(d<t.date().first || t.date().second<d) {out=fail; return;}
    t.query_train(d,seat,out);
}

void TrainManager::query_ticket(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    static vector<Train> trains1;
    static vector<Train> trains2;
    trains1.clear(); trains2.clear();
    train.FindByTag(s,trains1);
    train.FindByTag(t,trains2);

//    auto trains1=train.FindByTag(s);
//    auto trains2=train.FindByTag(t);

    unordered_set<string> station1;
    vector<pair<pair<int,string>,int>> list; // The first one is value, the second one is id.
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
                list.push_back({{value,train_t.train_id()},i});
            }
        }
    }
    if(list.empty()) {
        out.push_back("0");
        return;
    }
    sort(list.begin(),list.end());

    out.push_back(to_string(list.size()));
    for(int k=0; k<list.size(); ++k) out.push_back(trains2[list[k].second].information(s,t,d,seat));
}

int TrainManager::write_log(int id,Status s,const string &u,const string &i,const string &f,const string &t,const RealTime &d,const RealTime &a,lint p,int n)
{
    Log new_log(id,s,u,i,f,t,d,a,p,n);
    auto main_key=u+to_string(id);
    log.insert(main_key,new_log);
    log.AddTag(main_key,u);
    return 0;
}

bool TrainManager::update_log(const string &u,int id,Status s)
{
    string mainKey=u+to_string(id);
    auto temp=log.FindByKey(mainKey);
    if(!temp.second) return false;
    temp.first.modify_status(s);
    log.Update(mainKey,temp.first);
    return true;
}

void TrainManager::query_order(const string &u,vecS &out)
{
    static vector<Log> temp;
    temp.clear();
    log.FindByTag(u,temp);
    out.push_back(to_string(temp.size()));
    for(int i=temp.size()-1;i>=0;--i) out.push_back(temp[i].display());
}

void TrainManager::query_transfer(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    static vector<Train> trains1;
    static vector<Train> trains2;
    trains1.clear(); trains2.clear();
    train.FindByTag(s,trains1);
    train.FindByTag(t,trains2);

//    auto trains1=train.FindByTag(s);
//    auto trains2=train.FindByTag(t);

    bool If_find=false;
    int cost_minimal=-1,train1_id,train2_id,cost_time_1=-1;
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
            auto station_name=trains1[i].station_name(k);
            if(start_add) stations1.insert({station_name,k});
            if(station_name==s) start_add=true;
        }

        for(int j=0;j<trains2.size();++j)
        {
            auto &train2=trains2[j];
            if(!train2.if_release() || train1.train_id()==train2.train_id()) continue;
            for(int k=0;k<trains2[j].station_number();++k)
            {
                const string &st=trains2[j].station_name(k);
                if(st==t) break;
                if(stations1.count(st)!=0)
                {
                    auto temp=train1.check_if_later(train2,d,s,st);
                    if(temp.first<0) continue;

                    int cost=0,cost_time=train1.get_time(s,st);
                    if(!If_time) cost=train1.get_price(s,st)+train2.get_price(st,t);
                    else cost=train1.get_time(s,st)+temp.first+train2.get_time(st,t);

                    if(!If_find || cost<cost_minimal || (cost==cost_minimal && cost_time<cost_time_1))
                    {
                        If_find=true;
                        cost_minimal=cost; cost_time_1=cost_time;
                        train1_id=i; train2_id=j;
                        transfer_station=st;
                        date_of_transfer=temp.second;
                    }

                }
            }
        }
    }

    if(!If_find)
    {
        out.push_back("0");
        return;
    }
//    printf("[Debug]: In function (query_transfer), train1=%s, train2=%s, station1=%s, station2=%s, station_transfer=%s, date_of_transfer=%s\n",trains1[train1_id].train_id().c_str(),trains2[train2_id].train_id().c_str(),s.c_str(),t.c_str(),transfer_station.c_str(),date_of_transfer.display().c_str());
    out.push_back(trains1[train1_id].information(s,transfer_station,d,seat));
    out.push_back(trains2[train2_id].information(transfer_station,t,date_of_transfer,seat));
}

lint TrainManager::buy_ticket(const string &i,Date d,const string &f,const string &t,int n,int id,const string &u,bool q)
{
    auto search=train.FindByKey(i);
    if(!search.second) return -404;
    auto &train1=search.first;
    if(!train1.if_release()) return -404;
    if(!train1.check_date(d,f)) return -404;
    if(!train1.check_sequence(f,t)) return -404;
    if(train1.seat_number()<n) return -404;

    auto date=train1.date_for_record(f,d);
    auto seat_remain=train1.check_seat(f,t,date,seat);
    lint total_price=train1.get_price(f,t,n);
    auto time=train1.obtain_time(f,t,date);


    if(seat_remain>=n)
    {
        train1.decrease_seat(f,t,date,n,seat);
        write_log(id,SUCCESS,u,i,f,t,time.first,time.second,total_price/n,n);
        return total_price;
    }
    else
    {
        if(q)
        {
            write_log(id,PENDING,u,i,f,t,time.first,time.second,total_price/n,n);
            return 0;
        }
        return -404;
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

    output={log1.train(),log1.status_now()};
    log1.modify_status(REFUNDED);
    log.Update(main_key,log1);
    if(output.second==0) return output;

    auto train1=train.FindByKey(output.first).first;
    auto stations=log1.stations();
    auto day=log1.times().first.date();

    train1.increase_seat(stations.first,stations.second,day,log1.number(),seat);
//    train.Update(output.first,train1);// consider: This line is redundant.
    return output;
}

bool TrainManager::re_buy_ticket(const string &i,Date d,const string &f,const string &t,int n,int id,const string &u)
{
    auto train_find=train.FindByKey(i).first;
    auto date=train_find.date_for_record(f,d);
    auto seat_remain=train_find.check_seat(f,t,date,seat);
    if(seat_remain>=n)
    {
        train_find.decrease_seat(f,t,date,n,seat);
        update_log(u,id,SUCCESS);
        return true;
    }
    return false;
}











