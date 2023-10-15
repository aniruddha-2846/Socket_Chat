#ifndef CLIENT_H
#define CLIENT_H

// checking if the machine is a Windows Machine
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WINDOWS
#endif

// common libraries
#include <iostream>
#include <cstring>

#ifdef OS_WINDOWS
#include <error.h>
#endif

// function to show error messages
void clienterror(std::string errormesg)
{
#ifdef OS_WINODWS
    std::cout << errorname << " " << WSAGetLastError() << std::endl;
    WSACleanup();
#else
    std::cerr << errormesg << strerror(errno) << std::endl;
    exit(-1);
#endif

    exit(EXIT_FAILURE);
}

#endif