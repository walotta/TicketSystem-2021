//
// Created by wzj on 2021/1/18.
//

#ifndef BOOKSTORE_FILESTORAGE_H
#define BOOKSTORE_FILESTORAGE_H

#define Bpt_Size 300

#include "multiBpt.h"
#include <utility>
#include "StoragePool.h"
#include "ErrorMessage.h"
#include <map>
using namespace std;

template<class T,int SizeOfBpt,int SizeOfBptBlock,int CacheOfBpt,int CacheOfBptBlock,int CacheOfData>
class StorageManger
{
private:
    class assistBlock
    {
    public:
        int cnt=0;
        int log=0;
    }baseMessage;
    const string name;
    StoragePool<T,assistBlock,CacheOfData>* dataLibrary;
    BPlusTree<SizeOfBpt,CacheOfBpt>* MainIndex;
    multiBPlusTree<SizeOfBpt,SizeOfBptBlock,CacheOfBpt,CacheOfBptBlock>* TagIndex=nullptr;
public:
    StorageManger()=delete;
    StorageManger(const string& _name,bool has_tag):name(_name)
    {
        dataLibrary=new StoragePool<T,assistBlock,CacheOfData>("dataLibrary_"+name);
        MainIndex=new BPlusTree<SizeOfBpt,CacheOfBpt>(name);
        if(has_tag)TagIndex=new multiBPlusTree<SizeOfBpt,SizeOfBptBlock,CacheOfBpt,CacheOfBptBlock>(name);
        baseMessage=dataLibrary->readExtraBlock();
    }

    ~StorageManger()
    {
        dataLibrary->writeExtraBlock(baseMessage);
        delete dataLibrary;
        delete MainIndex;
        if(TagIndex!=nullptr)delete TagIndex;
    }

    inline int size()
    {
        return baseMessage.cnt;
    }

    inline void insert(const string& MainKey,const T& to_insert)
    {
        int new_id=dataLibrary->add(to_insert);
        MainIndex->insert(MainKey,new_id);
        baseMessage.cnt++;
    }

    inline void Remove(const string& MainKey)
    {
        int remove_id=MainIndex->find(MainKey)[0];
        dataLibrary->remove(remove_id);
        baseMessage.cnt--;
    }

    inline void RemoveTag(const string& MainKey,const string& Tag)
    {
        if(TagIndex==nullptr)throw error("this StorageManger has no TagIndex");
        int remove_id=MainIndex->find(MainKey)[0];
        TagIndex->remove(Tag,remove_id);
    }

    inline void RemoveTag(const string& MainKey,const vector<string>& TagList)
    {
        if(TagIndex==nullptr)throw error("this StorageManger has no TagIndex");
        int remove_id=MainIndex->find(MainKey);
        for(auto Tag:TagList)
        {
            TagIndex->remove(Tag,remove_id);
        }
    }

    inline void AddTag(const string& MainKey,const string& Tag)
    {
        if(TagIndex==nullptr)throw error("this StorageManger has no TagIndex");
        int find_id=MainIndex->find(MainKey)[0];
        TagIndex->insert(Tag,find_id);
    }

    inline vector<T> FindByTag(const string& Tag)
    {
        if(TagIndex==nullptr)throw error("this StorageManger has no TagIndex");
        vector<int>id_list=TagIndex->find(Tag);
        vector<T>ansList;
        for(auto it:id_list)
        {
            ansList.push_back(dataLibrary->get(it));
        }
        return ansList;
    }

    inline std::pair<T,bool> FindByKey(const string& MainKey)
    {
        vector<int> ans_list=MainIndex->find(MainKey);
        if(ans_list.empty())return make_pair(T(),false);
        return make_pair(dataLibrary->get(ans_list[0]),true);
    }

    inline void Update(const string& MainKey,const T& to_update)
    {
        int find_id=MainIndex->find(MainKey)[0];
        dataLibrary->update(find_id,to_update);
    }

    inline void AddTag(const string& MainKey,const vector<string>& list)
    {
        if(TagIndex==nullptr)throw error("this StorageManger has no TagIndex");
        int find_id=MainIndex->find(MainKey)[0];
        for(auto it:list)
        {
            TagIndex->insert(it,find_id);
        }
    }

    inline void clean()
    {
        dataLibrary->clearAll();
        MainIndex->clean();
        if(TagIndex!=nullptr)TagIndex->clean();
    }

    inline int read_log()
    {
        return baseMessage.log;
    }

    inline void write_log(int new_log)
    {
        baseMessage.log=new_log;
    }

    inline int log_plus()
    {
        return baseMessage.log++;
    }

    inline int plus_log()
    {
        return ++(baseMessage.log);
    }
};
#endif //BOOKSTORE_FILESTORAGE_H