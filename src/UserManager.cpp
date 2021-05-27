//
// Created by Fourest on 2021/5/10.
//

#include "UserManager.h"

bool UserManager::add_user(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    if(c=="")
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
    int pri=check_login(c);
    if(pri==-404) return "-1";

    auto tp=user.FindByKey(u);
    if(!tp.second) return "-1";
    if(tp.first.pri()>pri) return "-1";// Access denied.

    return tp.first.display();
}

string UserManager::modify_profile(const string &c,const string &u,const string &p,const string &n,const string &m,int g)
{
    int pri=check_login(c);
    if(pri==-404) return "-1";

    auto tp=user.FindByKey(u);
    if(!tp.second) return "-1";
    if(tp.first.pri()>pri || g>=pri) return "-1";

    string pp=p,nn=n,mm=m;
    if(p=="") pp=tp.first.pass();
    if(n=="") nn=tp.first.nam();
    if(m=="") mm=tp.first.mail();
    if(g==-404)  g=tp.first.pri();
    User temp(u,pp,nn,mm,g);
    user.Update(u,temp);
    return temp.display();
}

vecS UserManager::query_order(const string &u)
{
    if(check_login(u)==-404) return vecS();

    auto temp=log.FindByTag(u);
    vecS output;
    for(int i=0;i<temp.size();++i) output.push_back(temp[i].display());
    return output;
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
    log.clean();
    loggedUser.clear();
    return true;
}



















