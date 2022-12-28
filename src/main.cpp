#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Fatal error - please provide a port number!" << std::endl;
        return -1;
    }
    Teapot server = Teapot(atoi(argv[1]));
    CORSMiddleware cors_middleware = CORSMiddleware("*", "*", "*", 86400, true);
    server.addMiddleware(cors_middleware);
    server.returnJSON("/test", "{\"name\": \"john\", \"surname\": \"doe\"}");
    server.returnHTML("/example", "<html><h1>Example</h1></html>");
    server.runServer();
    return 0;
}