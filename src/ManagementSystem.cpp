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

vecS ManagementSystem::query_ticket(const string &s,const string &t,Date d,const string &p)
{
    return train.query_ticket(s,t,d,p);
}

vecS ManagementSystem::query_transfer(const string &s,const string &t,Date d,const string &p)
{
    return train.query_transfer(s,t,d,p);
}

vector<string> ManagementSystem::query_order(const string &u)
{
    return user.query_order(u);
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

string ManagementSystem::buy_ticket(const string &u,const string &i,Date d,const string &f,const string &t,int n,bool q)
{
    int pri=user.check_login(u);
    if(pri==-404) return "-1";

    int cost=train.buy_ticket(i,d,f,t,n);
    if(cost==-1)
    {
        if(q)
        {
            //todo: Add to queue.
            return "queue";
        }
        else return "-1";
    }
    else
    {
        user.buy_ticket(u,i,d,f,t,n,q);
        return to_string(cost);
    }


}

bool ManagementSystem::clean()
{
    user.clean();
    train.clean();
    return true;
}
