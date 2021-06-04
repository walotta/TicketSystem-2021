//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USERMANAGER_H
#define TICKETSYSTEM_2021_USERMANAGER_H

#include "User.h"
#include "TrainManager.h"

class UserManager
{
    class UserStorage
    {
        StoragePool<User,bool,300> user_data;
        BPlusTree<300,300> user_name_index;
    public:
        UserStorage():user_data("user_data.dat"),user_name_index("user_index.dat"){}
        ~UserStorage()=default;

        int get_id(str username)
        {
            vector<index> temp;
            user_name_index.find(username,temp);
            if(temp.empty()) return -404;
            return temp[0].first;
        }
        User get_user(int id) {return user_data.get(id);}

        void update(const int &id,const User &user) {user_data.update(id,user);}
        void insert(str username,const User &user)
        {
            int id=user_data.add(user);
            user_name_index.insert({username,id},id);//todo: get serial_number.
        }

        bool empty() {return user_data.readExtraBlock();}
        void not_empty() {user_data.writeExtraBlock(true);}
        void clean()
        {
            user_data.clearAll();
            user_name_index.clean();
        }
    };
    UserStorage users;
    // map<username,pair<priority,orderNum>>
    unordered_map<string,pair<int,int>> logged_users;

public:
    UserManager(){}
    ~UserManager(){logged_users.clear();}

    bool add_user(str c,str u,str p,str n,str m,int g);
    bool login(str u,str p);
    bool logout(str u);
    string query_profile(str c,str u);
    string modify_profile(str c,str u,str p,str n,str m,int g);
    bool clean();

    int check_priority(str u) const;// If the users is logged, return its priority; else return -404;
    int query_user_priority(str u);
    int query_order_number(str u) const;
    bool add_order_number(str u);
};


#endif //TICKETSYSTEM_2021_USERMANAGER_H
