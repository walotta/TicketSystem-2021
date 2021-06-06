//
// Created by wzj on 2021/1/18.
//

#ifndef BOOKSTORE_STORAGEPOOL_H
#define BOOKSTORE_STORAGEPOOL_H

#include <iostream>
#include <fstream>
#include "cache.hpp"
using std::string;
using std::fstream;
using std::ios;


template<class T,class extraBlock,int SizeOfCache>
class StoragePool
{
private:
    int WritePoint=-1;//块编号从0开始，-1说明需要写入新块
    int LastBlock=-1;//文件中的最后一块
    fstream pool;
    const int base=2*sizeof(int)+sizeof(extraBlock);
    //const string dir="StorageFile/";
    const string dir="";
    const string StorageFileName;
    cachePool<SizeOfCache,T> cache;

    inline void fileOpen()
    {
        pool.open(dir+StorageFileName,fstream::in|fstream::out|ios::binary);
        pool.seekg(0,ios::beg);
        pool.read(reinterpret_cast<char*>(&WritePoint),sizeof(int));
        pool.read(reinterpret_cast<char*>(&LastBlock),sizeof(int));
    }
    inline void fileClose()
    {
        pool.seekp(0,ios::beg);
        pool.write(reinterpret_cast<const char*>(&WritePoint),sizeof(int));
        pool.write(reinterpret_cast<const char*>(&LastBlock),sizeof(int));
        pool.close();
    }

public:
    StoragePool()=delete;

    explicit StoragePool(const string& FileName):StorageFileName(FileName)
    {
        pool.open(dir+FileName,fstream::in|ios::binary);
        if(pool.fail())
        {
            pool.clear();
            pool.close();
            pool.open(dir+FileName,fstream::out|ios::binary);
            pool.close();
            pool.open(dir+FileName,fstream::in|fstream::out|ios::binary);
            pool.seekp(0,ios::beg);
            WritePoint=-1;
            LastBlock=-1;
            pool.write(reinterpret_cast<const char*>(&WritePoint),sizeof(int));
            pool.write(reinterpret_cast<const char*>(&LastBlock),sizeof(int));
            extraBlock tem;
            pool.write(reinterpret_cast<const char*>(&tem),sizeof(extraBlock));
        }else
        {
            pool.seekg(0,ios::beg);
            pool.read(reinterpret_cast<char*>(&WritePoint),sizeof(int));
            pool.read(reinterpret_cast<char*>(&LastBlock),sizeof(int));
        }
        pool.close();
    }

    ~StoragePool()
    {
        /*pool.seekp(0,ios::beg);
        pool.write(reinterpret_cast<const char*>(&WritePoint),sizeof(int));
        pool.write(reinterpret_cast<const char*>(&LastBlock),sizeof(int));
        pool.close();*/
    }

    int add(const T &block)
    {
        fileOpen();
        int ans;
        if(WritePoint==-1)
        {
            LastBlock++;
            ans=LastBlock;
            pool.seekp(0,ios::end);
            pool.write(reinterpret_cast<const char*>(&block),sizeof(T));
        }else
        {
            int tem;
            ans=WritePoint;
            pool.seekg(base+WritePoint*sizeof(T),ios::beg);
            pool.read(reinterpret_cast<char*>(&tem),sizeof(int));
            pool.seekp(base+WritePoint*sizeof(T),ios::beg);
            pool.write(reinterpret_cast<const char*>(&block),sizeof(T));
            WritePoint=tem;
        }
        fileClose();
        cache.insert(ans,block);
        return ans;
    }

    void remove(int id)
    {
        fileOpen();
        pool.seekp(base+id*sizeof(T),ios::beg);
        pool.write(reinterpret_cast<const char*>(&WritePoint),sizeof(int));
        fileClose();
        WritePoint=id;
        cache.remove(id);
    }

    void update(int id,const T &block)
    {
        fileOpen();
        pool.seekp(base+id*sizeof(T),ios::beg);
        pool.write(reinterpret_cast<const char*>(&block),sizeof(T));
        fileClose();
        cache.update(id,block);
    }

    T get(int id)
    {
        T tem;
        int cache_id=cache.find(id);
        if(cache_id==-1)
        {
            fileOpen();
            pool.seekg(base+id*sizeof(T),ios::beg);
            pool.read(reinterpret_cast<char*>(&tem),sizeof(T));
            fileClose();
            cache.insert(id,tem);
        }else
        {
            tem=cache[cache_id];
        }
        /*fileOpen();
        T tem;
        pool.seekg(base+id*sizeof(T),ios::beg);
        pool.read(reinterpret_cast<char*>(&tem),sizeof(T));
        fileClose();*/
        return tem;
    }

    void clearAll()
    {
        pool.open(dir+StorageFileName,fstream::out|ios::binary);
        pool.close();
        pool.open(dir+StorageFileName,fstream::in|fstream::out|ios::binary);
        pool.seekp(0,ios::beg);
        WritePoint=-1;
        LastBlock=-1;
        pool.write(reinterpret_cast<const char*>(&WritePoint),sizeof(int));
        pool.write(reinterpret_cast<const char*>(&LastBlock),sizeof(int));
        extraBlock tem;
        pool.write(reinterpret_cast<const char*>(&tem),sizeof(extraBlock));
        pool.close();
        cache.clear();
    }

private:
    int tellSpace()
    {
        fileOpen();
        fileClose();
        return LastBlock;
    }

public:

    extraBlock readExtraBlock()
    {
        fileOpen();
        pool.seekg(2*sizeof(int),ios::beg);
        extraBlock tem;
        pool.read(reinterpret_cast<char*>(&tem),sizeof(extraBlock));
        fileClose();
        return tem;
    }

    void writeExtraBlock(const extraBlock& o)
    {
        fileOpen();
        pool.seekp(2*sizeof(int),ios::beg);
        pool.write(reinterpret_cast<const char*>(&o),sizeof(extraBlock));
        fileClose();
    }
};



#endif //BOOKSTORE_STORAGEPOOL_H
