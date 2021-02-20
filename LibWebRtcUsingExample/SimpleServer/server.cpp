#include "server.h"
#include "spdlog/spdlog.h"

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <netinet/in.h>

using namespace spdlog;
const int PORT = 8111;
namespace MServer
{

void on_read_cb(struct bufferevent* bev, void* ctx)
{
    struct evbuffer* input = bufferevent_get_input(bev);
    struct evbuffer* output = bufferevent_get_output(bev);
    evbuffer_add_buffer(output, input);
}

void on_accept_cb(
    struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ctx)
{
    //回调中处理 fd 和 bufferevent 的关系

    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, fd, 0);

    bufferevent_enable(bev, EV_READ | EV_WRITE);
    bufferevent_setcb(bev, on_read_cb, NULL, NULL, NULL);
}

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
    struct event_base* base = nullptr;
    struct sockaddr_in serveraddr;

    event_base_new();
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);         //端口
    serveraddr.sin_addr.s_addr = INADDR_ANY;   // 监听所有网卡

    auto* listener = evconnlistener_new_bind(base, on_accept_cb, NULL, LEV_OPT_REUSEABLE, 10,
        (const struct sockaddr*)(&serveraddr), sizeof(serveraddr));
    if (!listener)
    {
        warn("Listener init error\n");
    }
    event_base_dispatch(base);
}
}   // namespace MServer