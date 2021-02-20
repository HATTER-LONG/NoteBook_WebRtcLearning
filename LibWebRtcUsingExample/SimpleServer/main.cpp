#include "TcpServer.h"
#include "ThreadPool.h"
int main(int argc, char* argv[])
{
    auto* server = new MServer::TCPServer {};
    server->run();
    return 0;
}