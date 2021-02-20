#include "ThreadPool.h"
#include "server.h"
int main(int argc, char* argv[])
{
    auto* server = new MServer::Server {};
    server->run();
    return 0;
}