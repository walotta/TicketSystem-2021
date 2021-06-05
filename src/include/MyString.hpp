//
// Created by wzj on 2021/3/26.
//
#include <cstring>
#include <iostream>

using std::string;
using std::ostream;

class MyString
{
private:
    //todo: Add a function that can change the size of this class.
    char storage[80]{};
public:
    MyString(){storage[0]='\0';}
    MyString(const string &o)
    {
        strcpy(storage,o.c_str());
    }
    bool operator<(const MyString& o)const
    {
        return strcmp(storage,o.storage)<0;
    }
    bool operator>(const MyString& o)const
    {
        return strcmp(storage,o.storage)>0;
    }
    bool operator<(const string &o)const
    {
        return strcmp(storage,o.c_str())<0;
    }
    bool operator>(const string &o)const
    {
        return strcmp(storage,o.c_str())>0;
    }
    friend bool operator<(const string& o,const MyString &m)
    {
        return strcmp(o.c_str(),m.storage)<0;
    }
    friend bool operator>(const string& o,const MyString &m)
    {
        return strcmp(o.c_str(),m.storage)>0;
    }
    MyString& operator=(const MyString &o)
    {
        if(this==&o)return *this;
        strcpy(storage,o.storage);
        return *this;
    }
    MyString& operator=(const string &o)
    {
        strcpy(storage,o.c_str());
        return *this;
    }
    explicit operator string ()const
    {
        return string(storage);
    }
    bool operator==(const MyString& o)const
    {
        return strcmp(storage,o.storage)==0;
    }
    bool operator!=(const MyString& o)const
    {
        return strcmp(storage,o.storage)!=0;
    }
    bool operator==(const string& o)const
    {
        return strcmp(storage,o.c_str())==0;
    }
    bool operator!=(const string& o)const
    {
        return strcmp(storage,o.c_str())!=0;
    }
    friend bool operator==(const string& o,const MyString &m)
    {
        return strcmp(o.c_str(),m.storage)==0;
    }
    friend bool operator!=(const string& o,const MyString &m)
    {
        return strcmp(o.c_str(),m.storage)!=0;
    }
    friend ostream &operator<<(ostream& os,const MyString& m)
    {
        os<<m.storage;
        return os;
    }
};