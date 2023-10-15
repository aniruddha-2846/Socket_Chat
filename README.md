# Socket_Chat

A simple chat application using socket programming APIs in C++ which is compatible with Linux and Windows.

Run client.cpp and serer.cpp separately.

For Linux: g++ server.cpp -o server

For Windows: g++ server.cpp -o server -lws2_32

- Uses non-blocking function calls.
- Single threaded. Hence, is a SYNCHRONOUS program.
