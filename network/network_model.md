# 网络模型

- [网络模型](#网络模型)
  - [golang 网络设计](#golang-网络设计)
  - [nginx 网络设计](#nginx-网络设计)
    - [redis 网络设计](#redis-网络设计)
    - [netty 网络设计](#netty-网络设计)

## golang 网络设计

- listen

![go_listen](https://github.com/gongluck/images/blob/main/network/go/go_listen.png)

- accept

![go_accept](https://github.com/gongluck/images/blob/main/network/go/go_accept.png)

- read

![go_read](https://github.com/gongluck/images/blob/main/network/go/go_read.png)

## nginx 网络设计

![nginx_master_worker](https://github.com/gongluck/images/blob/main/network/nginx/nginx_master_worker.png)
![nginx_worker](https://github.com/gongluck/images/blob/main/network/nginx/nginx_worker.png)

### redis 网络设计

- 单线程版本

![redis_network](https://github.com/gongluck/images/blob/main/network/redis/redis_network.png)

- 多线程版本

![redis_multithreads](https://github.com/gongluck/images/blob/main/network/redis/redis_multithreads.png)

### netty 网络设计

![netty](https://github.com/gongluck/images/blob/main/network/netty/netty.png)
