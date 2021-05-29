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
    MyString trainIDate; // This is the mainKey of this class.
    int seat_remained[102];
public:
    RemainedSeat(str mainKey):trainIDate(mainKey){}

    int min_seat(const int &start,const int &end) const
    {
        int output=seat_remained[start];
        for(int i=start;i<end;++i)
        {
            output=min(output,seat_remained[i]);
        }
        return output;
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
        RealTime arrival,departure;

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
    bool if_release() const { return If_release; }
    string train_id() const {return (string)trainID;}
    pair<Date,Date> date() const
    {
        pair<Date,Date> output(sale_beg,sale_end);
        return output;
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
    bool check_date(const Date &d) const
    {
        if(d<sale_beg || sale_end<d) return false;
        else return true;
    }
    int get_price(str i,str f) const
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
        return station[b].price-station[a].price;
    }
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
    }
    string information(str i,str f,const Date &d,ST &store) const
    {
        bool If_find_initial=false;
        int a=0,b=0,seat=seatNum;
        auto seats=store.FindByKey((string)trainID+" "+d.display()).first;
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

    bool operator<(const Train &t) const
    {
        return trainID<t.trainID;
    }
    string station_name(const int &pos) const
    {
        return (string)station[pos].name;
    }

    int station_number() const {return stationNum;}
    RealTime station_departure(const int &pos) const {return station[pos].departure;}
    RealTime station_arrival(const int &pos) const {return station[pos].arrival;}


};

enum STATUS{SUCCESS,PENDING,REFUNDED};
class Log
{
    int id; // The main-key.
    MyString status;
    MyString username;// The tag.
    MyString trainID;
    MyString From,To;
    RealTime departure,arrive;
    int price,num;
public:
    Log()=default;
    Log(int k,STATUS s,str u,str i,str f,str t,const RealTime &d,const RealTime &a,int p,int n):username(u),trainID(i),From(f),To(t),departure(d),arrive(a)
    {
        if(s==PENDING) status="pending";
        if(s==SUCCESS) status="success";
        if(s==REFUNDED)status="refunded";
        price=p; num=n; id=k;
    }
    string display() const
    {
        string output("["+(string)status+"] "+(string)trainID+" "+(string)From+" ");
        output+=departure.display()+" -> "+(string)To+" "+arrive.display()+" ";
        output+=to_string(price)+" "+to_string(num);
        return output;
    }
    void modify_status(const STATUS &s)
    {
        if(s==PENDING) status="pending";
        if(s==REFUNDED) status="refunded";
        if(s==SUCCESS) status="success";
    }
    string main_key() const {return (string)username+to_string(id);}
    string tag() const {return (string)username;}
};

struct TrainSort
{
    Train train;
    int value;

    explicit TrainSort(const Train &t,int v):train(t),value(v){}
    bool operator<(const TrainSort &t) const
    {
        return value<t.value;
    }
};


#endif //TICKETSYSTEM_2021_TRAIN_H
