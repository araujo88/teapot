#ifndef UNIX_ADDRESS_SOCKET_H_
#define UNIX_ADDRESS_SOCKET_H_

#include "address.hpp"
#include <sys/socket.h>

namespace tpt
{
    class UnixAddress : public IAddress
    {
    private:
        struct sockaddr *address;

    public:
        UnixAddress();
        ~UnixAddress();
        struct sockaddr *getAddress();
    };
}

#endif