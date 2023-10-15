// checking if the machine is a Windows Machine
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WINDOWS
#endif

// mandatory library includes
#include <iostream>
#include <string>
#include "sandesh.h"
#include "server.h"

// OS specific library includes
#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <iostream>
#include <cstring>
#include <sys/socket.h> //for bind(), getaddrinfo(), connect(), socket(), listen(), accept(), send(), recv(), shutdown(),
#include <sys/types.h>  // for getaddrinfo(),
#include <netinet/in.h> //for htons, ntoh
#include <arpa/inet.h>  // for  struct in_addr, inet_addr, inet_ntoa,
#include <net/if.h>
#include <unistd.h> // for close(), fork()
#endif

struct sockaddr_in cliaddr; // structure for client's socket

#define PORT 8191
#define MY_ERROR -1
#define BUFFER 1024
#define MAX_CLIENTS 3
#define BACKLOG 10

int main()
{
    int serverfd; // socket descriptor for the  listener socket
    int clientfd; // socket descriptor for the clinet to connect to the server / for sending and recieving information
    struct sockaddr_in for_server, for_peer;
#ifdef OS_WINDOWS
    int ipaddr_len;
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#else
    socklen_t ipaddr_len;
    char ipaddress[INET_ADDRSTRLEN];
#endif
    sandesh_info("Connection establishment initiated.....");

    for_server.sin_family = AF_INET;
    for_server.sin_port = htons(PORT);
    for_server.sin_addr.s_addr = INADDR_ANY;
    memset(&(for_server.sin_zero), 0, 8);

    if ((serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == MY_ERROR)
        servererror("socket() Error :: ");

    // the socket which is used  by bind() becomes a listener socket, it cannot act as a soket to  send/recv information
    if (bind(serverfd, (struct sockaddr *)&for_server, sizeof(for_server)) == MY_ERROR)
        servererror("bind() Error :: ");

    if (listen(serverfd, BACKLOG) == MY_ERROR)
        servererror("Listen Error");

    ipaddr_len = sizeof(for_peer);
    if ((clientfd = accept(serverfd, (struct sockaddr *)&for_peer, &ipaddr_len)) == MY_ERROR)
        servererror("accept() Error :: ");

    sandesh_info("Connection Established. ");

#ifdef OS_WINDOWS
    char *ipaddress = inet_ntoa(for_peer.sin_addr);
#else
    inet_ntop(AF_INET, &(for_peer.sin_addr), ipaddress, INET_ADDRSTRLEN);
#endif

    std::cout << "IPaddress of Client :: " << ipaddress << std::endl
              << std::endl;

    const char *msg = "'Hi!' from server";
    int sendfd = send(clientfd, msg, strlen(msg), 0);
    if (sendfd == MY_ERROR)
        servererror("send() Error :: ");

    char buff_to_send[1024] = {
        0,
    };

    char buff_to_recv[1024] = {
        0,
    };

    while (1)
    {
        if (recv(clientfd, buff_to_recv, 1024, 0) == MY_ERROR)
            servererror("recv() Error :: ");
        std::cout << "From Client: " << buff_to_recv;
        std::cout << "To Client: ";
        fgets(buff_to_send, 1023, stdin);
        send(clientfd, buff_to_send, sizeof(buff_to_send), 0);
    }

    return 0;
}
