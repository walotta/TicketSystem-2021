//
// Created by wzj on 2021/1/18.
//

#ifndef BOOKSTORE_FILESTORAGE_H
#define BOOKSTORE_FILESTORAGE_H

#define Bpt_Size 300

#include "BPlusTree.h"
#include "StoragePool.h"
#include "ErrorMessage.h"
#include <map>
using namespace std;

template<class T>
class FileStorage
{
private:
    class IsEmpty;
    BPlusTree<Bpt_Size>* bptStorage[6];
    StoragePool<T,IsEmpty>* dataStorage;
    int AmountOfKey=0;
    const string nameOfFile;
    map<string,int> keyMap;

    int nowId=-1;
    T nowBlock;
    string nowMainKey;
    class IsEmpty
    {
    private:
        bool a;
    public:
        IsEmpty(){a=true;}
        void activate(){a=false;}
        bool check(){return a;}
    }emptyFlag;

public:

    FileStorage()=delete;
    FileStorage(const vector<string> &o,const string& Name):nameOfFile(Name)
    {
        if(o.empty()||o.size()>6)throw error("Wrong key number in FileStorage");
        AmountOfKey=o.size();
        for(int i=0;i<AmountOfKey;i++)
        {
            const string tem_string=o[i];
            bptStorage[i]=new BPlusTree<Bpt_Size>(nameOfFile+"_"+tem_string);
            keyMap[tem_string]=i;
        }
        dataStorage=new StoragePool<T,IsEmpty>("FileStorage_"+nameOfFile);
        emptyFlag=dataStorage->readExtraBlock();
    }
    ~FileStorage()
    {
        for(int i=0;i<AmountOfKey;i++)
        {
            delete bptStorage[i];
        }
        emptyFlag.activate();
        dataStorage->writeExtraBlock(emptyFlag);
        delete dataStorage;
    }

    bool empty()
    {
        return emptyFlag.check();
    }

    void Insert(const T &o,const string &mainKey)
    {
        if(bptStorage[0]->find(mainKey).empty())
        {
            int newId=dataStorage->add(o);
            bptStorage[0]->insert(mainKey,newId);
            nowId=newId;
            nowMainKey=mainKey;
            nowBlock=o;
        }else
        {
            throw error("repeat mainKey");
        }
    }

    void Delete(const string &mainKey)
    {
        vector<int> id_vector=bptStorage[0]->find(mainKey);
        if(id_vector.size()!=1)throw error("File delete number wrong");
        bptStorage[0]->remove(mainKey,id_vector[0]);
        dataStorage->remove(id_vector[0]);
        if(mainKey==nowMainKey)nowId=-1;
    }

    vector<T> Find(const string &KeyType,const string &key)
    {
        vector<int> ans_id;
        vector<T> ans;
        ans_id=bptStorage[keyMap[KeyType]]->find(key);
        for(auto it:ans_id)
        {
            ans.push_back(dataStorage->get(it));
        }
        return ans;
    }

    T Give(const string &MainKey)
    {
        vector<int> ans=bptStorage[0]->find(MainKey);
        if(ans.empty())throw error("Do not find mainKey in"+nameOfFile);
        int id=ans[0];
        return dataStorage->get(id);
    }

    T Give(int id)
    {
        return dataStorage->get(id);
    }

    T Give()
    {
        if(nowId==-1)throw error("Do not Select in"+nameOfFile);
        return nowBlock;
    }

    string GiveMainKey()
    {
        if(nowId==-1)throw error("Do not Select in"+nameOfFile);
        return nowMainKey;
    }

    int Select(const string &MainKey)
    {
        vector<int> ans=bptStorage[0]->find(MainKey);
        if(ans.empty())throw error("Do not find mainKey in"+nameOfFile);
        nowId=ans[0];
        nowBlock=dataStorage->get(nowId);
        nowMainKey=MainKey;
        return nowId;
    }

    void Select(int select_id,const string &MainKey)
    {
        nowId=select_id;
        nowBlock=dataStorage->get(nowId);
        nowMainKey=MainKey;
    }

    void Update(const T &o,const string &MainKey)
    {
        if(nowId==-1)throw error("Do not Select in"+nameOfFile);
        nowBlock=o;
        if(MainKey!=nowMainKey)
        {
            if(bptStorage[0]->find(MainKey).empty())
            {
                dataStorage->update(nowId,o);
                bptStorage[0]->remove(nowMainKey,nowId);
                bptStorage[0]->insert(MainKey,nowId);
                nowMainKey=MainKey;
            }else
            {
                throw error("repeat mainKey");
            }
        }else
        {
            dataStorage->update(nowId,o);
        }
    }

    void Update(const T &o)
    {
        if(nowId==-1)throw error("Do not Select in "+nameOfFile);
        nowBlock=o;
        dataStorage->update(nowId,o);
    }

    void AddKey(const string &KeyType,const string &key)
    {
        if(nowId==-1)throw error("Do not Select in"+nameOfFile);
        if(keyMap[KeyType]==0)throw error("Can not change MainKey");
        try
        {
            bptStorage[keyMap[KeyType]]->insert(key,nowId);
        }catch (const error &w)
        {
            if(w.get()=="BptInsertRepeat")throw error("FIleStorage add key repeat");
            else throw error(w.get());
        }
    }

    void RemoveKey(const string &KeyType,const string &key)
    {
        if(nowId==-1)throw error("Do not Select in"+nameOfFile);
        if(keyMap[KeyType]==0)throw error("Can not change MainKey");
        try
        {
            bptStorage[keyMap[KeyType]]->remove(key,nowId);
        }catch (const error &w)
        {
            if(w.get()=="BPT remove not found")throw error("FIleStorage find no remove key");
            else throw error(w.get());
        }
    }

    bool HaveSelect()
    {
        if(nowId==-1)return false;
        else return true;
    }

    vector<T> GiveAllBlock()
    {
        vector<int> tem=bptStorage[0]->giveAllStorage();
        vector<T> ans;
        for(auto it:tem)
        {
            ans.push_back(dataStorage->get(it));
        }
        return ans;
    }

    void ClearSelect()
    {
        nowId=-1;
    }
};
#endif //BOOKSTORE_FILESTORAGE_H
