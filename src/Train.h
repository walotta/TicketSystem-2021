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
    RemainedSeat(str main_key):trainI_Date(main_key){}

    int min_seat(const int &start,const int &end) const
    {
        int output=seat_remained[start];
        for(int i=start;i<end;++i)
        {
            output=min(output,seat_remained[i]);
        }
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
            station[pos+1].update(s[pos+1],station[pos].price+p[pos],m,station[pos].departure+t[pos],station[pos+1].arrival+o[pos]);
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
    string station_name(const int &pos) const {return (string)station[pos].name;}
    RealTime station_departure(const int &pos) const {return station[pos].departure;}
    RealTime station_arrival(const int &pos) const {return station[pos].arrival;}
    // The "Date" in the parameter represents the date of the train's departure from the departure-station.
    Date departure_date(str station_name,const Date &d) const
    {
        for(int i=0;i<stationNum;++i)
        {
            if(station[i].name==station_name)
            {
                RealTime temp=station[i].departure+RealTime(d);
                return temp.date();
            }
        }
        return Date();
    }
    Date date_for_record(str station_name,const Date &d) const
    {
        for(int i=0;i<stationNum;++i)
        {
            if(station[i].name==station_name) return d-station[i].departure.date().dayNum();
        }
        return Date();
    }

    bool release(ST &store)
    {
        if(If_release) return false;
        If_release=true;
        for(auto i=sale_beg;i<sale_end;++i)
        {
            string main_key=(string)trainID+" "+i.display();
            RemainedSeat seat(main_key);
            for(int j=0;j<stationNum;++j) seat[j]=seatNum;
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
            string temp;
            temp+=(string)station[i].name;
            temp+=" "+station[i].arrival.display()+" -> "+station[i].departure.display();
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
        }
        int price=station[b].price-station[a].price;
        Time departure(station[a].departure.time()),arrival(station[b].arrival.time());
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
    int get_price(str i,str f,int n=1) const
    {
        int a=0,b=0;
        for(int j=0;j<stationNum;++j)
        {
            if(i==station[j].name) a=j;
            if(f==station[j].name) {b=j; break;}
        }
        return (station[b].price-station[a].price)*n;
    }// Only for comparing the cost.
    int get_time(str i,str f) const
    {
        int a=0,b=0;
        for(int j=0;j<stationNum;++j)
        {
            if(i==station[j].name) a=j;
            if(f==station[j].name)
            {
                b=j; break;
            }
        }
        return station[b].arrival-station[a].departure;
    } // Use for comparing the cost.

    // pair<time_cost,transfer_date>
    pair<int,Date> check_if_later(const Train &train,const Date &start_date,str start_station,str transfer_station) const
    {
        auto ids=get_id(start_station,transfer_station);
        int date_gap=station[ids.second].arrival.date()-station[ids.first].departure.date();
        auto arrival_date=start_date+date_gap;
        auto arrival_time=station[ids.second].arrival.time();
        RealTime arrival(arrival_date,arrival_time);



    }
    int check_seat(str i,str f,const Date &d,ST &store) const
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;

        string main_key(i+" "+d.display());
        auto seats=store.FindByKey(main_key).first;
        return seats.min_seat(start,end);
    }

    void decrease_seat(str i,str f,const Date &d,int n,ST &store)
    {
        auto id=get_id(i,f);
        int start=id.first,end=id.second;

        string main_key(i+" "+d.display());
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
        decrease_seat(i,f,date_for_record(i,d),-n,store);
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
