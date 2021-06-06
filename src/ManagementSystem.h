//
// Created by Fourest on 2021/5/12.
//

#ifndef TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
#define TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H

#include "TrainManager.h"
#include "UserManager.h"


class ManagementSystem
{

    struct Order
    {
        int serial_number;
        int id;// Represent the number of the order in its users's list.
        MyString user;
        MyString trainID;// The tag
        Date date;
        MyString start;
        MyString arrive;
        int number;

        Order()=default;
        Order(int k,int _id,str u,str i,const Date &d,str f,str t,const int &n):serial_number(k),id(_id),user(u),trainID(u),date(d),start(f),arrive(t),number(n){}
        bool operator<(const Order &o) const {return serial_number<o.serial_number;}
    };
    class OrderStorage
    {
        struct ints {int value=0; ints()=default; explicit ints(int x):value(x){}};

        BPlusTree<1000,500> order_index;
        StoragePool<Order,ints,500> order_data;

    public:
        OrderStorage():order_index("order_index.dat"),order_data("order_data.dat"){}
        ~OrderStorage()=default;

        int get_id(str train_id,const int &id)
        {
            vector<ex_index> temp;
            order_index.find(train_id,temp);
            if(temp.empty()) return -404;
            return temp[0].first;
        }
        void add_order(str train_id,const int &number,const Order &order)
        {
            order_data.writeExtraBlock(ints(number));
            int order_id=order_data.add(order);
            order_index.insert({train_id,number},order_id);
        }
        void update(const int &order_id,const Order &order) {order_data.update(order_id,order);}
        Order get_order(const int &order_id) {return order_data.get(order_id);}

        void get_orders(str train_id,vector<pair<Order,int>> &out)
        {
            vector<ex_index> temp;
            order_index.find(train_id,temp);
            for(int i=0;i<temp.size();++i)
                out.push_back({order_data.get(temp[i].first),temp[i].first});
            sort(out.begin(),out.end());
            //consider: This place could be optimized
        }
        int order_number() {return order_data.readExtraBlock().value;}
        void remove_order(const int &order_id,str train_id,const int &number)
        {
            order_index.remove({train_id,number},order_id);
            order_data.remove(order_id);
        }

        int get_ids(str train_id,vector<ex_index> &out)
        {
            order_index.find(train_id,out);
            return out.size();
        }

        void clean()
        {
            order_index.clean();
            order_data.clearAll();
        }
    };


    TrainManager train;
    UserManager user;
    OrderStorage orders;

    void add_order(int id,str u,str i,const Date &d,str f,str t,const int &n);

public:
    ManagementSystem()=default;
    ~ManagementSystem()=default;

    bool add_train(str i,int n,int m,const vecS &s,vecI p,Time x,vecI t,vecI o,Date d_beg,Date d_end,char y);
    bool release_train(str i);
    void query_train(str i,Date d,vecS &out);
    bool delete_train(str i);
    void query_ticket(str s,str t,Date d,bool If_time,vecS &out);
    void query_transfer(str s,str t,Date d,bool If_time,vecS &out);
    string buy_ticket(str u,str i,const Date &d,str f,str t,int n,bool q);

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    void query_order(str u,vecS &out); // If query failed, return an empty vector.

    int query_user_priority(str u);
    bool refund_ticket(str u,int n);
    bool clean();
};


#endif //TICKETSYSTEM_2021_MANAGEMENTSYSTEM_H
