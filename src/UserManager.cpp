//
// Created by Fourest on 2021/5/10.
//

#include "UserManager.h"

bool UserManager::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    if(user.size()==0)
    {
        if(!loggedUser.empty()) return false;
        if(user.size()!=0) return false;
        User firstUser(u,p,n,m,10);
        user.insert(u,firstUser);
        return true;
    }

    int pri=check_login(c);
    if(pri==-404 || pri<=g) return false;

    auto tp=user.FindByKey(u);
    if(tp.second) return false;

    User temp(u,p,n,m,g);
    user.insert(u,temp);
    return true;
}

bool UserManager::login(const string &u,const string &p)
{
    if(check_login(u)!=-404) return false;

    auto temp=user.FindByKey(u);
    if(!temp.second) return false;
    if(!temp.first.check_pass(p)) return false;

    pair<string,pair<int,int>> add(u,pair<int,int>(temp.first.pri(),temp.first.order_number()));
    loggedUser.insert(add);
    return true;
}

bool UserManager::logout(const string &u)
{
    auto temp=loggedUser.find(u);
    if(temp==loggedUser.end()) return false;
    loggedUser.erase(temp);
    return true;
}

string UserManager::query_profile(const string &c,const string &u)
{
    string fail="-1";

    int pri=check_login(c);
    if(pri==-404) return fail;// Un-logged user "c".

    auto tp=user.FindByKey(u);
    if(!tp.second) return fail;// Nonexistent user.
    if(tp.first.pri()>pri) return fail;// Access denied.

    return tp.first.display();
}

string UserManager::modify_profile(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    string fail="-1";

    int pri=check_login(c);
    if(pri==-404) return fail;// Un-logged user "c".

    auto tp=user.FindByKey(u);
    if(!tp.second) return fail; // Nonexistent user "u".
    auto &user1=tp.first;
    if(user1.pri()>pri || g>=pri) return fail;// Access denied.

    string pp=p,nn=n,mm=m;
    if(p!="") user1.pass()=p;
    if(n!="") user1.nam()=n;
    if(m!="") user1.mail()=m;
    if(g==-404)  g=user1.pri();
    user.Update(u,user1);
    return user1.display();
}

int UserManager::check_login(const string &u) const
{
    auto tpp=loggedUser.find(u);
    if(tpp==loggedUser.end()) return -404;
    return tpp->second.first;
}

bool UserManager::clean()
{
    user.clean();
    loggedUser.clear();
    return true;
}

int UserManager::query_user_priority(const string &u)
{
    auto tp=user.FindByKey(u);
    if(!tp.second) return -404;
    return tp.first.pri();
}

int UserManager::query_order_number(const string &u) const
{
    auto temp=loggedUser.find(u);
    return temp->second.second;
}

bool UserManager::add_order_number(const string &u)
{
    auto t=user.FindByKey(u);
    if(!t.second) return false;
    t.first.add_order();
    user.Update(u,t.first);

    auto temp=loggedUser.find(u);
    if(loggedUser.end()!=temp) temp->second.second++;
    return true;
}




















