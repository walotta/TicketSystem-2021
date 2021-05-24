//
// Created by Fourest on 2021/5/10.
//

#include "UserManager.h"

bool UserManager::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    //todo: When first add user...
    auto tpp=loggedUser.find(c);
    if(tpp==loggedUser.end() || tpp->second<=g) return false;

    auto tp=user.FindByKey(u);
    if(tp.second) return false;

    User temp(u,p,n,m,g);
    user.insert(u,temp);
    return true;
}

bool UserManager::login(const string &u,const string &p)
{
    auto tpp=loggedUser.find(u);
    if(tpp!=loggedUser.end()) return false;

    auto temp=user.FindByKey(u);
    if(!temp.second) return false;
    if(!temp.first.check_pass(p)) return false;

    pair<string,int> add(u,temp.first.pri());
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
    auto tpp=loggedUser.find(c);
    if(tpp==loggedUser.end()) return "-1";

    auto tp=user.FindByKey(u);
    if(!tp.second) return "-1";
    if(tp.first.pri()>tpp->second) return "-1";// Access denied.

    return tp.first.display();
}

string UserManager::modify_profile(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    auto tpp=loggedUser.find(c);
    if(tpp==loggedUser.end()) return "-1";

    auto tp=user.FindByKey(u);
    if(!tp.second) return "-1";
    if(tp.first.pri()>tpp->second || g>=tpp->second) return "-1";

    string pp=p,nn=n,mm=m;
    if(p=="") pp=tp.first.pass();
    if(n=="") nn=tp.first.nam();
    if(m=="") mm=tp.first.mail();
    if(g==-404)  g=tp.first.pri();
    User temp(u,pp,nn,mm,g);
}
