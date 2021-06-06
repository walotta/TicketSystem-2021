#include "cmd.hpp"
#include <iostream>
#include <fstream>
#include "ErrorMessage.h"


//std::ifstream in("../data/basic_1/1.in");
//std::ofstream out("output.txt");



int main(int argc,char *argv[])
{
//    freopen("debug.txt","a",stdout);
//    printf("\n\n");
    bool run_with_io=true;
    if(argc!=1) run_with_io=false;

    cmd ticketLibrary;
    if(run_with_io)
    {
        std::ios::sync_with_stdio(false);
        try {ticketLibrary.run(std::cin,std::cout);}
        catch(error x)
        {
            std::cout<<x.what()<<std::endl;
            return 0;
        }
    }
    else
    {
        std::stringstream sin,sout;
        ticketLibrary.run(sin,sout);
    }
}