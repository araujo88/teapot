# teapot 🫖

A simple and lightweight multi-threaded web framework in C++ that serves static websites.

## Getting started

### Compiling

`make clean && make`

### Setting up

In `main.cpp` file:

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server;
    server.runServer();
    return 0;
}
```

To start the server, run `./teapot`. This will start the server at the default url `localhost:8000`.

To run in a different port:

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server = tpt::Teapot(atoi(argv[1]));
    server.runServer();
    return 0;
}

```

Start the server by running `./teapot <port_number>`.

Optionally, the following arguments can be provided for the server instance:

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server = tpt::Teapot(ip_address, port, max_connections, logging_type, static_files_dir);
    server.runServer();
    return 0;
}

```

`ip_address`: the server IP address. The default is `127.0.0.1`. <br>
`port`: the server port. The default is `8000`. <br>
`max_connections`: the maximum number of simultaneous requests. The default is `10`. <br>
`logging_type`: the Teapot server provides three levels of logging: `DEFAULT`, `DISABLED` and `VERBOSE`. <br>
`static_files_dir`: the relative folder path where static files are located. The default is set to `static`. <br>

## Serving files

You can link a HTML file to a URL by using the method `serveFile(url, file_path)`:

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server;
    server.serveFile("/example", "/example.html");
    server.runServer();
    return 0;
}
```

## Returning JSON responses

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server;
    server.returnJSON("/test", "{\"name\": \"john\", \"surname\": \"doe\"}");
    server.runServer();
    return 0;
}
```

## Returning hard-coded HTML

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server;
    server.returnHTML("/example", "<html><h1>Example</h1></html>");
    server.runServer();
    return 0;
}
```

## Adding middleware

Teapot web framework provides some builtin middleware such as CORS middleware, sanitizer and security headers by default. However, they can be customized to your needs by instantiating an object and then adding it to the server instance.

```
#include "../include/teapot.hpp"
#include "../include/cors_middleware.hpp"

int main(int argc, char *argv[])
{
    tpt::Teapot server;
    tpt::CORSMiddleware cors_middleware = tpt::CORSMiddleware("*", "*", "*", 86400, true);
    server.addMiddleware(cors_middleware);
    server.runServer();
    return 0;
}
```

For CORS middleware, the parameters are: `CORSMiddleware(allow_origins, allow_methods, allow_headers, max_age, allow_credentials)`.

You can create your own custom middleware by inherting from the `Middleware` class which already contains the request and response handlers methods.

## TODOs

- Implement WinSocket
