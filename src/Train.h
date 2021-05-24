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
        int price;
        RealTime arrival,departure;
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

        for(int ii=0;ii<n;++ii)
        {
            station[ii].name=s[ii];
            station[ii].price=p[ii];
            //todo: process time.
        }

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
        for(int i=0;i<stationNum;++i)
        {
            string temp;
            temp+=(string)station[i].name;
            temp+=" "+station[i].arrival.display()+" -> "+station[i].departure.display();
            //todo

        }



        return output;
    }

};


#endif //TICKETSYSTEM_2021_TRAIN_H
