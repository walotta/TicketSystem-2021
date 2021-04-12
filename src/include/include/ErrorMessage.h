//
// Created by wzj on 2021/1/20.
//

#ifndef BOOKSTORE_ERRORMESSAGE_H
#define BOOKSTORE_ERRORMESSAGE_H
#include <cstring>
#include <stdexcept>

class error: public std::exception
{
private:
    std::string message;
public:
    explicit error(const std::string &mes):message(mes) {}
    //error(const error &o):message(o.message) {}
    std::string what(){return message;}
    std::string get()const{return message;}
};
#endif //BOOKSTORE_ERRORMESSAGE_H
