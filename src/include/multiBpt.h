//
// Created by wzj on 2021/5/12.
//

#ifndef BPT_MULTIBPT_H
#define BPT_MULTIBPT_H

#include "BPlusTree.h"

template<int Size_of_bpt,int Size_of_blockList>
class multiBPlusTree
{
private:
    class _blockList
    {
        class dataBlock
        {
            int storage[Size_of_blockList]={0};
        public:
            int cnt=0;
            int next=-1;
            int front=-1;
            inline void insert(int x)
            {
                storage[cnt]=x;
                cnt++;
            }
            inline void remove(int pos)
            {
                for(int i=pos;i<cnt;i++)
                    storage[i]=storage[i+1];
                cnt--;
            }
            inline int& operator[](int pos){return storage[pos];}
        };
        struct assistBlock
        {
            int x;
        };
        StoragePool<dataBlock,assistBlock>* BlockList;
        const string name;
    public:
        _blockList()=delete;
        explicit _blockList(const string& _name):name(_name)
        {
            string headString="multiBptBlockList_";
            BlockList=new StoragePool<dataBlock,assistBlock>(headString+name);
        }
        //建立一个新的块链并插入x返回新块链对应的id
        inline int add_new_list(int x)
        {
            dataBlock tem;
            tem.insert(x);
            return BlockList->add(tem);
        }

        //根据list_id返回块链的全部数据
        inline std::vector<int> give_list(int list_id)
        {
            vector<int> ans;
            while(list_id!=-1)
            {
                dataBlock tem=BlockList->get(list_id);
                for(int i=0;i<tem.cnt;i++)
                    ans.push_back(tem[i]);
                list_id=tem.next;
            }
            return ans;
        }

        //删除list_id对应的块链中的数据x,若块链全空返回true
        bool remove(int list_id,int x)
        {
            int pos=-1;
            dataBlock tem=BlockList->get(list_id);
            while(list_id!=-1)
            {
                for(int i=0;i<tem.cnt;i++)
                {
                    if(tem[i]==x)
                    {
                        tem.remove(i);
                        pos=i;
                        break;
                    }
                }
                if(pos!=-1)break;
                list_id=tem.next;
                tem=BlockList->get(list_id);
            }
            if(tem.cnt==0)
            {
                if(tem.front==-1&&tem.next==-1)
                {
                    BlockList->remove(list_id);
                    return true;
                }
                else if(tem.front!=-1)
                {
                    dataBlock fr=BlockList->get(tem.front);
                    fr.next=tem.next;
                    BlockList->update(tem.front,fr);
                    if(tem.next!=-1)
                    {
                        dataBlock ne=BlockList->get(tem.next);
                        ne.front=tem.front;
                        BlockList->update(tem.next,ne);
                    }
                    BlockList->remove(list_id);
                }else
                {
                    dataBlock ne=BlockList->get(tem.next);
                    for(int i=0;i<ne.cnt;i++)tem.insert(ne[i]);
                    int next_id=ne.next;
                    BlockList->remove(tem.next);
                    tem.next=next_id;
                    if(next_id!=-1)
                    {
                        ne=BlockList->get(tem.next);
                        ne.front=list_id;
                        BlockList->update(tem.next,ne);
                    }
                    BlockList->update(list_id,tem);
                }
            }else
                BlockList->update(list_id,tem);
            return false;
        }

        //向list_id对应的块链中插入数据x
        void insert(int list_id,int x)
        {
            dataBlock now=BlockList->get(list_id);
            while(now.next!=-1)
            {
                list_id=now.next;
                now=BlockList->get(list_id);
            }
            if(now.cnt!=Size_of_blockList)
            {
                now.insert(x);
                BlockList->update(list_id,now);
            }else
            {
                dataBlock tem;
                tem.insert(x);
                tem.front=list_id;
                now.next=BlockList->add(tem);
                BlockList->update(list_id,now);
            }
        }
    };
    BPlusTree<Size_of_bpt>* index;
    _blockList* BlockList;
    const string name;
public:
    multiBPlusTree()=delete;
    explicit multiBPlusTree(const string& _name):name(_name)
    {
        string headString="multi_";
        index=new BPlusTree<Size_of_bpt>(headString+name);
        BlockList=new _blockList(name);
    }

    ~multiBPlusTree()
    {
        delete index;
        delete BlockList;
    }

    std::vector<int> find(const string &key)const
    {
        return BlockList->give_list(index->find(key)[0]);
    }

    void remove(const string &key, const int &id)
    {
        int list_id=index->find(key)[0];
        bool empty=BlockList->remove(list_id,id);
        if(empty)
        {
            index->remove(key,list_id);
        }
    }

    void insert(const string &key, const int &id)
    {
        vector<int> tem=index->find(key);
        if(tem.empty())
        {
            int list_id=BlockList->add_new_list(id);
            index->insert(key,list_id);
        }else
        {
            BlockList->insert(tem[0],id);
        }
    }
};

#endif //BPT_MULTIBPT_H
