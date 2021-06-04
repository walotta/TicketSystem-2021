//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_TRAINMANAGER_H
#define TICKETSYSTEM_2021_TRAINMANAGER_H

#include "Train.h"

#include "base.h"

class Train;
struct RemainedSeat;
class Log;

class RemainedSeat
{
    MyString trainI_Date; // This is the mainKey of this class.
    int seat_remained[102];
public:
    RemainedSeat()=default;
    explicit RemainedSeat(str main_key):trainI_Date(main_key){}
    explicit RemainedSeat(str main_key,const int &station_num,const int &seat_num):trainI_Date(main_key)
    {
        for(int i=0;i<=station_num;++i) seat_remained[i]=seat_num;
    }

    int min_seat(const int &start,const int &end) const
    {
        int output=seat_remained[start];
        for(int i=start;i<end;++i) output=min(output,seat_remained[i]);
        return output;
    }
    void de_seat(const int &start,const int &end,const int &n)
    {
        for(int i=start;i<end;++i) seat_remained[i]-=n;
    }
    int operator[](const int &index) const
    {
        return seat_remained[index];
    }
    void debug(int n) const
    {
        printf("[Debug]: In RemainedSeat[%s], ",string(trainI_Date).c_str());
        for(int i=0;i<n-1;++i) printf("%d ",seat_remained[i]);
        printf("\n");
    }
};

class Train
{
private:
    struct Station
    {
        MyString name;
        int price; // The "price" store the price that from the Departure station.
        RealTime arrival,departure;// We define the start_station's departure date is 06.01.

        Station()=default;

        void update(str n,int p,const RealTime &t1,const RealTime &t2)
        {
            name=n; price=p;
            arrival=t1; departure=t2;
        }
        void update(str n,int p) {name=n;price=p;}
    };

    Station station[102];
    MyString trainID;
    int stationNum;// Z,(1,100]
    int seatNum;// <1e5
    Time startTime;
    Date sale_beg,sale_end;
    bool If_release;
    char train_type;


public:
    pair<int,int> get_id(str i,str f) const
    {
        int start=0,end=0;
        for(int k=0;k<stationNum;++k)
        {
            if(station[k].name==i) start=k;
            if(station[k].name==f) {end=k; break;}
        }
        return {start,end};
    }
    int get_id(str s) const
    {
        for(int i=0;i<stationNum;++i)
        {
            if(station[i].name==s) return i;
        }
        return stationNum;
    }

    Train()=default;
    ~Train()=default;
    Train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y):trainID(i),startTime(x),sale_beg(d_beg),sale_end(d_end)
    {
        stationNum=n; seatNum=m;
        train_type=y; If_release=false;

        // Process the name & price & time;
        station[0].update(s[0],0);
        station[0].departure=RealTime(Date(6,1),x);
        for(int pos=0;pos<stationNum-2;++pos)
        {
            RealTime arrival(station[pos].departure+t[pos]);
            station[pos+1].update(s[pos+1],station[pos].price+p[pos],arrival,arrival+o[pos]);
        }
        int last=stationNum-1;
        station[last].update(s[last],station[last-1].price+p[last-1]);
        station[last].arrival=station[last-1].departure+t[last-1];
    }

    bool operator<(const Train &t) const
    {
        return trainID<t.trainID;
    }

    bool if_release() const {return If_release;}
    string train_id() const {return (string)trainID;}
    char type() const {return train_type;}
    pair<Date,Date> date() const {return {sale_beg,sale_end};}
    int station_number() const {return stationNum;}
    int station_price(const int &pos) const {return station[pos].price;}
    string station_name(const int &pos) const {return (string)station[pos].name;}
    void stations(vecS &out) const
    {
        for(int i=0;i<stationNum;++i) out.push_back((string)station[i].name);
    }
    RealTime station_departure(const int &pos) const {return station[pos].departure;}
    RealTime station_arrival(const int &pos) const {return station[pos].arrival;}
    // The "Date" in the parameter represents the date of the train's departure from the departure-station.
    Date departure_date(str station_name,const Date &d) const
    {
        int id=get_id(station_name);
        RealTime temp=station[id].departure+RealTime(d);
        return temp.date();
    }
    Time departure_time(str station_name) const
    {
        int id=get_id(station_name);
        return station[id].departure.time();
    }
    Date set_off_date(str station_name,const Date &d) const
    {
        int id=get_id(station_name);
        return d-station[id].departure.date().dayNum();
    }


    void query_train(vecS &out,const Date &date,RemainedSeat seat=RemainedSeat()) const
    {
        out.push_back((string)trainID+" "+train_type);
        for(int k=0; k<stationNum; ++k)
        {
            RealTime arrival(station[k].arrival+RealTime(date));
            RealTime departure(station[k].departure+RealTime(date));
            if(station[k].arrival.minutes<0) arrival.minutes=-404;
            if(station[k].departure.minutes<0) departure.minutes=-404;
            string temp((string)station[k].name);
            temp+=" "+arrival.display()+" -> "+departure.display();
            temp+=" "+to_string(station[k].price)+" ";
            if(k==stationNum-1) temp+="x";
            else
            {
                if(!If_release) temp+=to_string(seatNum);
                else temp+=to_string(seat[k]);
            }
            out.push_back(temp);
        }
    }
    string information(str i,str f,const Date &d,const RemainedSeat &seat) const
    {
        bool If_find_initial=false;
        int a=0,b=0,seat_num=seatNum;
        for(int j=0;j<stationNum;++j)
        {
            auto &st=station[j];
            if(i==st.name) {a=j; If_find_initial=true;}
            if(f==st.name) {b=j; break;}
            if(If_find_initial) seat_num=min(seat_num,seat[j]);
        }
        int price=station[b].price-station[a].price;
        int time_gap=station[b].arrival-station[a].departure;
        RealTime departure(d,station[a].departure.time()),arrival(departure+time_gap);
        string output((string)trainID+" "+i+" "+departure.display());
        output+=" -> "+f+" "+arrival.display()+" "+to_string(price)+" "+to_string(seat_num);
        return output;
    }
    bool check_date(const Date &d,str station_name) const
    {
        auto temp=set_off_date(station_name,d);
        if(temp<sale_beg || sale_end<temp) return false;
        return true;
    }
    bool check_sequence(str i,str f) const
    {
        int start=-404,end=-404;
        for(int k=0;k<stationNum;++k)
        {
            if(station[k].name==f) {end=k; break;}
            if(station[k].name==i) start=k;
        }
        if(start<0 || end<0 || start>=end) return false;
        return true;
    }
    lint get_price(str i,str f,int n=1) const
    {
        auto id=get_id(i,f);
        return (station[id.second].price-station[id.first].price)*lint(n);
    }// Only for comparing the cost.
    int get_time(str i,str f) const
    {
        auto id=get_id(i,f);
        return station[id.second].arrival-station[id.first].departure;
    } // Use for comparing the cost.

    // pair<time_cost_between_transfer,transfer_date>
    pair<RealTime,RealTime> obtain_time(str i,str f,const Date &d)
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;
        return {station[start].departure+RealTime(d),station[end].arrival+RealTime(d)};
    }
    pair<int,Date> check_if_later(const Train &train,const Date &start_date,str start_station,str transfer_station) const
    {
        auto ids=get_id(start_station,transfer_station);
        int date_gap=station[ids.second].arrival.date()-station[ids.first].departure.date();
        auto arrival_date=start_date+date_gap;
        auto arrival_time=station[ids.second].arrival.time();
        RealTime arrival(arrival_date,arrival_time);

        auto last_sale_date=train.departure_date(transfer_station,train.sale_end);
        auto d_time=train.departure_time(transfer_station);

        if(RealTime(last_sale_date,d_time)<=arrival) return {-1,Date()};
//        if(last_sale_date<arrival_date) return {-1,Date()};
//        if(arrival_date==last_sale_date && d_time<arrival_time) return {-1,Date()};

        auto first_sale_date=train.departure_date(transfer_station,train.sale_beg);

//        printf("[Debug]: In function (check_if_later), train1=%s, train2=%s, transfer_station=%s, first_sale_date=%s, last_sale_date=%s\n",train_id().c_str(),train.train_id().c_str(),transfer_station.c_str(),first_sale_date.display().c_str(),last_sale_date.display().c_str());
        for(auto day=first_sale_date; day<=last_sale_date; ++day)
        {
            RealTime temp(day,d_time);
            if(arrival<=temp) return {temp-arrival,day};
        }
        return {0,Date()};
    }

    int seat_number() const {return seatNum;}
    int check_seat(str i,str f,const Date &d,const RemainedSeat &seat) const
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;
        return seat.min_seat(start,end);
    }
    void decrease_seat(str i,str f,int n,RemainedSeat &seat) const
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;
        seat.de_seat(start,end,n);
    }
    void increase_seat(str i,str f,int n,RemainedSeat &seat)
    {
        decrease_seat(i,f,-n,seat);
    }
};

enum Status{SUCCESS=1,PENDING=0,REFUNDED=-1};
class Log
{
    // The main-key is "username+id"
    int id; // The main-key.
    Status status;
    MyString username;// The tag.
    MyString trainID;
    MyString From,To;
    RealTime departure,arrive;
    int price,num;
public:
    Log()=default;
    Log(int k,Status s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,int p,int n): username(u),trainID(i),From(f),To(t),departure(d),arrive(a),status(s)
    {
        price=p; num=n; id=k;
    }
    string display() const
    {
        string output("["+status_string()+"] "+(string)trainID+" "+(string)From+" ");
        output+=departure.display()+" -> "+(string)To+" "+arrive.display()+" ";
        output+=to_string(price)+" "+to_string(num);
        return output;
    }
    void modify_status(const Status &s) {status=s;}
    string main_key() const {return (string)username+to_string(id);}
    string tag() const {return (string)username;}
    string user() const {return (string)username;}
    Status status_now() const {return status;}
    string status_string() const
    {
        if(status==PENDING) return "pending";
        if(status==SUCCESS) return "success";
        return "refunded";
    }
    string train() const {return (string)trainID;}
    pair<string,string> stations() const {return {(string)From,(string)To};}
    pair<RealTime,RealTime> times() const {return {departure,arrive};}
    int number() const {return num;}
    bool operator<(const Log &log) const
    {
        return id<log.id;
    }
};




class TrainManager
{
    class TrainStorage
    {
        BPlusTree<300,300> train_multi_index;
        BPlusTree<300,300> train_single_index;
        StoragePool<Train,bool,300> train_data;

    public:
        TrainStorage():train_multi_index("train_multi_index.dat"),train_single_index("train_single_index.dat"),train_data("train_data.dat"){}
        ~TrainStorage()=default;

        int get_id(str train_id)
        {
            vector<index> temp;
            train_single_index.find(train_id,temp);
            if(temp.empty()) return -404;
            return temp[0].first;
        }
        int get_ids(str station_name,vector<index> &out)
        {
            train_multi_index.find(station_name,out);
            if(out.empty()) return -404;
            return out.size();
        }
        Train get_train(const int &id) {return train_data.get(id);}
        void get_trains(str station_name,vector<Train> out)
        {
            vector<index> temp;
            train_multi_index.find(station_name,temp);
            for(int k=0;k<temp.size();++k)
                out.push_back(train_data.get(temp[k].first));
        }
        void update(const int &id,const Train &train) {train_data.update(id,train);}
        void release(const int &id,const Train &train)
        {
            vecS stations;
            train.stations(stations);
            for(int i=0;i<stations.size();++i)
            {
                train_multi_index.insert({stations[i],id},id);//todo: get serial_number
            }
        }
        void add_train(str train_id,const Train &train)
        {
            int id=train_data.add(train);
            train_single_index.insert({train_id,id},id);//todo: get serial_number
        }
        void delete_train(str train_id)
        {
            vector<index> key;
            train_single_index.find(train_id,key);
            train_single_index.remove({train_id,key[0].second},key[0].first);
            train_data.remove(key[0].first);
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
        BPlusTree<300,300> seat_index;
        StoragePool<RemainedSeat,bool,300> seat_data;

        string get_key(str train_id,const Date &date) const
        {
            return train_id+" "+date.display();
        }
    public:
        SeatStorage():seat_index("seat_index.dat"),seat_data("seat_data.dat"){}
        ~SeatStorage()=default;

        int get_id(str train_id,const Date &date)
        {
            vector<index> temp;
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
                RemainedSeat seat(main_key);
                int id=seat_data.add(seat);
                seat_index.insert({main_key,id},id);//todo: get serial-number
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
        BPlusTree<300,300> log_index;
        StoragePool<Log,bool,300> log_data;

    public:
        LogStorage():log_index("log_index.dat"),log_data("log_data.dat"){}
        ~LogStorage()=default;

        int get_id(str username,const int &id)
        {
            vector<index> temp;
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
            vector<index> temp;
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

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);// Only after we release the train when we could check its seat number.
    void query_train(str i,Date date,vecS &out);
    bool delete_train(str i);
    void query_ticket(str s,str t,Date d,bool If_time,vecS &out);
    void query_transfer(str s,str t,Date d,bool If_time,vecS &out);
    void query_order(str u,vecS &out); // If query failed, return an empty vector.
    lint buy_ticket(str i,Date d,str f,str t,int n,int id,str u,bool q);// If but_ticket failed, return 0; If no such train, return -404;
    pair<string,int> refund_ticket(str u,const int &n,Date &date);
    bool clean();

    bool re_buy_ticket(str f,str t,int n,int id,str u,const Train &train1,RemainedSeat &seat);// If success, return true;
    Train get_train(str i);
    RemainedSeat get_seat(str i,const Date &date);
    void update_seat(str train_id,const Date &date,const RemainedSeat &seat);
};


#endif //TICKETSYSTEM_2021_TRAINMANAGER_H
