//
// Created by Fourest on 2021/5/10.
//

#include "UserManager.h"

bool UserManager::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    if(users.empty())
    {
        if(!logged_users.empty()) return false;
        User first_user(u,p,n,m,10);
        users.add_user(u,first_user);
        users.not_empty();
        return true;
    }

    int pri=check_priority(c);
    if(pri==-404 || pri<=g) return false;
    if(users.get_id(u)>=0) return false;

    User temp(u,p,n,m,g);
    users.add_user(u,temp);
    return true;
}

bool UserManager::login(const string &u,const string &p)
{
    if(check_priority(u)!=-404) return false;

    int id=users.get_id(u);
    if(id<0) return false;
    User user(users.get_user(id));
    if(!user.check_pass(p)) return false;

    std::pair<int,int> p_n(user.priority(),user.order_number());
    logged_users.insert(u,p_n); 
    return true;
}

bool UserManager::logout(const string &u)
{
    auto temp=logged_users.find(u);
    if(temp==nullptr) return false;
    logged_users.erase(u);
    return true;
}

string UserManager::query_profile(const string &c,const string &u)
{
    string fail("-1");

    int priority=check_priority(c);
    if(priority==-404) return fail;// Un-logged users "c".

    int id=users.get_id(u);
    if(id<0) return fail;// Nonexistent users.
    User user(users.get_user(id));
    if(user.priority()>=priority && c!=u) return fail;// Access denied.

    return user.display();
}

string UserManager::modify_profile(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    string fail("-1");

    int priority=check_priority(c);
    if(priority==-404) return fail;// Un-logged users "c".

    int id=users.get_id(u);
    if(id<0) return fail; // Nonexistent users "u".
    User user(users.get_user(id));
    if((user.priority()>=priority && c!=u) || g>=priority) return fail;// Access denied.

    if(p!="") user.pass()=p;
    if(n!="") user.nam()=n;
    if(m!="") user.mail()=m;
    if(g!=-404) user.priority()=g;
    users.update(id,user);
    return user.display();
}

int UserManager::check_priority(const string &u) const
{
    auto tpp=logged_users.find(u);
    if(tpp==nullptr) return -404;
    return tpp->first;
}

bool UserManager::clean()
{
    users.clean();
    logged_users.clear();
    return true;
}

int UserManager::query_user_priority(const string &u)
{
    int id=users.get_id(u);
    if(id<0) return -404;
    return users.get_user(id).priority();
}

int UserManager::query_order_number(const string &u) const
{
    auto temp=logged_users.find(u);
    return temp->second;
}

bool UserManager::add_order_number(const string &u)
{
    int id=users.get_id(u);
    if(id<0) return false;
    User user(users.get_user(id));
    user.add_order();
    users.update(id,user);

    auto temp=logged_users.find(u);
    if(temp!=nullptr) temp->second++;
    return true;
}

