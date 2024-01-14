#ifndef GETIPPORT_H
#define GETIPPORT_H
#include<string>

class getipport
{
public:
    getipport();

public:
    std::string ServerIp;
    int ServerPort;

    std::string ProxyIp;
    int ProxyPort;

public:
   bool getServerIpPort();

};

#endif // GETIPPORT_H
