#include "server.h"
#include "spdlog/spdlog.h"

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <netinet/in.h>
#include <spdlog/fmt/bundled/format.h>
#include <string>
using namespace spdlog;
const int PORT = 8111;

static void on_read_cb(struct bufferevent* bev, void* ctx)
{
    /* 获取bufferevent中的读和写的指针 */
    /* This callback is invoked when there is data to read on bev. */
    struct evbuffer* input = bufferevent_get_input(bev);
    struct evbuffer* output = bufferevent_get_output(bev);
    /* 把读入的数据全部复制到写内存中 */
    /* Copy all the data from the input buffer to the output buffer. */
    evbuffer_add_buffer(output, input);
}
static void echo_event_cb(struct bufferevent* bev, short events, void* ctx)
{
    if (events & BEV_EVENT_ERROR)
        perror("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        bufferevent_free(bev);
    }
}
static void on_accept_cb(
    struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ctx)
{
    /* 初始化一个 bufferevent 用于数据的写入和读取，首先需要从 Listener 中获取 event_base */
    /* We got a new connection! Set up a bufferevent for it. */

    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = bufferevent_socket_new(base, fd, 0);

    /*设置 bufferevent 的回调函数，这里设置了读和事件的回调函数*/
    bufferevent_setcb(bev, on_read_cb, NULL, echo_event_cb, NULL);
    /* 启用该 buffevent 写和读 */
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

namespace MServer
{

inline void accept_error_cb(struct evconnlistener* listener, void* ctx)
{
    struct event_base* base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr,
        "Got an error %d (%s) on the listener. "
        "Shutting down.\n",
        err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
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
    struct event_base* base = event_base_new(); /* 初始化event_base */
    if (!base)
    {
        warn("open event base failed\n");
    }
    struct sockaddr_in serveraddr;
    /*初始化绑定地址*/
    /*Clear the sockaddr before using it,
     * in case there are extra platform-specific fields
     * that can mess us up.
     */
    memset(&serveraddr, 0, sizeof(serveraddr));
    /*This is an INET address */
    serveraddr.sin_family = AF_INET;
    /*Listen on 0.0.0.0*/
    serveraddr.sin_port = htons(PORT);
    /*Listen on the given port.*/
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    /* 初始化evconnlistener(绑定地址、设置回调函数以及连接属性) */
    auto* listener = evconnlistener_new_bind(base, on_accept_cb, NULL, LEV_OPT_REUSEABLE, 10,
        (const struct sockaddr*)(&serveraddr), sizeof(serveraddr));
    if (!listener)
    {
        warn("Listener init error\n");
        return;
    }
    /* 设置Listen错误回调函数 */
    evconnlistener_set_error_cb(listener, accept_error_cb);
    /* 开始accept进入循环 */
    event_base_dispatch(base);
}
}   // namespace MServer