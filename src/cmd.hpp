//
// Created by walotta on 2021/5/14.
//

#include "ManagementSystem.h"
#include "include/separator.h"
#include <sstream>

class cmd
{
private:
    ManagementSystem sys;
    Fourest::separator words;

    void to_vector_int(str input,vector<int> &out)
    {
        Fourest::separator ints("|");
        ints.process(input);
        for(int i=0;i<ints.size();++i) out.push_back(to_int(ints[i]));
    }
    void to_vector_str(str input,vector<string> &out)
    {
        Fourest::separator ints("|");
        ints.process(input);
        out=ints.content();
    }

    template<class T>
    void print_os(const vector<T> &list,ostream &ost)
    {
        int size=list.size();
        for(int i=0; i<size; i++)
        {
            ost<<list[i];
            if(i!=size-1) ost<<'\n';
        }
    }
public:
    cmd():words(" "){}
    void run(std::istream &is, std::ostream &os)
    {
        words.readLine(is);
        do
        {
            if(words.empty()) continue;
            auto &command=words[0];
            if(command=="add_user")
            {
                string c(""),u(""),p(""),n(""),m("");
                int g=-404;
                for(int i=1; i<words.size(); i+=2)
                {
                    if(words[i]=="-c") c=words[i+1];
                    else if(words[i]=="-u") u=words[i+1];
                    else if(words[i]=="-p") p=words[i+1];
                    else if(words[i]=="-n") n=words[i+1];
                    else if(words[i]=="-m") m=words[i+1];
                    else if(words[i]=="-g") g=to_int(words[i+1]);
                }
                os<<sys.add_user(c,u,p,n,m,g)-1;
            }
            else if(command=="login")
            {
                string u(""),p("");
                for(int i=1; i<words.size(); i+=2)
                {
                    if(words[i]=="-u") u=words[i+1];
                    else p=words[i+1];
                }
                os<<sys.login(u,p)-1;
            }
            else if(command=="logout")
            {
                os<<sys.logout(words[2])-1;
            }
            else if(command=="query_profile")
            {
                string c(""),u("");
                for(int i=1; i<words.size(); i+=2)
                {
                    if(words[i]=="-c") c=words[i+1];
                    else u=words[i+1];
                }
                os<<sys.query_profile(c,u);
            }
            else if(command=="modify_profile")
            {
                string c(""),u(""),p(""),n(""),m("");
                int g=-404;
                for(int i=1; i<words.size(); i+=2)
                {
                    if(words[i]=="-c") c=words[i+1];
                    else if(words[i]=="-u") u=words[i+1];
                    else if(words[i]=="-p") p=words[i+1];
                    else if(words[i]=="-n") n=words[i+1];
                    else if(words[i]=="-m") m=words[i+1];
                    else if(words[i]=="-g") g=to_int(words[i+1]);
                }
                os<<sys.modify_profile(c,u,p,n,m,g);
            }
            else if(command=="add_train")
            {
                string i;
                int n,m;
                Date d_beg,d_end;
                Time x;
                char y;
                vector<string> s;
                vector<int> p,t,o;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-i") i=words[j+1];
                    else if(words[j]=="-n") n=to_int(words[j+1]);
                    else if(words[j]=="-m") m=to_int(words[j+1]);
                    else if(words[j]=="-s") to_vector_str(words[j+1],s);
                    else if(words[j]=="-p") to_vector_int(words[j+1],p);
                    else if(words[j]=="-x") x=Time(words[j+1]);
                    else if(words[j]=="-t") to_vector_int(words[j+1],t);
                    else if(words[j]=="-o") {if(n>2) to_vector_int(words[j+1],o);}
                    else if(words[j]=="-d")
                    {
                        string &day=words[j+1];
                        d_beg=Date(day.substr(0,5));
                        d_end=Date(day.substr(6,5));
                    }
                    else y=words[j+1][0];
                }
                os<<sys.add_train(i,n,m,s,p,x,t,o,d_beg,d_end,y)-1;
            }
            else if(command=="release_train")
            {
                os<<sys.release_train(words[2])-1;
            }
            else if(command=="query_train")
            {
                string i;
                Date d;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-i") i=words[j+1];
                    else d=Date(words[j+1]);
                }
                static vecS ans;
                ans.clear();
                sys.query_train(i,d,ans);
                print_os(ans,os);
            }
            else if(command=="delete_train")
            {
                os<<sys.delete_train(words[2])-1;
            }
            else if(command=="query_ticket")
            {
                string s,t;
                Date d;
                bool p=true;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-s") s=words[j+1];
                    else if(words[j]=="-t") t=words[j+1];
                    else if(words[j]=="-d") d=Date(words[j+1]);
                    else if(words[j]=="-p")
                    {
                        if(words[j+1]=="time") p=true;
                        else p=false;
                    }
                }
                static vecS ans;
                ans.clear();
                sys.query_ticket(s,t,d,p,ans);
                print_os(ans,os);
            }
            else if(command=="query_transfer")
            {
                string s,t;
                Date d;
                bool p=true;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-s") s=words[j+1];
                    else if(words[j]=="-t") t=words[j+1];
                    else if(words[j]=="-d") d=Date(words[j+1]);
                    else if(words[j]=="-p")
                    {
                        if(words[j+1]=="time") p=true;
                        else p=false;
                    }
                }
                static vecS ans;
                ans.clear();
                sys.query_transfer(s,t,d,p,ans);
                print_os(ans,os);
            }
            else if(command=="buy_ticket")
            {
                string u,i,f,t;
                Date d;
                int n;
                bool q=false;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-u") u=words[j+1];
                    else if(words[j]=="-i") i=words[j+1];
                    else if(words[j]=="-d") d=Date(words[j+1]);
                    else if(words[j]=="-n") n=to_int(words[j+1]);
                    else if(words[j]=="-f") f=words[j+1];
                    else if(words[j]=="-t") t=words[j+1];
                    else if(words[j]=="-q")
                    {
                        if(words[j+1]=="false") q=false;
                        else q=true;
                    }
                }
                os<<sys.buy_ticket(u,i,d,f,t,n,q);
            }
            else if(command=="query_order")
            {
                string u("");
                if(words[1]=="-u") u=words[2];
                static vecS ans;
                ans.clear();
                sys.query_order(u,ans);
                print_os(ans,os);
            }
            else if(command=="refund_ticket")
            {
                string u;
                int n=1;
                for(int j=1; j<words.size(); j+=2)
                {
                    if(words[j]=="-u") u=words[j+1];
                    if(words[j]=="-n") n=to_int(words[j+1]);
                }
                os<<sys.refund_ticket(u,n)-1;
            }
            else if(command=="clean")
            {
                sys.clean();
                os<<0;
            }
            else if(command=="exit")
            {
                os<<"bye"<<endl;
                os.flush();
                break;
            }
            os<<"\n";
        } while(words.readLine(is));
    }
};