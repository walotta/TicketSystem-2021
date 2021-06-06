//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_TRAINMANAGER_H
#define TICKETSYSTEM_2021_TRAINMANAGER_H

#include "Train.h"




class TrainManager
{
    class TrainStorage
    {
        BPlusTree<100,200> train_multi_index;
        BPlusTree<100,200> train_single_index;
        StoragePool<Train,bool,20> train_data;

    public:
        TrainStorage():train_multi_index("train_multi_index.dat"),train_single_index("train_single_index.dat"),train_data("train_data.dat"){}
        ~TrainStorage()=default;

        int get_id(str train_id)
        {
            vector<ex_index> temp;
            train_single_index.find(train_id,temp);
            if(temp.empty()) return -404;
            return temp[0].first;
        }
        int get_ids(str station_name,vector<ex_index> &out)
        {
            train_multi_index.find(station_name,out);
            if(out.empty()) return -404;
            return out.size();
        }
        Train get_train(const int &id) {return train_data.get(id);}
        void get_trains(str station_name,vector<Train> &out)
        {
            vector<ex_index> temp;
            train_multi_index.find(station_name,temp);
            for(int k=0;k<temp.size();++k)
            {
                out.push_back(train_data.get(temp[k].first));
            }
        }
        void update(const int &id,const Train &train) {train_data.update(id,train);}
        void release(const int &id,const Train &train)
        {
            auto key=hash_int(train.train_id());
            for(int i=0;i<train.station_number();++i)
            {
                train_multi_index.insert({train.station_name(i),key},id);//todo: get serial_number
            }
            train_data.update(id,train);
        }
        void add_train(str train_id,const Train &train)
        {
            int id=train_data.add(train);
            train_single_index.insert({train_id,id+10086},id);//todo: get serial_number
        }
        void delete_train(str train_id,const int &id)
        {
            train_single_index.remove({train_id,id+10086},id);
            train_data.remove(id);
        }
        void clean()
        {
            train_data.clearAll();
            train_single_index.clean();
            train_multi_index.clean();
        }
    };
    class SeatStorage
    {
        BPlusTree<100,200> seat_index;
        StoragePool<RemainedSeat,bool,20> seat_data;

        string get_key(str train_id,const Date &date) const
        {
            return train_id+date.display();
        }
    public:
        SeatStorage():seat_index("seat_index.dat"),seat_data("seat_data.dat"){}
        ~SeatStorage()=default;

        int get_id(str train_id,const Date &date)
        {
            vector<ex_index> temp;
            seat_index.find(get_key(train_id,date),temp);
            if(temp.empty()) return -404;
            return temp[0].first;
        }
        RemainedSeat get_seats(const int &id) {return seat_data.get(id);}
        void release(const Train &train)
        {
            string train_id(train.train_id());
            auto pos=train.date();
            for(Date date=pos.first; date<=pos.second; ++date)
            {
                string main_key(get_key(train_id,date));
                RemainedSeat seat(main_key,train.station_number(),train.seat_number());
                int id=seat_data.add(seat);
                seat_index.insert({main_key,id+10086},id);
            }
        }
        void update(const int &id,const RemainedSeat &seat) {seat_data.update(id,seat);}
        void clean()
        {
            seat_index.clean();
            seat_data.clearAll();
        }
    };
    class LogStorage
    {
        BPlusTree<100,200> log_index;
        StoragePool<Log,bool,20> log_data;

    public:
        LogStorage():log_index("log_index.dat"),log_data("log_data.dat"){}
        ~LogStorage()=default;

        int get_id(str username,const int &id)
        {
            vector<ex_index> temp;
            log_index.find(username,temp);
            if(temp.empty()) return -404;
            for(int i=0;i<temp.size();++i)
            {
                if(temp[i].second==id) return temp[i].first;
            }
            return -404;
        }
        void add_log(str username,const int &id,const Log &log)
        {
            int pos=log_data.add(log);
            log_index.insert({username,id},pos);
        }
        void update(const int &log_id,const Log &log) {log_data.update(log_id,log);}
        Log get_log(const int &log_id) {return log_data.get(log_id);}

        void get_logs(str username,vector<Log> &out)
        {
            vector<ex_index> temp;
            log_index.find(username,temp);
            for(int i=0;i<temp.size();++i)
                out.push_back(log_data.get(temp[i].first));
            sort(out.begin(),out.end());
            //consider: This place could be optimized
        }


        void clean()
        {
            log_index.clean();
            log_data.clearAll();
        }
    };

    TrainStorage trains;
    SeatStorage seats;
    LogStorage logs;


    bool update_log(str u,int id,Status s);
    void write_log(int id,Status s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,lint p,int n);
public:
    TrainManager()=default;
    ~TrainManager()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,const Time &x,vecI t,vecI o,const Date &d_beg,const Date &d_end,char y);
    bool release_train(str i);// Only after we release the train when we could check its seat number.
    void query_train(str i,Date date,vecS &out);
    bool delete_train(str i);
    void query_ticket(str s,str t,Date d,bool If_time,vecS &out);
    void query_transfer(str s,str t,Date d,bool If_time,vecS &out);
    void query_order(str u,vecS &out); // If query failed, return an empty vector.
    lint buy_ticket(str i,Date d,str f,str t,int n,int id,str u,bool q);// If but_ticket failed, return 0; If no such train, return -404;
    std::pair<string,int> refund_ticket(str u,const int &n,Date &date);
    bool clean();

    bool re_buy_ticket(str f,str t,int n,int id,str u,const Train &train1,RemainedSeat &seat);// If success, return true;
    Train get_train(str train_id);
    RemainedSeat get_seat(const int &seat_id);
    void update_seat(const int &seat_id,const RemainedSeat &seat);
    int get_seat_id(str i,const Date &date);
};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
