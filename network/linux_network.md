# Linux 网络

- [Linux 网络](#linux-网络)
  - [命令配置](#命令配置)
  - [套接字结构](#套接字结构)
  - [TCP](#tcp)
  - [网络收包](#网络收包)
  - [网络发包](#网络发包)
  - [epoll](#epoll)
  - [虚拟网络](#虚拟网络)
  - [性能优化](#性能优化)
  - [性能工具](#性能工具)
    - [传统工具](#传统工具)
    - [BPF工具](#bpf工具)

## 命令配置

- [ethtool](../tools/command.md#ethtool)
- [特殊文件](../tools/command.md#特殊文件)

## 套接字结构

![socket_struct](https://github.com/gongluck/images/blob/main/network/linux/socket/socket_struct.png)

## TCP

- TCP 状态轮转

![tcp_state](https://github.com/gongluck/images/blob/main/network/tcp/tcp_state.png)

- TCP 三次握手

![tcp_handshake](https://github.com/gongluck/images/blob/main/network/tcp/tcp_handshake.png)

## 网络收包

- 收包总流程

![recvpack](https://github.com/gongluck/images/blob/main/network/linux/recv/recvpack.png)

- 处理软中断

![ksoftirqd_recvpack](https://github.com/gongluck/images/blob/main/network/linux/recv/ksoftirqd_recvpack.png)

- 收包流程源码

![recvpack_source](https://github.com/gongluck/images/blob/main/network/linux/recv/recvpack_source.png)

- recvfrom 调用

![recvfrom](https://github.com/gongluck/images/blob/main/network/linux/recv/recvfrom.png)

## 网络发包

- 发包主流程

![sendpack](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack.png)

- 发包流程源码

![sendpack_source](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack_source.png)
![sendpack_end_source](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack_end_source.png)

- sendto 调用

![sendto](https://github.com/gongluck/images/blob/main/network/linux/send/sendto.png)

## epoll

- epoll 结构

![epoll](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll.png)
![epoll_struct](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll_struct.png)

- epoll_wait 调用

![epoll_wait](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll_wait.png)

## 虚拟网络

- bridge

![bridge](https://github.com/gongluck/images/blob/main/network/linux/bridge/bridge.png)

- net namespace

![net_namespace](https://github.com/gongluck/images/blob/main/network/linux/namespace/net_namespace.png)

- iptables

![iptables](https://github.com/gongluck/images/blob/main/network/linux/netfilter/iptables.png)

- virtual network

![virtual_network](https://github.com/gongluck/images/blob/main/network/linux/virtual/virtual_network.png)

## 性能优化

- 使用长连接取代短连接，可以显著降低 TCP 建立连接的成本。在每秒请求次数较多时，这样做的效果非常明显。
- 使用内存等方式，来缓存不常变化的数据，可以降低网络 I/O 次数，同时加快应用程序的响应速度。
- 使用 Protocol Buffer 等序列化的方式，压缩网络 I/O 的数据量，可以提高应用程序的吞吐。
- 使用 DNS 缓存、预取、HTTPDNS 等方式，减少 DNS 解析的延迟，也可以提升网络 I/O 的整体速度。

## 性能工具

### 传统工具

- [ss](../tools/command.md#ss)
- [ip](../tools/command.md#ip)
- nstat
- [netstat](../tools/command.md#netstat)
- [sar](../tools/command.md#sar)
- nicstat
- [ethtool](../tools/command.md#ethtool)
- [tcpdump](../tools/command.md#tcpdump)

### BPF工具

- tcpconnect[-bpfcc/.bt]
- tcpaccept[-bpfcc/.bt]
- tcplife[-bpfcc/.bt]
- tcptop[-bpfcc]
- tcpretrans[-bpfcc/.bt]
- gethostlatency[-bpfcc/.bt]