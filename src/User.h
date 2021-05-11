//
// Created by Fourest on 2021/5/10.
//

#ifndef TICKETSYSTEM_2021_USER_H
#define TICKETSYSTEM_2021_USER_H

#include <string>

typedef const std::string & str;

class User
{
    std::string username;// Consist of number, alphabet and '_', and its head must be alphabet, with length<=20.
    std::string password;// Consist of number, alphabet and '_', with length<=30.
    std::string name;// Consist of Chinese character, with 2<=length<=5.
    std::string mailAddr;// Consist of number, alphabet, '@' & '.', with length<=30, needless to check its legality.
    int privilege;// Z,[0,10]

public:
    User()=default;
    User(str u,str p,str n,str m,int g);

};


#endif //TICKETSYSTEM_2021_USER_H
