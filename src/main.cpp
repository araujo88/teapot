#include <iostream>
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    Teapot server("127.0.0.1", 8001, 1);
    server.runServer();
    return 0;
}