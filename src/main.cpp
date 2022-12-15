#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    Teapot server;
    CORSMiddleware cors_middleware = CORSMiddleware("*", "*", "*", 86400, true);
    server.addMiddleware(cors_middleware);
    server.runServer();
    return 0;
}