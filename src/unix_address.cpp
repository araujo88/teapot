#include "../include/unix_address.hpp"

using namespace tpt;

UnixAddress::UnixAddress()
{
    this->address = new struct sockaddr;
}

struct sockaddr *UnixAddress::getAddress()
{
    return this->address;
}

UnixAddress::~UnixAddress()
{
    delete address;
}
