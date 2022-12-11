#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    Teapot server = Teapot(atoi(argv[1]));
    server.serveFile("/", "/index.html");
    server.runServer();
    return 0;
}