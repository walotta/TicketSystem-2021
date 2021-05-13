#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <algorithm>
#include "multiBpt.h"
using namespace std;

const int keyNumber=2117568445;
const int msgNumber=121281;
const int insert_mode=100;
map<string,vector<int>> stdMap;
multiBPlusTree<320,100> bpt("test");

map<string,vector<int>>::iterator random_iterator()
{
    map<string,vector<int>>::iterator ans;
    do
    {
        string key=to_string(rand()%keyNumber);
        ans=stdMap.upper_bound(key);
        int delta=(rand()%(max(stdMap.size()/10,(unsigned long)3)));
        int tar=(rand()%2);
        if(tar==1)
        {
            for(int i=0;i<delta&&ans!=stdMap.end();i++)ans++;
        }else{
            for(int i=0;i<delta&&ans!=stdMap.begin();i++)ans--;
        }
    }while(ans==stdMap.end());
    return ans;
}

void insert()
{
    try
    {
        string key=to_string(rand()%keyNumber);
        int insert_number=rand()%insert_mode;
        //insert_number=1;
        //if(stdMap.count(key))continue;
        for(int i=0;i<insert_number;i++)
        {
            int msg=rand()%msgNumber;
            stdMap[key].push_back(msg);
            bpt.insert(key,msg);
        }
    }
    catch(...)
    {
        cout<<"error in insert"<<endl;
        exit(0);
    }
}

void remove()
{
    try
    {
        auto random_it=random_iterator();
        string key=random_it->first;
        vector<int> val_vector=random_it->second;
        int val=val_vector[rand()%val_vector.size()];
        bpt.remove(key,val);
        if(val_vector.size()==1)stdMap.erase(random_it);
        else
        {
            for(int i=0;i<val_vector.size();i++)
            {
                if(val_vector[i]==val)
                {
                    stdMap[key].erase(random_it->second.begin()+i);
                    break;
                }
            }
        }
    }
    catch(...)
    {
        cout<<"error in remove"<<endl;
        exit(0);
    }
}

void find()
{
    try
    {
        auto random_it=random_iterator();
        vector<int> std_ans=random_it->second;
        vector<int> my_ans=bpt.find(random_it->first);
        sort(std_ans.begin(),std_ans.end());
        sort(my_ans.begin(),my_ans.end());
        if(std_ans!=my_ans)
        {
            cout<<"find not fit"<<endl;
            exit(0);
        }
    }
    catch(...)
    {
        cout<<"error in find"<<endl;
        exit(0);
    }
}

int main()
{
    srand((unsigned)time(NULL));
    //srand(2333);
    try
    {
        for(int test_case=0;test_case<10000;test_case++)
        {
            int op=rand()%3;
            if(test_case<100)op=0;
            if(op==0)
            {
                insert();
            }else if(op==1)
            {
                remove();
            }else if(op==2)
            {
                find();
            }
        }
    }
    catch(...)
    {
        cout<<"unknown error occur!"<<endl;
        return 0;
    }
    cout<<"finish and your bpt is right!"<<endl;
    return 0;
}