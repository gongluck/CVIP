# Linux网络

- [Linux网络](#linux网络)
  - [命令配置](#命令配置)
  - [Linux网络工作图示](#linux网络工作图示)
    - [套接字结构](#套接字结构)
    - [TCP](#tcp)
    - [网络收包](#网络收包)
    - [网络发包](#网络发包)
    - [epoll](#epoll)
    - [虚拟网络](#虚拟网络)
  - [linux网络特性](#linux网络特性)
    - [```SO_REUSEADDR```/```SO_REUSEPORT```](#so_reuseaddrso_reuseport)
    - [```tcpdump```](#tcpdump)
  - [网络模型](#网络模型)
    - [golang网络设计](#golang网络设计)
    - [nginx网络设计](#nginx网络设计)
    - [redis网络设计](#redis网络设计)
    - [netty网络设计](#netty网络设计)

## 命令配置

- [ethtool](../tools/command.md#ethtool)
- [特殊文件](../tools/command.md#特殊文件)

## Linux网络工作图示

### 套接字结构

![socket_struct](https://github.com/gongluck/images/blob/main/Network/socket_struct.png)

### TCP

- ```TCP```状态轮转

  ![tcp_state](https://github.com/gongluck/images/blob/main/Network/tcp_state.png)

- ```TCP```三次握手

  ![tcp_handshake](https://github.com/gongluck/images/blob/main/Network/tcp_handshake.png)

### 网络收包

- 收包总流程

  ![recvpack](https://github.com/gongluck/images/blob/main/Network/recvpack.png)

- 处理软中断

  ![ksoftirqd_recvpack](https://github.com/gongluck/images/blob/main/Network/ksoftirqd_recvpack.png)

- 收包流程源码

  ![recvpack_source](https://github.com/gongluck/images/blob/main/Network/recvpack_source.png)

- ```recvfrom```调用

  ![recvfrom](https://github.com/gongluck/images/blob/main/Network/recvfrom.png)

### 网络发包

- 发包主流程

  ![sendpack](https://github.com/gongluck/images/blob/main/Network/sendpack.png)

- 发包流程源码

  ![sendpack_source](https://github.com/gongluck/images/blob/main/Network/sendpack_source.png)

  ![sendpack_end_source](https://github.com/gongluck/images/blob/main/Network/sendpack_end_source.png)

- ```sendto```调用

  ![sendto](https://github.com/gongluck/images/blob/main/Network/sendto.png)


### epoll

- ```epoll```结构

  ![epoll](https://github.com/gongluck/images/blob/main/Network/epoll.png)

  ![epoll_struct](https://github.com/gongluck/images/blob/main/Network/epoll_struct.png)

- ```epoll_wait```调用

  ![epoll_wait](https://github.com/gongluck/images/blob/main/Network/epoll_wait.png)

### 虚拟网络

- ```bridge```

  ![bridge](https://github.com/gongluck/images/blob/main/Network/bridge.png)

- ```net namespace```

  ![net_namespace](https://github.com/gongluck/images/blob/main/Network/net_namespace.png)

- ```iptables```

  ![iptables](https://github.com/gongluck/images/blob/main/Network/iptables.png)

- ```virtual network```

  ![virtual_natwork](https://github.com/gongluck/images/blob/main/Network/virtual_natwork.png)

## linux网络特性

### ```SO_REUSEADDR```/```SO_REUSEPORT```

[Linux内核中reuseport的演进](https://segmentfault.com/a/1190000020524323)

### ```tcpdump```

[tcpdump原理](../code/tcpdump)

![tcpdump捕获接收包](https://github.com/gongluck/images/blob/main/Network/tcpdump_recv.png)

![tcpdump捕获发送包](https://github.com/gongluck/images/blob/main/Network/tcpdump_send.png)

## 网络模型

### golang网络设计

- ```listen```

  ![go_listen](https://github.com/gongluck/images/blob/main/Network/go_listen.png)

- ```accept```

  ![go_accept](https://github.com/gongluck/images/blob/main/Network/go_accept.png)

- ```read```

  ![go_read](https://github.com/gongluck/images/blob/main/Network/go_read.png)

### nginx网络设计

![nginx_master_worker](https://github.com/gongluck/images/blob/main/Network/nginx_master_worker.png)

![nginx_worker](https://github.com/gongluck/images/blob/main/Network/nginx_worker.png)

### redis网络设计

- 单线程版本

  ![redis_network](https://github.com/gongluck/images/blob/main/Network/redis_network.png)

- 多线程版本

  ![redis_multithreads](https://github.com/gongluck/images/blob/main/Network/redis_multithreads.png)

### netty网络设计

![netty](https://github.com/gongluck/images/blob/main/Network/netty.png)
