//
// Created by Fourest on 2021/5/12.
//

#include "ManagementSystem.h"

bool ManagementSystem::release_train(const string &i)
{
    return train.release_train(i);
}

bool ManagementSystem::logout(const string &u)
{
    return user.logout(u);
}

bool ManagementSystem::login(const string &u,const string &p)
{
    return user.login(u,p);
}

bool ManagementSystem::delete_train(const string &i)
{
    return train.delete_train(i);
}

vecS ManagementSystem::query_train(const string &i,Date d)
{
    return train.query_train(i,d);
}

bool ManagementSystem::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    return user.add_user(c,u,p,n,m,g);
}

bool
ManagementSystem::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,Time x,const vector<int> &t,const vector<int> &o,Date d_beg,Date d_end,char y)
{
    return train.add_train(i,n,m,s,p,x,t,o,d_beg,d_end,y);
}

vecS ManagementSystem::query_ticket(const string &s,const string &t,Date d,bool If_time)
{
    return train.query_ticket(s,t,d,If_time);
}

vecS ManagementSystem::query_transfer(const string &s,const string &t,Date d,bool If_time)
{
    return train.query_transfer(s,t,d,If_time);
}

vector<string> ManagementSystem::query_order(const string &u)
{
    if(!user.check_login(u)) return vecS();
    return train.query_order(u);
}

string ManagementSystem::query_profile(const string &c,const string &u)
{
    return user.query_profile(c,u);
}

string ManagementSystem::modify_profile(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    return user.modify_profile(c,u,p,n,m,g);
}

bool ManagementSystem::refund_ticket(const string &u,int n)
{
    if(train.refund_ticket())//todo
    {
        user.refund_ticket(u,n);
        return true;
    }
    else return false;
}

string ManagementSystem::buy_ticket(const string &u,const string &i,const Date &d,const string &f,const string &t,int n,bool q)
{
    string fail="-1",into_queue="queue";
    int pri=user.check_login(u);
    if(pri==-404) return fail;

    int cost=train.buy_ticket(i,d,f,t,n);
    if(cost==-1)
    {
        if(q)
        {
            int num=order.plus_log();
            vecS tag;
            tag.push_back(u);
//            int id=user.write_log(u,i,d,f,t,n,PENDING);
            Order temp(num,id,u,i,d,f,t,n);
            order.insert(to_string(num),temp);
            order.AddTag(to_string(num),tag);
            return into_queue;
        }
        else return fail;
    }
    else
    {
        user.write_log(u,i,d,f,t,n,SUCCESS);
//        user.buy_ticket(u,i,d,f,t,n,q);
        return to_string(cost);
    }
}

bool ManagementSystem::clean()
{
    user.clean();
    train.clean();
    return true;
}

int ManagementSystem::query_user_priority(const string &u)
{
    return user.query_user_priority(u);
}

bool ManagementSystem::buy_ticket(const ManagementSystem::Order &ord)
{
    int cost=train.buy_ticket((string)ord.trainID,ord.date,(string)ord.start,(string)ord.arrive,ord.number);
    if(cost==-1) return false;

    user.write_log((string)ord.user,(string)ord.trainID,ord.date,(string)ord.start,(string)ord.arrive,ord.number,SUCCESS);
    return true;
}





