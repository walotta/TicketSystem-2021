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
    stringstream split;
    int to_int(const string& input)
    {
        int ans=0;
        for(int i=0; i<input.size(); ++i)
        {
            ans*=10;
            ans+=input[i]-'0';
        }
        return ans;
    }
    vector<int> to_vector_int(str input)
    {
        vector<int> output;
        Fourest::separator ints("|");
        ints.process(input);
        for(int i=0;i<ints.size();++i) output.push_back(to_int(ints[i]));
        return output;
    }
    vector<string> to_vector_str(str input)
    {
        vector<int> output;
        Fourest::separator ints("|");
        ints.process(input);
        return ints.content();
    }
    Date to_date(str input)
    {
        Fourest::separator processor("-");
        processor.process(input);
        return Date(to_int(processor[0]),to_int(processor[1]));
    }
    Time to_time(str input)
    {
        Fourest::separator processor(":");
        processor.process(input);
        return Time(to_int(processor[0]),to_int(processor[1]));
    }

    Date readDate(istream& o)
    {
        int month,day;
        o>>month;
        o.get();
        o>>day;
        Date ans;
        ans.month=month;
        ans.day=day;
        return ans;
    }
    Time readTime(istream& o)
    {
        int hour,minute;
        o>>hour;
        o.get();
        o>>minute;
        Time ans;
        ans.hour=hour;
        ans.minute=minute;
        return ans;
    }


    template<class T>
    void print_os(const vector<T> &list,ostream& o)
    {
        int cnt=list.size();
        for(int i=0;i<cnt;i++)
        {
            o<<list[i];
            if(i!=cnt-1)o<<'\n';
        }
    }
public:
    cmd():words(" "){}
    void run(std::istream &is, std::ostream &os)
    {
        while(words.readLine(is))
        {
            auto tokens=words.content();
            auto &command=tokens[0];
            if(command=="add_user")
            {
                string c="",u="",p="",n="",m="";
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
                string u="",p="";
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-u") u=tokens[i+1];
                    else p=tokens[i+1];
                }
                os<<sys.login(u,p)-1;
            }
            else if(command=="logout")
            {
                string u="";
                if(tokens[1]=="-u") u=tokens[2];
                os<<sys.logout(u)-1;
            }
            else if(command=="query_profile")
            {
                string c,u;
                for(int i=1;i<tokens.size();i+=2)
                {
                    if(tokens[i]=="-c") c=tokens[i+1];
                    else u=tokens[i+1];
                }
                os<<sys.query_profile(c,u);
            }
            else if(command=="modify_profile")
            {
                string c="",u="",p="",n="",m="";
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
                    else if(tokens[j]=="-s") s=to_vector_str(tokens[j+1]);
                    else if(tokens[j]=="-p") p=to_vector_int(tokens[j+1]);
                    else if(tokens[j]=="-x") x=to_time(tokens[j+1]);
                    else if(tokens[j]=="-t") t=to_vector_int(tokens[j+1]);
                    else if(tokens[j]=="-o") o=to_vector_int(tokens[j+1]);
                    else if(tokens[j]=="-d")
                    {
                        Fourest::separator process("|");
                        process.process(tokens[j+1]);
                        d_beg=to_date(process[0]);
                        d_end=to_date(process[1]);
                    }
                    else y=tokens[j+1][0];
                }
                os<<sys.add_train(i,n,m,s,p,x,t,o,d_beg,d_end,y)-1;
            }
            else if(command=="release_train")
            {
                string i;
                if(tokens[1]=="-i") i=tokens[2];
                os<<sys.release_train(i)-1;
            }
            else if(command=="query_train")
            {
                string i;
                Date d;
                for(int j=1;j<tokens.size();j+=2)
                {
                    if(tokens[j]=="-i") i=tokens[j+1];
                    else d=to_date(tokens[j+1]);
                }
                auto ans=sys.query_train(i,d);
                print_os(ans,os);
            }
            else if(command=="delete_train")
            {
                string i;
                if(tokens[1]=="-i") i=tokens[2];
                os<<sys.delete_train(i)-1;
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
                    else if(tokens[j]=="-d") d=to_date(tokens[j+1]);
                    else if(tokens[j]=="-p")
                    {
                        if(tokens[j+1]=="time") p=true;
                        else p=false;
                    }
                }
                auto ans=sys.query_ticket(s,t,d,p);
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
                    else if(tokens[j]=="-d") d=to_date(tokens[j+1]);
                    else if(tokens[j]=="-p")
                    {
                        if(tokens[j+1]=="time") p=true;
                        else p=false;
                    }
                }
                auto ans=sys.query_transfer(s,t,d,p);
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
                    else if(tokens[j]=="-d") d=to_date(tokens[j+1]);
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
                string u="";
                if(tokens[1]=="-u") u=tokens[2];
                auto ans=sys.query_order(u);
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
                os<<"bye";
                exit(0);
            }
            os<<"\n";
        }


        /*
        string op;
        while(is>>op)
        {
            if(op=="add_user")
            {
                string list[5];
                int pri;
                for(int i=0;i<6;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-c")
                    {
                        is>>list[0];
                    }else if(token=="-u")
                    {
                        is>>list[1];
                    }else if(token=="-p")
                    {
                        is>>list[2];
                    }else if(token=="-n")
                    {
                        is>>list[3];
                    }else if(token=="-m")
                    {
                        is>>list[4];
                    }else if(token=="-g")
                    {
                        is>>pri;
                    }
                }
                bool success=sys.add_user(list[0],list[1],list[2],list[3],list[4],pri);
                if(success)os<<0;
                else os<<-1;
            }else if(op=="login")
            {
                string list[2];
                for(int i=0;i<2;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-u")
                    {
                        is>>list[0];
                    }else if(token=="-p")
                    {
                        is>>list[1];
                    }
                }
                bool success=sys.login(list[0],list[1]);
                if(success)os<<0;
                else os<<-1;
            }else if(op=="logout")
            {
                string list;
                is>>list;
                is>>list;
                bool success=sys.logout(list);
                if(success)os<<0;
                else os<<-1;
            }else if(op=="query_profile")
            {
                string list[2];
                for(int i=0;i<2;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-c")
                    {
                        is>>list[0];
                    }else if(token=="-u")
                    {
                        is>>list[1];
                    }
                }
                string ans=sys.query_profile(list[0],list[1]);
                os<<ans;
            }else if(op=="modify_profile")
            {
                string list[5];
                for(int i=0;i<5;++i) list[i]="";
                int pri=-404;
                for(int i=0;i<6;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-c")
                    {
                        is>>list[0];
                    }else if(token=="-u")
                    {
                        is>>list[1];
                    }else if(token=="-p")
                    {
                        is>>list[2];
                    }else if(token=="-n")
                    {
                        is>>list[3];
                    }else if(token=="-m")
                    {
                        is>>list[4];
                    }else if(token=="-g")
                    {
                        is>>pri;
                    }else if(token.empty())
                    {
                        break;
                    }
                }
                string ans=sys.modify_profile(list[0],list[1],list[2],list[3],list[4],pri);
                os<<ans;
            }else if(op=="add_train")
            {
                string trainId;
                int stationNum,seatNum;
                vector<string> stations;
                Time startTime;
                vector<int> travelTimes,prices,stopoverTimes;
                Date saleDate_start,saleDate_stop;
                char type;
                for(int i=0;i<10;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-i")
                    {
                        is>>trainId;
                    }else if(token=="-n")
                    {
                        is>>stationNum;
                    }else if(token=="-m")
                    {
                        is>>seatNum;
                    }else if(token=="-s")
                    {
                        string to_split;
                        is>>to_split;
                        string str_station;
                        split.str(to_split);
                        getline(split,str_station,'|');
                        while(!str_station.empty())
                        {
                            stations.push_back(str_station);
                            str_station="";
                            getline(split,str_station,'|');
                        }
                    }else if(token=="-p")
                    {
                        string to_split;
                        is>>to_split;
                        string str_price;
                        split.clear();
                        split.str(to_split);
                        getline(split,str_price,'|');
                        while(!str_price.empty())
                        {
                            int int_price=to_int(str_price);
                            prices.push_back(int_price);
                            str_price="";
                            getline(split,str_price,'|');
                        }
                    }else if(token=="-x")
                    {
                        startTime=readTime(is);
                    }else if(token=="-t")
                    {
                        string to_split;
                        is>>to_split;
                        string str_time;
                        split.clear();
                        split.str(to_split);
                        getline(split,str_time,'|');
                        while(!str_time.empty())
                        {
                            int int_time=to_int(str_time);
                            travelTimes.push_back(int_time);
                            str_time="";
                            getline(split,str_time,'|');
                        }
                    }else if(token=="-o")
                    {
                        string to_split;
                        is>>to_split;
                        string str_time;
                        split.clear();
                        split.str(to_split);
                        getline(split,str_time,'|');
                        while(!str_time.empty())
                        {
                            int int_time=to_int(str_time);
                            stopoverTimes.push_back(int_time);
                            str_time="";
                            getline(split,str_time,'|');
                        }
                    }else if(token=="-d")
                    {
                        saleDate_start=readDate(is);
                        is.get();
                        saleDate_stop=readDate(is);
                    }else if(token=="-y")
                    {
                        is>>type;
                    }
                }
                bool success=sys.add_train(trainId,stationNum,seatNum,stations,prices,startTime,travelTimes,stopoverTimes,saleDate_start,saleDate_stop,type);
                if(success)os<<0;
                else os<<-1;
            }else if(op=="release_train")
            {
                string list;
                is>>list;
                is>>list;
                bool success=sys.release_train(list);
                if(success)os<<0;
                else os<<1;
            }else if(op=="query_train")
            {
                string trainId;
                Date ask_date;
                for(int i=0;i<2;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-i")
                    {
                        is>>trainId;
                    }else if(token=="-d")
                    {
                        ask_date=readDate(is);
                    }
                }
                vector<string> ans_list=sys.query_train(trainId,ask_date);
                if(ans_list.empty())os<<-1;
                else
                {
                    print_os(ans_list,os);
                }
            }else if(op=="delete_train")
            {
                string list;
                is>>list;
                is>>list;
                bool success=sys.delete_train(list);
                if(success)os<<0;
                else os<<1;
            }else if(op=="query_ticket")
            {
                string start_point,end_point,type;
                Date start_time;
                bool is_time=true;
                for(int i=0;i<4;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-s")
                    {
                        is>>start_point;
                    }else if(token=="-t")
                    {
                        is>>end_point;
                    }else if(token=="-d")
                    {
                        start_time=readDate(is);
                    }else if(token=="-p")
                    {
                        is>>token;
                        if(token=="cost")is_time=false;
                    }
                }
                vector<string> ans_list=sys.query_ticket(start_point,end_point,start_time,is_time);
                if(ans_list.empty())os<<-1;
                else
                {
                    os<<ans_list.size()<<'\n';
                    print_os(ans_list,os);
                }
            }else if(op=="query_transfer")
            {
                string start_point,end_point,type;
                Date start_time;
                bool is_time=true;
                for(int i=0;i<4;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-s")
                    {
                        is>>start_point;
                    }else if(token=="-t")
                    {
                        is>>end_point;
                    }else if(token=="-d")
                    {
                        start_time=readDate(is);
                    }else if(token=="-p")
                    {
                        is>>token;
                        if(token=="cost")is_time=false;
                    }
                }
                vector<string> ans_list=sys.query_transfer(start_point,end_point,start_time,is_time);
                if(ans_list.empty())os<<-1;
                else
                {
                    os<<ans_list.size()<<'\n';
                    print_os(ans_list,os);
                }
            }else if(op=="buy_ticket")
            {
                string userName,trainId,Start,End;
                Date date;
                int number;
                bool can_wait=false;
                for(int i=0;i<7;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-u")
                    {
                        is>>userName;
                    }else if(token=="-i")
                    {
                        is>>trainId;
                    }else if(token=="-d")
                    {
                        date=readDate(is);
                    }else if(token=="-n")
                    {
                        is>>number;
                    }else if(token=="-f")
                    {
                        is>>Start;
                    }else if(token=="-t")
                    {
                        is>>End;
                    }else if(token=="-q")
                    {
                        is>>token;
                        if(token=="true")can_wait=true;
                    }
                }
                string ans=sys.buy_ticket(userName,trainId,date,Start,End,number,can_wait);
                os<<ans;
            }else if(op=="query_order")
            {
                string list;
                is>>list;
                is>>list;
                vector<string> ans_list=sys.query_order(list);
                if(ans_list.empty())os<<-1;
                else
                {
                    print_os(ans_list,os);
                }
            }else if(op=="refund_ticket")
            {
                string userName;
                int num=1;
                for(int i=0;i<2;i++)
                {
                    string token;
                    is>>token;
                    if(token=="-u")
                    {
                        is>>userName;
                    }else if(token=="-n")
                    {
                        is>>num;
                    }
                }
                bool success=sys.refund_ticket(userName,num);
                if(success)os<<0;
                else os<<-1;
            }else if(op=="clean")
            {
                sys.clean();
            }else if(op=="exit")
            {
                exit(0);
            }
            os<<"\n";
        }
         */
    }
};