#ifndef SERVER_H
#define SERVER_H

// checking if the machine is a Windows Machine
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WINDOWS
#endif

// mandatory library includes
#include <iostream>
#include <cstring>
#include "client.h"

// OS specific library includes
#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <ifaddrs.h>

#endif

#define ERROR -1

#ifdef OS_WINDOWS
static void getIPAddressForWin(char *ipaddress)
{
    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) == ERROR)
        clienterror("gethostname() Error");

    struct addrinfo *result = nullptr;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(hostName, nullptr, &hints, &result) == ERROR)
        clienterror("getaddrinfo() Error");

    struct sockaddr_in *sockAddr = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);
    inet_ntop(AF_INET, &(sockAddr->sin_addr), ipaddress, INET_ADDRSTRLEN);
}
#else
static void getIPAddressForLinux(char *ipaddress){
    struct ifaddrs *ifap, *ifa;
    if (getifaddrs(&ifap) == 0) {
        for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr != nullptr && ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *sa = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
                inet_ntop(AF_INET, &(sa->sin_addr), ipaddress, INET_ADDRSTRLEN);
                if (strcmp(ipaddress, "127.0.0.1") != 0) {
                    break;  
                }
            }
        }
        freeifaddrs(ifap);
    } else clienterror("getifaddrs() Error.");
}
#endif

#endif