//
// Created by Fourest on 2021/5/10.
//

#include "TrainManager.h"

bool TrainManager::release_train(const string &i)
{
    int id=trains.get_id(i);
    if(id<0) return false;
    Train train(trains.get_train(id));
    if(train.if_release()) return false;
    train.release();
    trains.release(id,train);
    seats.release(train);
    return true;
}

bool TrainManager::clean()
{
    trains.clean();
    seats.clean();
    logs.clean();
    return true;
}

bool TrainManager::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,const Time &x,const vector<int> &t,const vector<int> &o,const Date &d_beg,const Date &d_end,char y)
{
    int id=trains.get_id(i);
    if(id>=0) return false; // If find the train already existed, return false;
    Train new_train(i,n,m,s,p,x,t,o,d_beg,d_end,y);
    trains.add_train(i,new_train);
    return true;
}

bool TrainManager::delete_train(const string &i)
{
    int id=trains.get_id(i);
    if(id<0) return false; // If find the train do not existed, return false;
    Train train(trains.get_train(id));
    if(train.if_release()) return false;
    trains.delete_train(i,id);
    return true;
}

void TrainManager::query_train(const string &i,Date date,vecS &out)
{
    int id=trains.get_id(i);
    if(id<0) {out.push_back("-1"); return;}
    Train train(trains.get_train(id));
    auto day=train.date();
    if(date<day.first || day.second<date) {out.push_back("-1"); return;}

    if(train.if_release())
    {
       int seat_id=seats.get_id(i,date);
       RemainedSeat seat(seats.get_seats(seat_id));
       train.query_train(out,date,seat);
    }
    else train.query_train(out,date);
}

void TrainManager::query_ticket(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    static vector<ex_index> trains1;
    static vector<ex_index> trains2;
    trains1.clear(); trains2.clear();
    trains.get_ids(s,trains1);
    trains.get_ids(t,trains2);
    HashMap<unsigned long long,int> station1;
    vector<std::pair<std::pair<int,string>,int>> list; // The first one is value, the second one is id.
    for(int i=0;i<trains1.size();++i) station1.insert(trains1[i].second,1);
    for(int i=0;i<trains2.size();++i)
    {
        auto &key=trains2[i];
        if(station1.count(key.second)!=0)
        {
            Train train(trains.get_train(key.first));
            auto pos=train.get_id(s,t);
            if(train.check_date(d,s) && train.if_release() && train.check_sequence(pos))
            {
                int value;
                if(If_time) value=train.get_time(pos);
                else value=train.get_price(pos);
                list.push_back({{value,train.train_id()},i});
            }
        }
    }
    if(list.empty()) {out.push_back("0"); return;}
    sort(list.begin(),list.end());

    out.push_back(std::to_string(list.size()));
    for(int k=0; k<list.size(); ++k)
    {
        int id=trains2[list[k].second].first;
        Train train(trains.get_train(id));
        Date date=train.set_off_date(s,d);
        int seat_id=seats.get_id(train.train_id(),date);
        RemainedSeat seat(seats.get_seats(seat_id));
        out.push_back(train.information(s,t,d,seat));
    }
}

void TrainManager::write_log(int id,Status s,const string &u,const string &i,const string &f,const string &t,const RealTime &d,const RealTime &a,lint p,int n)
{
    Log new_log(id,s,u,i,f,t,d,a,p,n);
    logs.add_log(u,id,new_log);
}

bool TrainManager::update_log(const string &u,int id,Status s)
{
    int log_id=logs.get_id(u,id);
    if(log_id<0) return false;
    Log log(logs.get_log(log_id));
    log.modify_status(s);
    logs.update(log_id,log);
    return true;
}

void TrainManager::query_order(const string &u,vecS &out)
{
    vector<Log> temp;
    logs.get_logs(u,temp);
    out.push_back(std::to_string(temp.size()));
    for(int i=temp.size()-1;i>=0;--i) out.push_back(temp[i].display());
}

void TrainManager::query_transfer(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    static vector<ex_index> trains1;
    static vector<ex_index> trains2;
    trains1.clear(); trains2.clear();
    trains.get_ids(s,trains1);
    trains.get_ids(t,trains2);

    bool If_find=false;
    int cost_minimal=-1,train1_id,train2_id,cost_time_1=-1;
    string transfer_station;
    Date date_of_transfer;
    auto hash_s=hash_int(s),hash_t=hash_int(t);

    for(int i=0;i<trains1.size();++i)
    {
        Train train1(trains.get_train(trains1[i].first));
        auto hash_t1=hash_int(train1.train_id());
        if(!train1.if_release() || !train1.check_date(d,s)) continue;

        HashMap<unsigned long long,int> stations1;
        bool start_add=false;
        for(int k=0;k<train1.station_number();++k)
        {
            auto hash_name=hash_int(train1.station_name(k));
            if(start_add) stations1.insert(hash_name,k);
            if(hash_name==hash_s) start_add=true;
        }

        for(int j=0;j<trains2.size();++j)
        {
            Train train2(trains.get_train(trains2[j].first));
            auto hash_t2=hash_int(train2.train_id());
            if(!train2.if_release() || hash_t1==hash_t2) continue;
            for(int k=0;k<train2.station_number();++k)
            {
                const string &st=train2.station_name(k);
                auto hash_st=hash_int(st);
                if(hash_st==hash_t) break;
                if(stations1.count(hash_st)!=0)
                {
                    auto temp=train1.check_if_later(train2,d,s,st);
                    if(temp.first<0) continue;
                    auto pos1=train1.get_id(s,st);
                    auto pos2=train2.get_id(st,t);

                    int cost=0,cost_time=train1.get_time(pos1);
                    if(!If_time) cost=train1.get_price(pos1)+train2.get_price(pos2);
                    else cost=cost_time+temp.first+train2.get_time(pos2);

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

    if(!If_find) {out.push_back("0"); return;}

    Train train1(trains.get_train(trains1[train1_id].first));
    Date date1=train1.set_off_date(s,d);
    int seat_id1=seats.get_id(train1.train_id(),date1);
    RemainedSeat seat1(seats.get_seats(seat_id1));
    out.push_back(train1.information(s,transfer_station,d,seat1));

    Train train2(trains.get_train(trains2[train2_id].first));
    Date date2=train2.set_off_date(transfer_station,date_of_transfer);
    int seat_id2=seats.get_id(train2.train_id(),date2);
    RemainedSeat seat2(seats.get_seats(seat_id2));
    out.push_back(train2.information(transfer_station,t,date_of_transfer,seat2));
}

lint TrainManager::buy_ticket(const string &i,Date d,const string &f,const string &t,int n,int id,const string &u,bool q)
{
    int train_id=trains.get_id(i);
    if(train_id<0) return -404;
    Train train(trains.get_train(train_id));
    auto station_pos=train.get_id(f,t);
    if(!train.if_release()) return -404;
    if(!train.check_date(d,f)) return -404;
    if(!train.check_sequence(station_pos)) return -404;
    if(train.seat_number()<n) return -404;

    auto date=train.set_off_date(f,d);

    int seat_id=seats.get_id(i,date);
    RemainedSeat seat(seats.get_seats(seat_id));
    auto seat_remain=train.check_seat(station_pos,seat);
    lint total_price=train.get_price(station_pos,n);
    auto time=train.obtain_time(station_pos,date);

    if(seat_remain>=n)
    {
        train.decrease_seat(station_pos,n,seat);
        seats.update(seat_id,seat);
        write_log(id,SUCCESS,u,i,f,t,time.first,time.second,total_price/n,n);
        return total_price;
    }
    else
    {
        if(!q) return -404;
        write_log(id,PENDING,u,i,f,t,time.first,time.second,total_price/n,n);
        return 0;
    }
}

std::pair<string,int> TrainManager::refund_ticket(const string &u,const int &n,Date &date)
{
    std::pair<string,int> output("fail",-1);

    int log_id=logs.get_id(u,n);
    if(log_id<0) return output;
    Log log(logs.get_log(log_id));
    if(log.status_now()==REFUNDED) return output;

    output={log.train(),log.status_now()};
    log.modify_status(REFUNDED);
    logs.update(log_id,log);
    if(output.second==0) return output;

    int id=trains.get_id(output.first);
    Train train(trains.get_train(id));
    auto stations=log.stations();
    auto station_id=train.get_id(stations.first,stations.second);
    auto day=log.times().first.date();
    date=train.set_off_date(stations.first,day);

    int seat_id=seats.get_id(train.train_id(),date);
    RemainedSeat seat(seats.get_seats(seat_id));
    train.increase_seat(station_id,log.number(),seat);
    seats.update(seat_id,seat);
    return output;
}

bool TrainManager::re_buy_ticket(const string &f,const string &t,int n,int id,const string &u,const Train &train,RemainedSeat &seat)
{
    auto ids=train.get_id(f,t);
    int seat_remain=seat.min_seat(ids.first,ids.second);
    if(seat_remain>=n)
    {
        seat.de_seat(ids.first,ids.second,n);
        update_log(u,id,SUCCESS);
        return true;
    }
    return false;
}

Train TrainManager::get_train(const string &train_id)
{
    return trains.get_train(trains.get_id(train_id));
}

RemainedSeat TrainManager::get_seat(const int &seat_id)
{
    return seats.get_seats(seat_id);
}

void TrainManager::update_seat(const int &seat_id,const RemainedSeat &seat)
{
    seats.update(seat_id,seat);
}

int TrainManager::get_seat_id(str i,const Date &date)
{
    return seats.get_id(i,date);
}











