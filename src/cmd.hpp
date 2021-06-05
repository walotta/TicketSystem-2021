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
            auto tokens=words.content();
            if(tokens.empty()) continue;
            auto &command=tokens[0];
            if(command=="add_user")
            {
                string c(""),u(""),p(""),n(""),m("");
                int g=-404;
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-c") c=tokens[i+1];
                    else if(tokens[i]=="-u") u=tokens[i+1];
                    else if(tokens[i]=="-p") p=tokens[i+1];
                    else if(tokens[i]=="-n") n=tokens[i+1];
                    else if(tokens[i]=="-m") m=tokens[i+1];
                    else if(tokens[i]=="-g") g=to_int(tokens[i+1]);
                }
                os<<sys.add_user(c,u,p,n,m,g)-1;
            }
            else if(command=="login")
            {
                string u(""),p("");
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-u") u=tokens[i+1];
                    else p=tokens[i+1];
                }
                os<<sys.login(u,p)-1;
            }
            else if(command=="logout")
            {
                os<<sys.logout(tokens[2])-1;
            }
            else if(command=="query_profile")
            {
                string c(""),u("");
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-c") c=tokens[i+1];
                    else u=tokens[i+1];
                }
                os<<sys.query_profile(c,u);
            }
            else if(command=="modify_profile")
            {
                string c(""),u(""),p(""),n(""),m("");
                int g=-404;
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-c") c=tokens[i+1];
                    else if(tokens[i]=="-u") u=tokens[i+1];
                    else if(tokens[i]=="-p") p=tokens[i+1];
                    else if(tokens[i]=="-n") n=tokens[i+1];
                    else if(tokens[i]=="-m") m=tokens[i+1];
                    else if(tokens[i]=="-g") g=to_int(tokens[i+1]);
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
                for(int j=1;j<tokens.size();j+=2)
                {
                    if(tokens[j]=="-i") i=tokens[j+1];
                    else if(tokens[j]=="-n") n=to_int(tokens[j+1]);
                    else if(tokens[j]=="-m") m=to_int(tokens[j+1]);
                    else if(tokens[j]=="-s") to_vector_str(tokens[j+1],s);
                    else if(tokens[j]=="-p") to_vector_int(tokens[j+1],p);
                    else if(tokens[j]=="-x") x=Time(tokens[j+1]);
                    else if(tokens[j]=="-t") to_vector_int(tokens[j+1],t);
                    else if(tokens[j]=="-o") {if(n>2) to_vector_int(tokens[j+1],o);}
                    else if(tokens[j]=="-d")
                    {
                        string &day=tokens[j+1];
                        d_beg=Date(day.substr(0,5));
                        d_end=Date(day.substr(6,5));
                    }
                    else y=tokens[j+1][0];
                }
                os<<sys.add_train(i,n,m,s,p,x,t,o,d_beg,d_end,y)-1;
            }
            else if(command=="release_train")
            {
                os<<sys.release_train(tokens[2])-1;
            }
            else if(command=="query_train")
            {
                string i;
                Date d;
                for(int j=1;j<tokens.size();j+=2)
                {
                    if(tokens[j]=="-i") i=tokens[j+1];
                    else d=Date(tokens[j+1]);
                }
                static vecS ans;
                ans.clear();
                sys.query_train(i,d,ans);
                print_os(ans,os);
            }
            else if(command=="delete_train")
            {
                os<<sys.delete_train(tokens[2])-1;
            }
            else if(command=="query_ticket")
            {
                string s,t;
                Date d;
                bool p=true;
                for(int j=1; j<tokens.size(); j+=2)
                {
                    if(tokens[j]=="-s") s=tokens[j+1];
                    else if(tokens[j]=="-t") t=tokens[j+1];
                    else if(tokens[j]=="-d") d=Date(tokens[j+1]);
                    else if(tokens[j]=="-p")
                    {
                        if(tokens[j+1]=="time") p=true;
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
                for(int j=1; j<tokens.size(); j+=2)
                {
                    if(tokens[j]=="-s") s=tokens[j+1];
                    else if(tokens[j]=="-t") t=tokens[j+1];
                    else if(tokens[j]=="-d") d=Date(tokens[j+1]);
                    else if(tokens[j]=="-p")
                    {
                        if(tokens[j+1]=="time") p=true;
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
                for(int j=1;j<tokens.size();j+=2)
                {
                    if(tokens[j]=="-u") u=tokens[j+1];
                    else if(tokens[j]=="-i") i=tokens[j+1];
                    else if(tokens[j]=="-d") d=Date(tokens[j+1]);
                    else if(tokens[j]=="-n") n=to_int(tokens[j+1]);
                    else if(tokens[j]=="-f") f=tokens[j+1];
                    else if(tokens[j]=="-t") t=tokens[j+1];
                    else if(tokens[j]=="-q")
                    {
                        if(tokens[j+1]=="false") q=false;
                        else q=true;
                    }
                }
                os<<sys.buy_ticket(u,i,d,f,t,n,q);
            }
            else if(command=="query_order")
            {
                string u("");
                if(tokens[1]=="-u") u=tokens[2];
                static vecS ans;
                ans.clear();
                sys.query_order(u,ans);
                print_os(ans,os);
            }
            else if(command=="refund_ticket")
            {
                string u;
                int n=1;
                for(int j=1;j<tokens.size();j+=2)
                {
                    if(tokens[j]=="-u") u=tokens[j+1];
                    if(tokens[j]=="-n") n=to_int(tokens[j+1]);
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