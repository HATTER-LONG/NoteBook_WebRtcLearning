# WebRtc 协议

- [WebRtc 协议](#webrtc-协议)
  - [STUN 协议](#stun-协议)
    - [RFC STUN 规范](#rfc-stun-规范)
    - [STUN 协议规格](#stun-协议规格)
  - [STUN Message Body](#stun-message-body)
  - [ICE](#ice)

## STUN 协议

STUN 存在的目的，从客户端角度就是进行 NAT 穿越，客户端发送 STUN 消息到 STUN 服务，STUN 服务收到了后将发送消息这台主机的外网地址发回客户主机。查出自己位于哪种类型的 NAT 之后以及 NAT 为某一个本地端口所绑定的 Internet 端端口。当与另外一端通信时，通过交换 NAT 信息就可以建立通信链接了。

流媒体服务器也使用了 STUN，主要时为了客户端发起 STUN 请求时的用户名和密码。用于验证请求客户端是否合法。

STUN 时典型的客户端/服务器模式。客户端发送请求，服务端进行响应。

### RFC STUN 规范

RFC 有以下两种 STUN 协议规范，其区别在于 RFC5389 还可以使用 TCP 进行穿越，有一些 3489 的协议废除与新增。

1. RFC3489/STUN：Simple Traversal of UDP Through NAT。基础描述。
2. RFC5389/STUN：Session Traversal Utilities for Nat。增量修改。

### STUN 协议规格

STUN 协议由 20 个字节的 Stun header 组成和 Data 组成。在 Body 中可以由 0 个或多个 Attribute 属性。

![0601](./Img/06_01.png)

1. 前两位的 `00`，协议规定。用于区分复用同意端口时 STUN 协议，因为 STUN 协议有老协议（非00）和新协议（00）。
2. TYPE，使用 STUN 协议有很多消息类型。加上前边 `00` 一共 2 个字节（16 bit）。剩余 14 位中有有两位将 STUN 协议分成两种类型，即 C0 和 C1，12 位用于定义请求/指示。
    - C1C0：如下
      - 00：表示一个请求。
      - 01：表示一个指示。
      - 10：表示是请求成功的响应。
      - 11：表示是请求失败的响应。
    - 消息举例：
      - 0x0001：0x0 (c1)(c0) 1，即 C0C1 都是 0 表示请求，最后一位是 1，总的就是绑定消息。
      - 0x0101：C1C0 是 10，表示请求成功的响应，总的就是绑定响应。
      - 0x0111：C1C0 是 11，表示请求失败的响应，总的就是绑定错误。

    ![0602](./Img/06_02.png)

3. Length，Message 的长度。2 个字节的消息长度，不包括消息头。
4. Magic Cookie：魔法数，用于判断消息为 STUN 消息。4 字节，23 位，固定值位 0x2112A442，通过它可以判断客户端是否支持某些属性。
5. Transaction ID：发送与回复都是用同一个 ID 进行联系。12 字节，96 位。

## STUN Message Body

STUN 消息头后有 0 或多个属性，每个属性进行 TLV 编码：Type - Length - Value。先根据类型 Type，去除数据长度 Length 是多少，读取 Value 数据。

![0603](./Img/06_03.png)

RFC3489 定义的属性：

![0604](./Img/06_04.png)

属性的使用，N/A 不必填写，M 必填，O 可选：

![0605](./Img/06_05.png)

## ICE

ICE（interactive Connectivity Establishment），需要两端进行交互才能创建链接。
