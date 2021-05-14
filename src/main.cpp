#include "cmd.hpp"
#include <iostream>

int main(int argc,char *argv[])
{
    bool run_with_io=true;
    if(argc!=1)
    {
        run_with_io=false;
    }
    cmd ticketLibrary;
    if(run_with_io)
    {
        ticketLibrary.run(std::cin,std::cout);
    }else
    {
        stringstream sin,sout;
        ticketLibrary.run(sin,sout);
    }
}