#include <iostream>
#include "src/webserver.hpp"

int main(int argc, char *argv[])
{
    Webserver server("127.0.0.1", 8000, 1);
    server.runServer();
    return 0;
}