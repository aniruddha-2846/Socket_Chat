#ifndef sandesh_messages_h
#define sandesh_messages_h

#include <string>
// void sandesh_info(std::string message);
// void sandesh_error(std::string message);

void sandesh_info(std::string message)
{
    std::cout << message << std::endl;
}

void sandesh_error(std::string message)
{
    std::cout << message << std::endl;
    exit(EXIT_FAILURE);
}

#endif