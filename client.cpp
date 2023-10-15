// checking if the machine is a Windows Machine
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WINDOWS
#endif

// mandatory library includes
#include <iostream>
#include <cstring>
#include <string>
#include "sandesh.h"
#include "client.h"
#include "getipaddr.h"

// OS specific library includes
#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#endif

#define PORT 8191
#define MY_ERROR -1
#define BUFFER 1024

int main()
{

    int clientfd;
    struct sockaddr_in for_client;
    char ipaddress[INET_ADDRSTRLEN];
#ifdef OS_WINDOWS
    int ipaddr_len;
    WSADATA wsdata;
    if (WSAStartup(MAKEWORD(2, 2), &wsdata) == MY_ERROR)
        clienterror("WSAStartup Error :: ");
    getIPAddressForWin(ipaddress);
#else
    socklen_t ipaddr_len;
    getIPAddressForLinux(ipaddress);
#endif

    // socket descriptions
    for_client.sin_family = AF_INET;
    for_client.sin_port = htons(PORT);
    for_client.sin_addr.s_addr = inet_addr(ipaddress);
    memset(&for_client.sin_zero, 0, 8);
    sandesh_info("Connection establishment initiated....");

    if ((clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == MY_ERROR)
        clienterror("socket() Error.! ");
    if (connect(clientfd, (struct sockaddr *)&for_client, sizeof(for_client)) == MY_ERROR)
        clienterror("connect() Error! ");

    sandesh_info("Connection Established.");

#ifdef OS_WINDOWS
    char *ipaddress_of_server = inet_ntoa(for_client.sin_addr);
    std::cout << "IPaddress of Server :: " << ipaddress_of_server << std::endl
              << std::endl;
#else
    inet_ntoa(AF_INET, &(for_client.sin_addr), ipaddress, INET_ADDRSTRLEN);
    std::cout << "IPaddress of Server :: " << ipaddress << std::endl
              << std::endl;
#endif

    char buff_to_send[BUFFER] = {
        0,
    };

    char buff_to_recv[BUFFER] = {
        0,
    };

    if (recv(clientfd, buff_to_recv, sizeof(buff_to_recv), 0) == MY_ERROR)
        clienterror("recv() Error :: ");
    std::cout << buff_to_recv << std::endl;
    sandesh_info("Begin sending messages to the server.");
    std::cout << std::endl;
    while (true)
    {
        std::cout << "To server: ";
        fgets(buff_to_send, 1023, stdin);
        send(clientfd, buff_to_send, sizeof(buff_to_send), 0);

        std::cout << "From Server: ";
        if (recv(clientfd, buff_to_recv, sizeof(buff_to_recv), 0) == MY_ERROR)
            clienterror("recv() Error ::");
        std::cout << buff_to_recv;
    }

    return 0;
}
