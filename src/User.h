//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USER_H
#define TICKETSYSTEM_2021_USER_H

//#include <string>
#include "base.h"

class User;

class User
{
    MyString username;// Consist of number, alphabet and '_', and its head must be alphabet, with length<=20.
    MyString password;// Consist of number, alphabet and '_', with length<=30.
    MyString name;// Consist of Chinese character, with 2<=length<=5.
    MyString mailAddr;// Consist of number, alphabet, '@' & '.', with length<=30, needless to check its legality.
    int privilege;// Z,[0,10]
    int orderedNum=0;

public:
    User()=default;
    ~User()=default;
    User(const User &user):username(user.username),password(user.password),name(user.name),mailAddr(user.mailAddr)
    {
        privilege=user.privilege;
        orderedNum=user.orderedNum;
    }
    User(str u,str p,str n,str m,int g): username(u),password(p),name(n),mailAddr(m),privilege(g){}

    string display() const {return (string)username+" "+(string)name+" "+(string)mailAddr+" "+std::to_string(privilege);}
    bool check_pass(str p) const {return password==p;}

    MyString &user() {return username;}
    MyString &pass() {return password;}
    MyString &nam()  {return name;}
    MyString &mail() {return mailAddr;}
    int &priority() {return privilege;}
    int order_number() const {return orderedNum;}
    void add_order() {++orderedNum;}

};





#endif //TICKETSYSTEM_2021_USER_H
