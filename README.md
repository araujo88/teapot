# teapot

A simple multi-threaded web server in C++

## Getting started

### Compiling

`make clean && make`

### Setting up

In `main.cpp` file:

```
#include "../include/teapot.hpp"

int main(int argc, char *argv[])
{
    Teapot server;
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
    Teapot server = Teapot(atoi(argv[1]));
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
    Teapot server = Teapot(ip_address, port, max_connections, logging_type, static_files_dir);
    server.runServer();
    return 0;
}

```

`ip_address`: the server IP address. The default is `127.0.0.1`. <br>
`port`: the server port. The default is `8000`. <br>
`max_connections`: the maximum number of simultaneous connections per thread. The default is `1`. <br>
`logging_type`: the Teapot server provides three levels of logging: `DEFAULT`, `DISABLED` and `VERBOSE`. <br>
`static_files_dir`: the relative folder path where static files are located. The default is set to `static`. <br>
