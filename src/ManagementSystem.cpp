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

void ManagementSystem::query_train(const string &i,Date d,vecS &out)
{
    train.query_train(i,d,out);
}

bool ManagementSystem::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    return user.add_user(c,u,p,n,m,g);
}

bool ManagementSystem::add_train(const string &i,int n,int m,const vecS &s,const vector<int> &p,Time x,const vector<int> &t,const vector<int> &o,Date d_beg,Date d_end,char y)
{
    return train.add_train(i,n,m,s,p,x,t,o,d_beg,d_end,y);
}

void ManagementSystem::query_ticket(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    train.query_ticket(s,t,d,If_time,out);
}

void ManagementSystem::query_transfer(const string &s,const string &t,Date d,bool If_time,vecS &out)
{
    train.query_transfer(s,t,d,If_time,out);
}

void ManagementSystem::query_order(const string &u,vecS &out)
{
    if(user.check_priority(u)==-404) out.push_back("-1");
    else train.query_order(u,out);
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
    if(user.check_priority(u)==-404) return fail;
    int total_number=user.query_order_number(u);
    int number=total_number+1-n;
    if(number<=0 || number>total_number) return fail;

    Date refund_date;
    auto back=train.refund_ticket(u,number,refund_date);
    if(back.second==REFUNDED) return fail;

    auto &trainID=back.first;
    vector<index> order_index;
    orders.get_ids(trainID,order_index);
    if(back.second==PENDING)
    {
        for(int i=0; i<order_index.size(); ++i)
        {
            int id=order_index[i].first;
            Order order(orders.get_order(id));
            if(order.id==number && order.user==u)
            {
                orders.remove_order(id,trainID,order.serial_number);
                break;
            }
        }
    }
    else
    {
        // Process pending after refund.
        static vector<pair<Order,int>> order_list;
        order_list.clear();
        orders.get_orders(trainID,order_list);

        auto train1=train.get_train(trainID);
        auto seats=train.get_seat(trainID,refund_date);
        for(int i=0;i<order_list.size();++i)
        {
            auto &order=order_list[i].first;
            auto date=train1.set_off_date((string)order.start,order.date);
            if(!(date==refund_date)) continue;

            bool If_success=train.re_buy_ticket((string)order.start,(string)order.arrive,order.number,order.id,(string)order.user,train1,seats);
            if(If_success)
            {
                string serial_key(to_string(order.serial_number)),&tag=trainID;
                orders.remove_order(order_list[i].second,trainID,order.serial_number);
            }
        }
        train.update_seat(trainID,refund_date,seats);
    }
    return success;
}

string ManagementSystem::buy_ticket(const string &u,const string &i,const Date &d,const string &f,const string &t,int n,bool q)
{
    string fail="-1",into_queue="queue";
    int pri=user.check_priority(u);
    if(pri==-404) return fail;

    int number=user.query_order_number(u); //consider: This place could optimize.
    lint cost=train.buy_ticket(i,d,f,t,n,number +1,u,q);
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

void ManagementSystem::add_order(int id,const string &u,const string &i,const Date &d,const string &f,const string &t,const int &n)
{
    auto number=orders.order_number()+1;
    Order new_order(number,id,u,i,d,f,t,n);
    orders.add_order(i,number,new_order);
}





