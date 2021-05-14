//
// Created by walotta on 2021/5/14.
//

#include "ManagementSystem.h"
#include <sstream>

class cmd
{
private:
    ManagementSystem sys;
    stringstream split;
    int to_int(const string& o)
    {
        int ans=0;
        for(int i=o.size()-1;i>=0;i--)
        {
            ans*=10;
            ans+=o[i]-'0';
        }
        return ans;
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
    void print_os(vector<T> list,ostream& o)
    {
        int cnt=list.size();
        for(int i=0;i<cnt;i++)
        {
            o<<list[i];
            if(i!=cnt-1)o<<'\n';
        }
    }
public:
    void run(std::istream &is, std::ostream &os)
    {
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
        }
    }
};