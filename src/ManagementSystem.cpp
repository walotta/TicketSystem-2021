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

bool ManagementSystem::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,Time x,const vector<int> &t,const vector<int> &o,Date d_beg,Date d_end,char y)
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
    if(user.check_login(u)==-404) return vecS({"-1"});
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
    bool fail=false,success=true;
    if(user.check_login(u)==-404) return fail;
    int number=user.query_order_number(u)+1-n;
    if(number<=0) return fail;

    auto back=train.refund_ticket(u,number);
    if(back.second==-1) return fail;

    auto &trainID=back.first;
    auto orders=order.FindByTag(trainID);
    if(back.second==0)
    {
        for(int i=0; i<orders.size(); ++i)
        {
            if(orders[i].id==number && orders[i].user==u)
            {
                string main_key(to_string(orders[i].serial_number)),tag(trainID);
                order.RemoveTag(main_key,tag);
                order.Remove(main_key);
                break;
            }
        }
    }
    else
    {
        // Process pending after refund.
        sort(orders.begin(),orders.end());
        int ll=orders.size();
        for(int i=0;i<ll;++i)
        {
            auto &orderI=orders[i];
            bool If_success=train.re_buy_ticket(trainID,orderI.date,(string)orderI.start,(string)orderI.arrive,orderI.number,orderI.id,(string)orderI.user);
            if(If_success)
            {
                string main_key(to_string(orderI.serial_number)),tag(trainID);
                order.RemoveTag(main_key,tag);
                order.Remove(main_key);
            }
        }
    }
    return success;
}

string ManagementSystem::buy_ticket(const string &u,const string &i,const Date &d,const string &f,const string &t,int n,bool q)
{
    string fail="-1",into_queue="queue";
    int pri=user.check_login(u);
    if(pri==-404) return fail;

    int number=user.query_order_number(u); //consider: This place could optimize.
    int cost=train.buy_ticket(i,d,f,t,n,number+1,u,q);
    if(cost>0)
    {
        user.add_order_number(u);
        return to_string(cost);
    }
    else
    {
        if(cost==-404) return fail;
        if(q)
        {
            add_order(number+1,u,i,d,f,t,n);
            user.add_order_number(u);
            return into_queue;
        }
        else return fail;
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
    bool If_queue_success=train.re_buy_ticket((string)ord.trainID,ord.date,(string)ord.start,(string)ord.arrive,ord.number,ord.id,(string)ord.user);
    if(If_queue_success) return true;
    else return false;
}

void ManagementSystem::add_order(int id,const string &u,const string &i,const Date &d,const string &f,const string &t,const int &n)
{
    auto number=1+order.read_log();
    Order new_order(number,id,u,i,d,f,t,n);
    string main_key(to_string(number));
    order.insert(main_key,new_order);
    order.AddTag(main_key,i);
    order.plus_log();
}





