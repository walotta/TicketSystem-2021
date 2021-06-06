//
// Created by wzj on 2021/3/26.
//
#include "ErrorMessage.h"
#include <unordered_map>
template<int cacheSize,class T>
class cachePool
{
private:
    //int id[cacheSize];
    struct block
    {
        int a=-1;
        block(int val=-1):a(val){}
    };
    std::unordered_map<int,block> index;
    std::pair<int,T*> storage[cacheSize];
    int size;
    int head;
public:
    cachePool()
    {
        size=0;
        head=-1;
    }
    ~cachePool()
    {
        for(int i=0;i<size;i++)
        {
            delete storage[i].second;
        }
    }
    int find(int _id)
    {
        return index[_id].a;
    }
    T operator[](int pos)
    {
        if(pos<0||pos>=size)throw error("cache operator[] error");
        return *(storage[pos].second);
    }
    void update(int _id,const T& other)
    {
        int pos=find(_id);
        if(pos==-1)
        {
            insert(_id,other);
        }else
        {
            delete storage[pos].second;
            storage[pos].second=new T(other);
        }
    }
    void insert(int _id,const T& other)
    {
        if(size==cacheSize)
        {
            head++;
            head%=size;
            delete storage[head].second;
            storage[head].second=new T(other);
            index.erase(storage[head].first);
            storage[head].first=_id;
            index[_id]=head;
        }else
        {
            head++;
            size++;
            storage[head].second=new T(other);
            storage[head].first=_id;
            index[_id]=head;
        }
    }
    void remove(int _id)
    {
        int pos;
        pos=find(_id);
        if(pos==-1)return;
        index.erase(_id);
    }
    void clear()
    {
        for(int i=0;i<size;i++)
        {
            delete storage[i].second;
        }
        index.clear();
        size=0;
        head=-1;
    }
};
