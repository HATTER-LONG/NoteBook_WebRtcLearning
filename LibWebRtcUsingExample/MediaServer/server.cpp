#include "server.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <unistd.h>

using namespace spdlog;
namespace MServer
{
Server::Server()
{
    info("{} construct ...", __FUNCTION__);
}
Server::~Server()
{
    info("{} destruct ...", __FUNCTION__);
}
void Server::run()
{
    while (1)
    {
        info("{} running...", __FUNCTION__);
        ::usleep(1000000);
    }
}
}   // namespace MServer