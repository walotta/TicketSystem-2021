//TicketSystem main.cpp for test

#include "systemcore.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <iomanip>
char real_command[100000];
char result[100000];

Sirius::System ticketSystem;

int main() {
    int slisten = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9988);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(slisten, (struct sockaddr*)&sin, sizeof(sin)) == -1)
        printf("bind error!");
    if (listen(slisten, 5) == -1)
    {
        printf("listen error!");
        return 0;
    }
    int sClient;//创建客户端socket
    sockaddr_in remoteAddr;//客户端的地址信息
    socklen_t nAddrlen = sizeof(remoteAddr);
    char revData[99000];//接收数据缓冲区
    unsigned long long run_cnt=0;

    std::string add_admin="add_user -c admin -u walotta_admin -p walotta_admin -n walotta_admin -m walotta@126.com -g 10";
    std::string sign_in_admin="login -u walotta_admin -p walotta_admin";
    printf("begin : %s\n",add_admin.c_str());
    printf("return: %s\n",ticketSystem.response(add_admin).second.c_str());
    printf("begin : %s\n",sign_in_admin.c_str());
    printf("return: %s\n\n",ticketSystem.response(sign_in_admin).second.c_str());

    while(1)
    {
        run_cnt++;
        printf("run at cnt %lld\n",run_cnt);
        memset(real_command, 0, sizeof(real_command));
        memset(result, 0, sizeof(result));
        //memset(real_result, 0, sizeof(real_result));
        sClient = accept(slisten, (struct sockaddr*)&remoteAddr, &nAddrlen);
        if (sClient == -1)
        {
            printf("accept error!\n");
            continue;
        }
        /*if (strcmp(inet_ntoa(remoteAddr.sin_addr), "49.234.112.112"))
        {
            printf("server attack from outside!\n");
            continue;
        }*/
        int ret = recv(sClient, revData, 99000, 0);
        std::stringstream sstr(revData);
        int cmd_length = 0;
        sstr >> cmd_length;
        int len_int=std::to_string(cmd_length).size();
        strncpy(real_command,revData+len_int,cmd_length);
        printf("read  command: %s\n", real_command);
        const auto& run_out = ticketSystem.response(real_command);
        strncpy(result,run_out.second.c_str(),run_out.second.size());
        printf("return result: %s\n----\n", result);
        send(sClient, result, strlen(result), 0);
        if(!run_out.first)break;
    }
    return 0;


    /*while (getline(std::cin, buffer)) {
        const auto& ret = ticketSystem.response(buffer);
        std::cout << ret.second << '\n';
        if (!ret.first) return 0;
    }
    return 0;*/
}