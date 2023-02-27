# Linux 网络

- [Linux 网络](#linux-网络)
  - [命令配置](#命令配置)
  - [Linux 网络工作图示](#linux-网络工作图示)
    - [套接字结构](#套接字结构)
    - [TCP](#tcp)
    - [网络收包](#网络收包)
    - [网络发包](#网络发包)
    - [epoll](#epoll)
    - [虚拟网络](#虚拟网络)

## 命令配置

- [ethtool](../tools/command.md#ethtool)
- [特殊文件](../tools/command.md#特殊文件)

## Linux 网络工作图示

### 套接字结构

![socket_struct](https://github.com/gongluck/images/blob/main/network/linux/socket/socket_struct.png)

### TCP

- TCP 状态轮转

![tcp_state](https://github.com/gongluck/images/blob/main/network/tcp/tcp_state.png)

- TCP 三次握手

![tcp_handshake](https://github.com/gongluck/images/blob/main/network/tcp/tcp_handshake.png)

### 网络收包

- 收包总流程

![recvpack](https://github.com/gongluck/images/blob/main/network/linux/recv/recvpack.png)

- 处理软中断

![ksoftirqd_recvpack](https://github.com/gongluck/images/blob/main/network/linux/recv/ksoftirqd_recvpack.png)

- 收包流程源码

![recvpack_source](https://github.com/gongluck/images/blob/main/network/linux/recv/recvpack_source.png)

- recvfrom 调用

![recvfrom](https://github.com/gongluck/images/blob/main/network/linux/recv/recvfrom.png)

### 网络发包

- 发包主流程

![sendpack](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack.png)

- 发包流程源码

![sendpack_source](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack_source.png)
![sendpack_end_source](https://github.com/gongluck/images/blob/main/network/linux/send/sendpack_end_source.png)

- sendto 调用

![sendto](https://github.com/gongluck/images/blob/main/network/linux/send/sendto.png)

### epoll

- epoll 结构

![epoll](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll.png)
![epoll_struct](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll_struct.png)

- epoll_wait 调用

![epoll_wait](https://github.com/gongluck/images/blob/main/network/linux/epoll/epoll_wait.png)

### 虚拟网络

- bridge

![bridge](https://github.com/gongluck/images/blob/main/network/bridge/bridge.png)

- net namespace

![net_namespace](https://github.com/gongluck/images/blob/main/network/namespace/net_namespace.png)

- iptables

![iptables](https://github.com/gongluck/images/blob/main/network/netfilter/iptables.png)

- virtual network

![virtual_network](https://github.com/gongluck/images/blob/main/network/virtual/virtual_network.png)
