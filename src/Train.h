//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_TRAIN_H
#define TICKETSYSTEM_2021_TRAIN_H

#include "base.h"



class Train
{
    struct Station
    {
        MyString name;
        int price; // The "price" store the price that from the Departure station.
        RealTime arrival,departure;
        int NUM=0;
        int seat_remain[100];

        Station()=default;

        void update(str n,int p,int num,const RealTime &t1,const RealTime &t2)
        {
            name=n;
            price=p;
            NUM=num;
            arrival=t1;
            departure=t2;
            for(int i=0;i<100;++i) seat_remain[i]=num;
        }
        void update(str n,int p,int num)
        {
            name=n;
            price=p;
            NUM=num;
            for(int i=0;i<100;++i) seat_remain[i]=num;
        }
    };

    Station station[120];
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

    bool release()
    {
        if(If_release) return false;
        else
        {
            If_release=true;
            return true;
        }
    }
    bool if_release() const { return If_release; }

    pair<Date,Date> date() const
    {
        pair<Date,Date> output(sale_beg,sale_end);
        return output;
    }

    vecS query_train() const
    {
        vecS output;
        output.push_back((string)trainID+" "+type);
        int price=0;
        for(int i=0;i<stationNum;++i)
        {
            string temp;
            temp+=(string)station[i].name;
            temp+=" "+station[i].arrival.display()+" -> "+station[i].departure.display();
            //todo

        }



        return output;
    }

    string train_id() const {return (string)trainID;}

    bool operator<(const Train &t) const
    {
        return trainID<t.trainID;
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

    string information(str i,str f,const Date &d) const
    {
        bool If_find_initial=false;
        int a=0,b=0,seat=seatNum;
        for(int j=0;j<stationNum;++j)
        {
            auto &st=station[j];
            if(i==st.name) {a=j; If_find_initial=true;}
            if(f==st.name) {b=j; break;}
            if(If_find_initial) seat=min(seat,st.seat_remain[d.dayNum()]);
        }
        int price=station[b].price-station[a].price;
        Time departure(station[a].departure.time()),arrival(station[b].arrival.time());
        string output((string)trainID+" "+i+" "+departure.display());
        output+=" -> "+f+" "+arrival.display()+" "+to_string(price)+" "+to_string(seat);
        return output;
    }


};


#endif //TICKETSYSTEM_2021_TRAIN_H
