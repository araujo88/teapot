#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    Teapot server = Teapot(atoi(argv[1]));
    server.serveFile("/", "/index.html");
    server.runServer();
    return 0;
}