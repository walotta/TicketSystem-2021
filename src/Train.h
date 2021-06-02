//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_TRAIN_H
#define TICKETSYSTEM_2021_TRAIN_H

#include "base.h"

class Train;
struct RemainedSeat;
class Log;
typedef StorageManger<Train,300,300,300,300,300> TR;
typedef StorageManger<RemainedSeat,300,300,300,300,300> ST;
typedef StorageManger<Log,300,300,300,300,300> LG;

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
    int &operator[](const int &index)
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
    struct Station
    {
        MyString name;
        int price; // The "price" store the price that from the Departure station.
        RealTime arrival,departure;// We define the start_station's departure date is 06.01.

        Station()=default;

        void update(str n,int p,int num,const RealTime &t1,const RealTime &t2)
        {
            name=n;
            price=p;
//            NUM=num;
            arrival=t1;
            departure=t2;
//            for(int i=0;i<100;++i) seat_remain[i]=num;
        }
        void update(str n,int p,int num)
        {
            name=n;
            price=p;
//            NUM=num;
//            for(int i=0;i<100;++i) seat_remain[i]=num;
        }
    };

    Station station[102];
    MyString trainID;
    int stationNum;// Z,(1,100]
    int seatNum;// <1e5
    Time startTime;
    Date sale_beg,sale_end;
    bool If_release;
    char type;


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
public:
    Train()=default;
    ~Train()=default;
    Train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y):trainID(i),startTime(x),sale_beg(d_beg),sale_end(d_end)
    {
        stationNum=n;
        seatNum=m;
        type=y;
        If_release=false;

        // Process the name & price & time;
        station[0].update(s[0],0,m);
        station[0].departure=RealTime(Date(6,1),x);
        for(int pos=0;pos<stationNum-2;++pos)
        {
            RealTime arrival(station[pos].departure+t[pos]);
            station[pos+1].update(s[pos+1],station[pos].price+p[pos],m,arrival,arrival+o[pos]);
            /*
            station[pos+1].name=s[pos+1];
            station[pos+1].price=station[pos].price+p[pos];
            station[pos+1].arrival=station[pos].departure+t[pos];
            station[pos+1].departure=station[pos+1].arrival+o[pos];
             */
        }
        int last=stationNum-1;
        station[last].update(s[last],station[last-1].price+p[last-1],m);
        station[last].arrival=station[last-1].departure+t[last-1];
    }

    bool operator<(const Train &t) const
    {
        return trainID<t.trainID;
    }

    bool if_release() const { return If_release; }
    string train_id() const {return (string)trainID;}
    pair<Date,Date> date() const {return {sale_beg,sale_end};}
    int station_number() const {return stationNum;}
    int seat_number() const {return seatNum;}
    string station_name(const int &pos) const {return (string)station[pos].name;}
    RealTime station_departure(const int &pos) const {return station[pos].departure;}
    RealTime station_arrival(const int &pos) const {return station[pos].arrival;}
    // The "Date" in the parameter represents the date of the train's departure from the departure-station.
    Date departure_date(str station_name,const Date &d) const
    {
        int id=get_id(station_name);
        RealTime temp=station[id].departure+RealTime(d);
        return temp.date();
    }
    Date arrival_date(str station_name,const Date &d) const
    {
        int id=get_id(station_name);
        RealTime temp=station[id].arrival+RealTime(d);
        return temp.date();
    }
    Time departure_time(str station_name) const
    {
        int id=get_id(station_name);
        return station[id].departure.time();
    }
    Date date_for_record(str station_name,const Date &d) const
    {
        int id=get_id(station_name);
        return d-station[id].departure.date().dayNum();
    }

    bool release(ST &store)
    {
        if(If_release) return false;
        If_release=true;
        for(auto i=sale_beg;i<=sale_end;++i)
        {
            string main_key=(string)trainID+" "+i.display();
            RemainedSeat seat(main_key,stationNum,seatNum);
            store.insert(main_key,seat);
        }
        return true;
    }
    vecS query_train(const Date &d,ST &store) const
    {
        vecS output;
        output.push_back((string)trainID+" "+type);
        auto seat=store.FindByKey((string)trainID+" "+d.display()).first;
        for(int i=0;i<stationNum;++i)
        {
            RealTime arrival(station[i].arrival+RealTime(d));
            RealTime departure(station[i].departure+RealTime(d));
            if(station[i].arrival.minutes<0) arrival.minutes=-404;
            if(station[i].departure.minutes<0) departure.minutes=-404;
            string temp((string)station[i].name);
            temp+=" "+arrival.display()+" -> "+departure.display();
            temp+=" "+to_string(station[i].price)+" ";
            if(i==stationNum-1) temp+="x";
            else
            {
                if(!If_release) temp+=to_string(seatNum);
                else temp+=to_string(seat[i]);
            }
            output.push_back(temp);
        }
        return output;
    }
    string information(str i,str f,const Date &d,ST &store) const
    {
        bool If_find_initial=false;
        int a=0,b=0,seat=seatNum;
        auto seats=store.FindByKey((string)trainID+" "+date_for_record(i,d).display()).first;
        for(int j=0;j<stationNum;++j)
        {
            auto &st=station[j];
            if(i==st.name) {a=j; If_find_initial=true;}
            if(f==st.name) {b=j; break;}
            if(If_find_initial) seat=min(seat,seats[j]);
            //consider: Would un-released train use this function?
            //          This part could be optimized.
        }
        int price=station[b].price-station[a].price;
        int time_gap=station[b].arrival-station[a].departure;
        RealTime departure(d,station[a].departure.time()),arrival(departure+time_gap);
        string output((string)trainID+" "+i+" "+departure.display());
        output+=" -> "+f+" "+arrival.display()+" "+to_string(price)+" "+to_string(seat);
        return output;
    }
    bool check_date(const Date &d,str station_name) const
    {
        auto temp=date_for_record(station_name,d);
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
    int get_price(str i,str f,int n=1) const
    {
        auto id=get_id(i,f);
        return (station[id.second].price-station[id.first].price)*n;
    }// Only for comparing the cost.
    int get_time(str i,str f) const
    {
        auto id=get_id(i,f);
        return station[id.second].arrival-station[id.first].departure;
    } // Use for comparing the cost.

    // pair<time_cost_between_transfer,transfer_date>
    pair<int,Date> check_if_later(const Train &train,const Date &start_date,str start_station,str transfer_station) const
    {
        auto ids=get_id(start_station,transfer_station);
        int date_gap=station[ids.second].arrival.date()-station[ids.first].departure.date();
        auto arrival_date_=start_date+date_gap;
        auto arrival_time=station[ids.second].arrival.time();
        RealTime arrival(arrival_date_,arrival_time);

        auto last_sale_date=train.departure_date(transfer_station,train.sale_end);
        auto d_time=train.departure_time(transfer_station);

        if(last_sale_date<arrival_date_) return {-1,Date()};
        if(arrival_date_==last_sale_date && d_time<arrival_time) return {-1,Date()};

        auto first_sale_date=train.departure_date(transfer_station,train.sale_beg);

//        printf("[Debug]: In function (check_if_later), train1=%s, train2=%s, transfer_station=%s, first_sale_date=%s, last_sale_date=%s\n",train_id().c_str(),train.train_id().c_str(),transfer_station.c_str(),first_sale_date.display().c_str(),last_sale_date.display().c_str());
        for(auto day=first_sale_date; day<=last_sale_date; ++day)
        {
            RealTime temp(day,d_time);
            if(arrival<temp) return {temp-arrival,day};
        }
        return {0,Date()};
    }
    int check_seat(str i,str f,const Date &d,ST &store) const
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;

        string main_key((string)trainID+" "+d.display());
        auto seats=store.FindByKey(main_key).first;
        return seats.min_seat(start,end);
    }

    void decrease_seat(str i,str f,const Date &d,int n,ST &store)
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;

        string main_key((string)trainID+" "+d.display());
        auto seats=store.FindByKey(main_key).first;
        seats.de_seat(start,end,n);
        store.Update(main_key,seats);
    }
    pair<RealTime,RealTime> obtain_time(str i,str f,const Date &d)
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;
        return {station[start].departure+RealTime(d),station[end].arrival+RealTime(d)};
    }
    void increase_seat(str i,str f,const Date &d,int n,ST &store)
    {
        auto date=date_for_record(i,d);
        decrease_seat(i,f,date,-n,store);
    }
};

enum STATUS{SUCCESS=1,PENDING=0,REFUNDED=-1};
class Log
{
    int id; // The main-key.
    STATUS status;
    MyString username;// The tag.
    MyString trainID;
    MyString From,To;
    RealTime departure,arrive;
    int price,num;
public:
    Log()=default;
    Log(int k,STATUS s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,int p,int n):username(u),trainID(i),From(f),To(t),departure(d),arrive(a),status(s)
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
    void modify_status(const STATUS &s) {status=s;}
    string main_key() const {return (string)username+to_string(id);}
    string tag() const {return (string)username;}
    string user() const {return (string)username;}
    STATUS status_now() const {return status;}
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
};



#endif //TICKETSYSTEM_2021_TRAIN_H
