#ifndef ISOCKET_H_
#define ISOCKET_H_

#ifdef _WIN32
typedef unsigned __int64    ssize_t;
#endif
#ifdef __linux__
typedef int SOCKET;
#endif

namespace tpt
{
    class ISocket
    {
    public:
        virtual void bindSocket() = 0;
        virtual void listenToConnections() = 0;
        virtual void acceptConnection(SOCKET& client_socket, void *client_address) = 0;
        virtual ssize_t receiveData(SOCKET client_socket, char *buffer, unsigned int buffer_size) = 0;
        virtual void sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags) = 0;
        virtual void closeSocket() = 0;
        virtual void closeSocket(SOCKET client_socket) = 0;
    };
}

#endif
