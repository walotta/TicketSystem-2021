//
// Created by wzj on 2021/1/15.
//

#ifndef BOOKSTORE_BPLUSTREE_H
#define BOOKSTORE_BPLUSTREE_H

#define No_Block_Merge_off
#define Debug_Mode_off

#include <vector>
#include <cstring>
#include "StoragePool.h"
#include "ErrorMessage.h"
#include "MyString.hpp"

template<int Size,int SizeOfCache>
class BPlusTree
{
private:
    const string name;
    int root=-1;
    int head=-1;
    class assistStore
    {
    public:
        bool used=false;
        int rootStorage;
        int headStorage;
    };
    class BStore
    {
    public:
        bool IfLeaves=-1;
        int NextLeaf=-1;
        int FrontLeaf=-1;
        int storeNumber=-1;
        int storeId[Size];
        MyString storeKey[Size];
    };
    BStore rootBlock;
    StoragePool<BStore,assistStore,SizeOfCache>* storage;

    //返回是否需要裂块，需要返回裂块后的新id值
    int dp_insert(int now,MyString key, const int &id){
        BStore nowBlock=storage->get(now);
        if(nowBlock.IfLeaves)
        {
            //叶节点
            //检查重复插入
            int check_i=0;
            for(check_i=0;check_i<nowBlock.storeNumber;check_i++)
            {
                if(key<nowBlock.storeKey[check_i])break;
            }
            if(check_i!=0&&nowBlock.storeKey[check_i-1]==key)
            {
                for(int j=check_i-1;j>=0;j--)
                {
                    if(nowBlock.storeKey[j]!=key)break;
                    if(nowBlock.storeId[j]==id)throw error("BptInsertRepeat");
                }
                if(nowBlock.storeKey[0]==key&&nowBlock.FrontLeaf!=-1)
                {
                    BStore check=storage->get(nowBlock.FrontLeaf);
                    bool finish=false;
                    while(true)
                    {
                        for(int j=check.storeNumber-1;j>=0;j--)
                        {
                            if(check.storeKey[j]!=key)
                            {
                                finish=true;
                                break;
                            }
                            if(check.storeId[j]==id)throw error("BptInsertRepeat");
                        }
                        if(check.FrontLeaf==-1||finish)break;
                        check=storage->get(check.FrontLeaf);
                    }
                }
            }


            if(nowBlock.storeNumber<Size)
            {
                int i=0;
                for(i=0;i<nowBlock.storeNumber;i++)
                {
                    if(key<nowBlock.storeKey[i])break;
                }



                for(int j=nowBlock.storeNumber;j>i;j--)
                {
                    nowBlock.storeKey[j]=nowBlock.storeKey[j-1];
                    nowBlock.storeId[j]=nowBlock.storeId[j-1];
                }
                nowBlock.storeKey[i]=key;
                nowBlock.storeId[i]=id;
                nowBlock.storeNumber++;
                storage->update(now,nowBlock);
                if(now==root)rootBlock=nowBlock;
                return -1;
            }else
            {
                int numberOfNew=Size/2;
                BStore new_Store;
                new_Store.IfLeaves=true;
                new_Store.storeNumber=numberOfNew;
                for(int i=0;i<numberOfNew;i++)
                {
                    new_Store.storeKey[i]=nowBlock.storeKey[i];
                    new_Store.storeId[i]=nowBlock.storeId[i];
                }
                for(int i=numberOfNew;i<nowBlock.storeNumber;i++)
                {
                    nowBlock.storeId[i-numberOfNew]=nowBlock.storeId[i];
                    nowBlock.storeKey[i-numberOfNew]=nowBlock.storeKey[i];
                }
                nowBlock.storeNumber=nowBlock.storeNumber-numberOfNew;
                for(int i=0;i<Size;i++)
                {
                    if(i<numberOfNew)
                    {
                        if(key<new_Store.storeKey[i])
                        {
                            for(int j=numberOfNew;j>i;j--)
                            {
                                new_Store.storeKey[j]=new_Store.storeKey[j-1];
                                new_Store.storeId[j]=new_Store.storeId[j-1];
                            }
                            new_Store.storeKey[i]=key;
                            new_Store.storeId[i]=id;
                            new_Store.storeNumber++;
                            break;
                        }
                    }else
                    {
                        if(key<nowBlock.storeKey[i-numberOfNew])
                        {
                            for(int j=nowBlock.storeNumber;j>i-numberOfNew;j--)
                            {
                                nowBlock.storeId[j]=nowBlock.storeId[j-1];
                                nowBlock.storeKey[j]=nowBlock.storeKey[j-1];
                            }
                            nowBlock.storeKey[i-numberOfNew]=key;
                            nowBlock.storeId[i-numberOfNew]=id;
                            nowBlock.storeNumber++;
                            break;
                        }
                        if(i==Size-1)
                        {
                            nowBlock.storeKey[i-numberOfNew+1]=key;
                            nowBlock.storeId[i-numberOfNew+1]=id;
                            nowBlock.storeNumber++;
                        }
                    }
                }
                new_Store.FrontLeaf=nowBlock.FrontLeaf;
                new_Store.NextLeaf=now;
                int newId=storage->add(new_Store);
                nowBlock.FrontLeaf=newId;
                storage->update(now,nowBlock);
                if(now==root)rootBlock=nowBlock;
                int front=new_Store.FrontLeaf;
                if(front==-1)
                {
                    head=newId;
                }else
                {
                    new_Store=storage->get(front);
                    new_Store.NextLeaf=newId;
                    storage->update(front,new_Store);
                    if(front==root)rootBlock=new_Store;
                }
                return newId;
            }
        }else
        {
            //非叶节点
            int i=0;
            for(i=0;i<nowBlock.storeNumber-1;i++)
            {
                if(key<nowBlock.storeKey[i+1])break;
            }
            int newId=dp_insert(nowBlock.storeId[i],key,id);
            if(newId==-1)
            {
                return -1;
            }else
            {
                //处理裂块
                BStore temm=storage->get(nowBlock.storeId[i]);
                MyString newKey=temm.storeKey[0];
                if(nowBlock.storeNumber<Size)
                {
                    for(int j=nowBlock.storeNumber;j>i;j--)
                    {
                        nowBlock.storeId[j]=nowBlock.storeId[j-1];
                        nowBlock.storeKey[j]=nowBlock.storeKey[j-1];
                    }
                    nowBlock.storeNumber++;
                    nowBlock.storeId[i]=newId;
                    nowBlock.storeKey[i+1]=newKey;
                    storage->update(now,nowBlock);
                    if(now==root)rootBlock=nowBlock;
                    return -1;
                }else
                {
                    int numberOfNew=Size/2;
                    //nowBlock.storeId[i]=newId;
                    BStore new_Store;
                    new_Store.IfLeaves=false;
                    new_Store.storeNumber=numberOfNew;
                    for(int j=0;j<numberOfNew;j++)
                    {
                        new_Store.storeKey[j]=nowBlock.storeKey[j];
                        new_Store.storeId[j]=nowBlock.storeId[j];
                    }
                    for(int j=numberOfNew;j<nowBlock.storeNumber;j++)
                    {
                        nowBlock.storeId[j-numberOfNew]=nowBlock.storeId[j];
                        nowBlock.storeKey[j-numberOfNew]=nowBlock.storeKey[j];
                    }
                    nowBlock.storeNumber=nowBlock.storeNumber-numberOfNew;
                    if(i<numberOfNew)
                    {
                        for(int j=numberOfNew;j>i;j--)
                        {
                            new_Store.storeId[j]=new_Store.storeId[j-1];
                            new_Store.storeKey[j]=new_Store.storeKey[j-1];
                        }
                        new_Store.storeKey[i+1]=newKey;
                        new_Store.storeId[i]=newId;
                        new_Store.storeNumber++;
                    }else
                    {
                        for(int j=nowBlock.storeNumber;j>i-numberOfNew;j--)
                        {
                            nowBlock.storeId[j]=nowBlock.storeId[j-1];
                            nowBlock.storeKey[j]=nowBlock.storeKey[j-1];
                        }
                        nowBlock.storeKey[i+1-numberOfNew]=newKey;
                        nowBlock.storeId[i-numberOfNew]=newId;
                        nowBlock.storeNumber++;
                    }
                    storage->update(now,nowBlock);
                    if(now==root)rootBlock=nowBlock;
                    int ans=storage->add(new_Store);
                    return ans;
                }
            }

        }
    }

    //搜索删除，在返回后由父节点判断是否需要合并
    void dp_remove(int now,const MyString &key, const int &id)
    {
        BStore nowBlock=storage->get(now);
        if(nowBlock.IfLeaves)
        {
            //叶子节点
            int i=0;
            for(i=0;i<nowBlock.storeNumber;i++)
            {
                if(key<nowBlock.storeKey[i])break;
            }
            i--;
            if(i==-1)
            {
                throw error("BPT remove not found");
            }
            bool finish=false;
            if(nowBlock.storeKey[i]==key)
            {
                int j;
                for(j=i;j>=0;j--)
                {
                    if(nowBlock.storeId[j]==id&&nowBlock.storeKey[j]==key)
                    {
                        finish=true;
                        break;
                    }
                }
                if(finish)
                {
                    //删除点在当前块
                    for(int k=j;k<nowBlock.storeNumber-1;k++)
                    {
                        nowBlock.storeId[k]=nowBlock.storeId[k+1];
                        nowBlock.storeKey[k]=nowBlock.storeKey[k+1];
                    }
                    nowBlock.storeNumber--;
                    storage->update(now,nowBlock);
                    if(now==root)rootBlock=nowBlock;
                }else
                {
                    //删除点在前面块
                    BStore f=storage->get(nowBlock.FrontLeaf);
                    int f_point=nowBlock.FrontLeaf;
                    while(true)
                    {
                        for(int k=f.storeNumber-1;k>=0;k--)
                        {
                            if(f.storeKey[k]!=key)throw error("BPT remove not found");
                            else
                            {
                                if(f.storeId[k]==id)
                                {
                                    for(int ii=k;ii<f.storeNumber-1;ii++)
                                    {
                                        f.storeId[ii]=f.storeId[ii+1];
                                        f.storeKey[ii]=f.storeKey[ii+1];
                                    }
                                    f.storeNumber--;
                                    storage->update(f_point,f);
                                    if(f_point==root)rootBlock=f;
                                    finish=true;
                                    break;
                                }
                            }
                        }
                        if(finish)
                        {
                            break;
                        }else if(f.FrontLeaf==-1)
                        {
                            throw error("BPT remove not found");
                        }else
                        {
                            f_point=f.FrontLeaf;
                            f=storage->get(f_point);
                        }
                    }
                }
            }else
            {
                throw error("BPT remove not found");
            }
        }else
        {
            //不是叶子节点
            int i=0;
            for(i=0;i<nowBlock.storeNumber-1;i++)
            {
                if(key<nowBlock.storeKey[i+1])break;
            }
            dp_remove(nowBlock.storeId[i],key,id);
            BStore son=storage->get(nowBlock.storeId[i]);
            nowBlock.storeKey[i]=son.storeKey[0];
            const int _min=Size/2;

#ifdef No_Block_Merge
            bptStorage->update(now,nowBlock);
            if(now==root)rootBlock=nowBlock;
            return;
#endif

            if(son.storeNumber<_min && nowBlock.storeNumber>1)
            {
                //需要扩容
                int direction=1;
                if(i==nowBlock.storeNumber-1)direction=-1;
                BStore neigh=storage->get(nowBlock.storeId[i+direction]);
                if(neigh.storeNumber<_min)
                {
                    //合并
                    BStore new_block;
                    if(son.IfLeaves)
                    {
                        //子节点是叶子节点，维护链表
                        new_block.IfLeaves=true;
                        if(direction==1)
                        {
                            //取右邻居
                            new_block.NextLeaf=neigh.NextLeaf;
                            new_block.FrontLeaf=son.FrontLeaf;
                        }else
                        {
                            //取左邻居
                            new_block.NextLeaf=son.NextLeaf;
                            new_block.FrontLeaf=neigh.FrontLeaf;
                        }
                    }else
                    {
                        new_block.IfLeaves=false;
                    }
                    //执行合并操作
                    new_block.storeNumber=son.storeNumber+neigh.storeNumber;
                    if(direction==1)
                    {
                        for(int j=0;j<new_block.storeNumber;j++)
                        {
                            if(j<son.storeNumber)
                            {
                                new_block.storeId[j]=son.storeId[j];
                                new_block.storeKey[j]=son.storeKey[j];
                            }else
                            {
                                new_block.storeId[j]=neigh.storeId[j-son.storeNumber];
                                new_block.storeKey[j]=neigh.storeKey[j-son.storeNumber];
                            }
                        }
                    }else
                    {
                        for(int j=0;j<new_block.storeNumber;j++)
                        {
                            if(j<neigh.storeNumber)
                            {
                                new_block.storeId[j]=neigh.storeId[j];
                                new_block.storeKey[j]=neigh.storeKey[j];
                            }else
                            {
                                new_block.storeId[j]=son.storeId[j-neigh.storeNumber];
                                new_block.storeKey[j]=son.storeKey[j-neigh.storeNumber];
                            }
                        }
                    }
                    storage->remove(nowBlock.storeId[i]);
                    storage->remove(nowBlock.storeId[i+direction]);
                    int newId=storage->add(new_block);
                    if(direction==1)
                    {
                        for(int j=i+2;j<nowBlock.storeNumber;j++)
                        {
                            nowBlock.storeKey[j-1]=nowBlock.storeKey[j];
                            nowBlock.storeId[j-1]=nowBlock.storeId[j];
                        }
                        nowBlock.storeKey[i]=new_block.storeKey[0];
                        nowBlock.storeId[i]=newId;
                        nowBlock.storeNumber--;
                    }else
                    {
                        nowBlock.storeKey[i-1]=new_block.storeKey[0];
                        nowBlock.storeId[i-1]=newId;
                        nowBlock.storeNumber--;
                    }
                    //修改相邻元素的链表结构
                    if(new_block.IfLeaves)
                    {
                        if(new_block.FrontLeaf==-1)
                        {
                            head=newId;
                        }else
                        {
                            BStore tem=storage->get(new_block.FrontLeaf);
                            tem.NextLeaf=newId;
                            storage->update(new_block.FrontLeaf,tem);
                            if(new_block.FrontLeaf==root)rootBlock=tem;
                        }
                        if(new_block.NextLeaf!=-1)
                        {
                            BStore tem=storage->get(new_block.NextLeaf);
                            tem.FrontLeaf=newId;
                            storage->update(new_block.NextLeaf,tem);
                            if(new_block.NextLeaf==root)rootBlock=tem;
                        }
                    }
                }else
                {
                    //领养
                    if(direction==1)
                    {
                        son.storeId[son.storeNumber]=neigh.storeId[0];
                        son.storeKey[son.storeNumber]=neigh.storeKey[0];
                        son.storeNumber++;
                        for(int j=1;j<neigh.storeNumber;j++)
                        {
                            neigh.storeKey[j-1]=neigh.storeKey[j];
                            neigh.storeId[j-1]=neigh.storeId[j];
                        }
                        neigh.storeNumber--;
                        nowBlock.storeKey[i+direction]=neigh.storeKey[0];
                    }else
                    {
                        for(int j=son.storeNumber;j>0;j--)
                        {
                            son.storeKey[j]=son.storeKey[j-1];
                            son.storeId[j]=son.storeId[j-1];
                        }
                        son.storeNumber++;
                        son.storeKey[0]=neigh.storeKey[neigh.storeNumber-1];
                        son.storeId[0]=neigh.storeId[neigh.storeNumber-1];
                        neigh.storeNumber--;
                        nowBlock.storeKey[i]=son.storeKey[0];
                    }
                    storage->update(nowBlock.storeId[i],son);
                    storage->update(nowBlock.storeId[i+direction],neigh);
                }
            }
            storage->update(now,nowBlock);
            if(now==root)rootBlock=nowBlock;
        }
    }

    std::vector<int> dp_find(int now,const MyString &key)const
    {
        BStore nowBlock=storage->get(now);
        if(nowBlock.IfLeaves)
        {
            int i=0;
            for(i=0;i<nowBlock.storeNumber;i++)
            {
                if(key<nowBlock.storeKey[i])break;
            }
            i--;
            if(i==-1)
            {
                vector<int> ans;
                return ans;
            }
            if(nowBlock.storeKey[i]==key)
            {
                vector<int> ans;
                ans.push_back(nowBlock.storeId[i]);
                for(int j=i-1;j>=0;j--)
                {
                    if(nowBlock.storeKey[j]==key)ans.push_back(nowBlock.storeId[j]);
                    else return ans;
                }
                BStore find=storage->get(nowBlock.FrontLeaf);
                while(true)
                {
                    for(int j=find.storeNumber-1;j>=0;j--)
                    {
                        if(find.storeKey[j]==key)ans.push_back(find.storeId[j]);
                        else return ans;
                    }
                    if(find.FrontLeaf==-1)return ans;
                    else find=storage->get(find.FrontLeaf);
                }
            }else
            {
                vector<int> ans;
                return ans;
            }
        }else
        {
            int i=0;
            for(i=0;i<nowBlock.storeNumber-1;i++)
            {
                if(key<nowBlock.storeKey[i+1])break;
            }
            return dp_find(nowBlock.storeId[i],key);
        }
    }

public:

    BPlusTree()=delete;
    explicit BPlusTree(const string& _name):name(_name)
    {
        string headString="BptOf_";
        storage=new StoragePool<BStore,assistStore,SizeOfCache>(headString+name);
        assistStore ass;
        ass=storage->readExtraBlock();
        if(!ass.used)
        {
            root=-1;
            head=-1;
            ass.rootStorage=root;
            ass.headStorage=head;
            ass.used=true;
            storage->writeExtraBlock(ass);
        }else
        {
            root=ass.rootStorage;
            head=ass.headStorage;
            rootBlock=storage->get(root);
        }
    }

    ~BPlusTree()
    {
        assistStore ass;
        ass.headStorage=head;
        ass.rootStorage=root;
        ass.used=true;
        storage->writeExtraBlock(ass);
        delete storage;
    }

    void insert(const string &key, const int &id)
    {
        if(root==-1)
        {
            //当前无数据
            BStore tem;
            tem.FrontLeaf=-1;
            tem.NextLeaf=-1;
            tem.IfLeaves=true;
            tem.storeNumber=1;
            tem.storeId[0]=id;
            tem.storeKey[0]=key;
            root=storage->add(tem);
            head=root;
            rootBlock=tem;
        }else
        {
            //当前有数据
            int newId=dp_insert(root,key,id);
            if(newId==-1)return;
            else
            {
                /*if(rootBlock.IfLeaves)
                {
                    //仅有一个节点的裂块
                    BStore new_head;
                    BStore temm=bptStorage->get(newId);
                    new_head.IfLeaves=false;
                    new_head.storeNumber=2;
                    new_head.storeKey[0]=temm.storeKey[0];
                    new_head.storeId[0]=newId;
                    new_head.storeKey[1]=rootBlock.storeKey[0];
                    new_head.storeId[1]=root;
                    root=bptStorage->add(new_head);
                    rootBlock=new_head;
                }else
                {
                    //有多个节点的裂块
                    if(rootBlock.storeNumber<Size)
                    {
                        int i=0;
                        for(i=0;i<rootBlock.storeNumber;i++)
                        {
                            if(key<rootBlock.storeKey[i])break;
                        }
                        for(int j=rootBlock.storeNumber;j>i;j--)
                        {
                            rootBlock.storeId[j]=rootBlock.storeId[j-1];
                            rootBlock.storeKey[j]=rootBlock.storeKey[j-1];
                        }
                        rootBlock.storeNumber++;
                        rootBlock.storeId[i]=newId;
                        BStore temm=bptStorage->get(rootBlock.storeId[i+1]);
                        rootBlock.storeKey[i+1]=temm.storeKey[0];
                        bptStorage->update(root,rootBlock);
                        return;
                    }else
                    {
                        int i=0;
                        for(i=0;i<rootBlock.storeNumber-1;i++)
                        {
                            if(key<rootBlock.storeKey[i+1])break;
                        }
                        BStore temm=bptStorage->get(rootBlock.storeId[i]);
                        MyString newKey=temm.storeKey[0];
                        int numberOfNew=Size/2;
                        BStore new_Store;
                        new_Store.IfLeaves=false;
                        new_Store.storeNumber=numberOfNew;
                        for(int j=0;j<numberOfNew;j++)
                        {
                            new_Store.storeKey[j]=rootBlock.storeKey[j];
                            new_Store.storeId[j]=rootBlock.storeId[j];
                        }
                        for(int j=numberOfNew;j<rootBlock.storeNumber;j++)
                        {
                            rootBlock.storeId[j-numberOfNew]=rootBlock.storeId[j];
                            rootBlock.storeKey[j-numberOfNew]=rootBlock.storeKey[j];
                        }
                        rootBlock.storeNumber=rootBlock.storeNumber-numberOfNew;

                        bptStorage->update(root,rootBlock);
                        int ans=bptStorage->add(new_Store);
                        BStore new_root;
                        new_root.storeNumber=2;
                        new_root.IfLeaves=false;
                        new_root.storeId[0]=ans;
                        new_root.storeKey[0]=new_Store.storeKey[0];
                        new_root.storeKey[1]=rootBlock.storeKey[0];
                        new_root.storeId[1]=root;
                        root=bptStorage->add(new_root);
                        rootBlock=new_root;
                    }
                }*/
                BStore new_head;
                BStore temm=storage->get(newId);
                new_head.IfLeaves=false;
                new_head.storeNumber=2;
                new_head.storeKey[0]=temm.storeKey[0];
                new_head.storeId[0]=newId;
                new_head.storeKey[1]=rootBlock.storeKey[0];
                new_head.storeId[1]=root;
                root=storage->add(new_head);
                rootBlock=new_head;
            }
        }
    }

    void remove(const string &key, const int &id)
    {
        //不完全，对于重复key的块合并不完整
        /*bool check_flag=false;
        vector<int> check;
        check=find(key);
        for(auto it:check)
        {
            if(it==id)
            {
                check_flag=true;
                break;
            }
        }
        if(!check_flag)throw error("BPT remove not found");*/

        if(rootBlock.IfLeaves)
        {
            //根节点是叶子节点
            for(int i=0;i<rootBlock.storeNumber;i++)
            {
                if(rootBlock.storeId[i]==id&&rootBlock.storeKey[i]==key)
                {
                    for(int j=i+1;j<rootBlock.storeNumber;j++)
                    {
                        rootBlock.storeKey[j-1]=rootBlock.storeKey[j];
                        rootBlock.storeId[j-1]=rootBlock.storeId[j];
                    }
                    rootBlock.storeNumber--;
                    storage->update(root,rootBlock);
                    return;
                }
            }
            throw error("BPT remove not found");
        }else
        {
            //树高度高于1
            dp_remove(root, key, id);
            if(rootBlock.storeNumber==1)
            {
                int new_root=rootBlock.storeId[0];
                storage->remove(root);
                root=new_root;
                rootBlock=storage->get(root);
            }
        }
    }

    std::vector<int> find(const string &key)const
    {
        if(root==-1)
        {
            vector<int> ans;
            return ans;
        }else
        {
            return dp_find(root,key);
        }
    }

    std::vector<int> giveAllStorage()const
    {
        vector<int> ans;
        BStore tem=storage->get(head);
        while(true)
        {
            for(int i=0;i<tem.storeNumber;i++)
            {
                ans.push_back(tem.storeId[i]);
            }
            if(tem.NextLeaf==-1)break;
            else tem=storage->get(tem.NextLeaf);
        }
        return ans;
    }

#ifdef Debug_Mode
    void debug()
    {
        cout<<endl<<"[debug]"<<endl;
        int k=head;
        cout<<"head:"<<head<<endl;
        cout<<"root:"<<root<<endl<<endl;
        while(k!=-1)
        {
            BStore KS=storage->get(k);
            cout<<k<<' '<<KS.storeNumber<<endl;
            cout<<"Key:"<<' ';
            for(int i=0;i<KS.storeNumber;i++)
            {
                cout<<KS.storeKey[i]<<' ';
            }
            cout<<endl;
            cout<<"Id :"<<' ';
            for(int i=0;i<KS.storeNumber;i++)
            {
                cout<<KS.storeId[i]<<' ';
            }
            cout<<endl;
            cout<<"NextLeaf   "<<KS.NextLeaf<<endl;
            cout<<"FrontLeaf  "<<KS.FrontLeaf<<endl;
            cout<<endl;
            k=KS.NextLeaf;
        }
        cout<<"[debug finish]"<<endl;
    }

    void debug(int id)
    {
        cout<<"["<<id<<" block]"<<endl;
        BStore KS=storage->get(id);
        cout<<id<<' '<<KS.storeNumber<<endl;
        cout<<"Key:"<<' ';
        for(int i=0;i<KS.storeNumber;i++)
        {
            cout<<KS.storeKey[i]<<' ';
        }
        cout<<endl;
        cout<<"Id :"<<' ';
        for(int i=0;i<KS.storeNumber;i++)
        {
            cout<<KS.storeId[i]<<' ';
        }
        cout<<endl;
        cout<<endl;
        cout<<"["<<id<<" block finished]"<<endl;
    }

    void debugRoot()
    {
        cout<<"[root block]"<<endl;
        BStore KS=storage->get(root);
        cout<<root<<' '<<KS.storeNumber<<endl;
        cout<<"Key:"<<' ';
        for(int i=0;i<KS.storeNumber;i++)
        {
            cout<<KS.storeKey[i]<<' ';
        }
        cout<<endl;
        cout<<"Id :"<<' ';
        for(int i=0;i<KS.storeNumber;i++)
        {
            cout<<KS.storeId[i]<<' ';
        }
        cout<<endl;
        cout<<endl;
        cout<<"[root block finished]"<<endl;
    }

#endif
};
#endif //BOOKSTORE_BPLUSTREE_H
