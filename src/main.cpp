#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    Teapot server = Teapot(atoi(argv[1]));
    CORSMiddleware cors_middleware = CORSMiddleware("*", "*", "*", 86400, true);
    server.addMiddleware(cors_middleware);
    server.returnJSON("/test", "{\"name\": \"john\", \"surname\": \"doe\"}");
    server.runServer();
    return 0;
}