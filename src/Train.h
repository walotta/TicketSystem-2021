//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_TRAIN_H
#define TICKETSYSTEM_2021_TRAIN_H

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
    bool temp(const string &o)const{return o==trainI_Date;}
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
    inline int get_id(str s) const
    {
        for(int i=0;i<stationNum;++i)
        {
            if(station[i].name==s) return i;
        }
        return stationNum;
    }

    Train()=default;
    ~Train()=default;
    Train(str i,int n,int m,const vecS &s,vecI p,const Time &x,vecI t,vecI o,const Date &d_beg,const Date &d_end,char y):trainID(i),startTime(x),sale_beg(d_beg),sale_end(d_end)
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
    void release() {If_release=true;}
    string train_id() const {return (string)trainID;}
    pair<Date,Date> date() const {return {sale_beg,sale_end};}
    int station_number() const {return stationNum;}
    string station_name(const int &pos) const {return (string)station[pos].name;}
    void stations(vecS &out) const
    {
        for(int i=0;i<stationNum;++i) out.push_back((string)station[i].name);
    }
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
    inline Date set_off_date(str station_name,const Date &d) const
    {
        return d-station[get_id(station_name)].departure.date().dayNum();
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
        int start=0,end=0,seat_num=seatNum;
        for(int j=0;j<stationNum;++j)
        {
            auto &st=station[j];
            if(i==st.name) {start=j; If_find_initial=true;}
            if(f==st.name) {end=j; break;}
            if(If_find_initial) seat_num=min(seat_num,seat[j]);

        }
        int price=station[end].price-station[start].price;
        int time_gap=station[end].arrival-station[start].departure;
        RealTime departure(d,station[start].departure.time()),arrival(departure+time_gap);
        string output((string)trainID+" "+i+" "+departure.display());
        output+=" -> "+f+" "+arrival.display()+" "+to_string(price)+" "+to_string(seat_num);
        return output;
    }
    inline bool check_date(const Date &d,str station_name) const
    {
        Date temp=set_off_date(station_name,d);
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
    void increase_seat(str i,str f,int n,RemainedSeat &seat) {decrease_seat(i,f,-n,seat);}
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
    int serial_number() const {return id;}
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
    bool operator<(const Log &log) const {return id<log.id;}
};



#endif //TICKETSYSTEM_2021_TRAIN_H
