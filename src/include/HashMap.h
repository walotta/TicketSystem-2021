//
// Created by wzj on 2021/3/26.
// Edit by Fourest on 2021.06.04
//

#ifndef CODE_HASHMAP_H
#define CODE_HASHMAP_H

#include <string>
#include <functional>

/*
template<typename key_type,typename value_type,class Hash=std::hash<key_type>>
class HashMap
{
private:
    typedef unsigned long long unsigned_lint;

    static const unsigned_lint prime_number=30;
    const unsigned_lint prime_list[prime_number]={
        53,101,211,401,809,1601,3203,6421,12809,25601,51203,102407,204803,409609,//14
        819229,1638431,3276803,6553621,13107229,26214401ll,52428841ll,104857601ll,209715263ll,//9
        419430419ll,838860817ll,1677721631ll,3355443229ll,6710886407ll,13421772823ll,26843545607ll//7
    };

    class LinkList
    {
    public:
        struct Node
        {
            key_type *key;
            value_type *value;
            Node *next;

            Node(const key_type &k,const value_type &v,Node *n):key(k),value(v),next(n){}
            ~Node(){delete key; delete value;}
        };

        Node *head;
        unsigned_lint length;

    public:
        LinkList():head(nullptr),length(0){}
        ~LinkList()
        {
            while(head!=nullptr)
            {
                auto temp=head;
                head=head->next;
                delete temp;
            }
        }

        void insert(const key_type &k,const value_type &v)
        {
            Node *new_head=new Node(k,v,head);
            head=new_head;
            ++length;
        }
        void erase(const key_type &k)
        {
            if(*head->key==k)
            {
                auto temp=head;
                head=head->next;
                delete temp;
                --length;
                return;
            }
            auto now=head;
            while(now->next!=nullptr)
            {
                if(*now->next->key==k)
                {
                    auto temp=now->next;
                    now->next=temp->next;
                    delete temp;
                    --length;
                    return;
                }
                now=now->next;
            }
        }
        value_type &find(const key_type &k)
        {
            auto now=head;
            while(*now->key!=k && now->next!=nullptr) now=now->next;
            if(*now->key==k) return *now->value;
            return value_type();
        }

        int size() const {return length;}
        bool empty() const {return length==0;}
        void add_node(Node *node)
        {
            node->next=head;
            head=node;
            ++length;
        }
    };
    using node_t=typename LinkList::Node;

    unsigned_lint prime_pos_now=0;
    unsigned_lint length=0;
    LinkList *lists;
    Hash hash;

    inline unsigned_lint get_index(const key_type &k) const
    {
        return hash(k)%prime_list[prime_pos_now];
    }

    void resize()
    {
        LinkList *new_list=new LinkList[prime_list[++prime_pos_now]];
        for(int i=0;i<prime_list[prime_pos_now-1];++i)
        {
//            node_t *now=lists[i].head;
//            while(now!=nullptr)
//            {
//                auto index=get_index(n)
//            }
        }



    }







public:
    HashMap();
    ~HashMap()=default;



};
*/

#endif //CODE_HASHMAP_H
