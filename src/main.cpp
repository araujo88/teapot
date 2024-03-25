#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Fatal error - please provide a port number!" << std::endl;
        return EXIT_FAILURE;
    }
    tpt::Teapot server = tpt::Teapot(atoi(argv[1]));
    tpt::CORSMiddleware cors_middleware = tpt::CORSMiddleware("*", "*", "*", 86400, true);
    tpt::SecurityMiddleware security_middleware;

    server.addMiddleware(cors_middleware);
    server.addMiddleware(security_middleware);
    server.returnJSON("/test", "{\"name\": \"john\", \"surname\": \"doe\"}");
    server.returnHTML("/example", "<html><h1>Example</h1></html>");
    server.run();
    return 0;
}
