#include "cmd.hpp"
#include <iostream>
#include <fstream>
#include "ErrorMessage.h"

ifstream in("../data/basic_2/1.in");
ofstream out("output.txt");



int main(int argc,char *argv[])
{
//    freopen("debug.txt","a",stdout);
//    printf("\n\n");
    bool run_with_io=true;
    if(argc!=1) run_with_io=false;

    cmd ticketLibrary;
    if(run_with_io)
    {
        try {ticketLibrary.run(in,out);}
        catch(error x)
        {
            cout<<x.what()<<endl;
            return 0;
        }
    }
    else
    {
        stringstream sin,sout;
        ticketLibrary.run(sin,sout);
    }
}