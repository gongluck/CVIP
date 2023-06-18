# 网络配置

- [网络配置](#网络配置)
  - [/etc/sysctl.conf(修改后运行sysctl -p生效)](#etcsysctlconf修改后运行sysctl--p生效)
    - [fs](#fs)
      - [`fs.file-max = n`](#fsfile-max--n)
    - [net](#net)
      - [core](#core)
        - [`net.core.netdev_max_backlog = n`](#netcorenetdev_max_backlog--n)
        - [`net.core.rmem_default = n`](#netcorermem_default--n)
        - [`net.core.wmem_default = n`](#netcorewmem_default--n)
        - [`net.core.rmem_max = n`](#netcorermem_max--n)
        - [`net.core.wmem_max = n`](#netcorewmem_max--n)
      - [ipv4](#ipv4)
        - [`net.ipv4.ip_local_port_range = m n`](#netipv4ip_local_port_range--m-n)
        - [tcp](#tcp)
          - [`net.ipv4.tcp_tw_reuse = 1`](#netipv4tcp_tw_reuse--1)
          - [`net.ipv4.tcp_max_tw_buckets = n`](#netipv4tcp_max_tw_buckets--n)
          - [`net.ipv4.tcp_keepalive_time = n`](#netipv4tcp_keepalive_time--n)
          - [`net.ipv4.tcp_fin_timeout = n`](#netipv4tcp_fin_timeout--n)
          - [`net.ipv4.tcp_max_syn_backlog = n`](#netipv4tcp_max_syn_backlog--n)
          - [`net.ipv4.tcp_rmem = min default max`](#netipv4tcp_rmem--min-default-max)
          - [`net.ipv4.tcp_wmem = min defualt max`](#netipv4tcp_wmem--min-defualt-max)
          - [`net.ipv4.tcp_syncookies = 1`](#netipv4tcp_syncookies--1)


## /etc/sysctl.conf(修改后运行sysctl -p生效)

### fs

#### `fs.file-max = n`

- 系统级别的能够打开的文件句柄的数量。

### net

#### core

##### `net.core.netdev_max_backlog = n`

- 网卡接收队列长度。用于在内核处理比网卡接收慢时缓冲数据。

##### `net.core.rmem_default = n`

- 内核套接字接收缓冲区默认大小。
- 会覆盖net.ipv4.tcp_rmem的对应值。

##### `net.core.wmem_default = n`

- 内核套接字发送缓冲区默认大小。
- 会覆盖net.ipv4.tcp_wmem的对应值。

##### `net.core.rmem_max = n`

- 内核套接字接收缓冲区最大大小。
- 会覆盖net.ipv4.tcp_rmem的对应值。

##### `net.core.wmem_max = n`

- 内核套接字发送缓冲区最大大小。
- 会覆盖net.ipv4.tcp_wmem的对应值。

#### ipv4

##### `net.ipv4.ip_local_port_range = m n`

- 本地端口选择范围。

##### tcp

###### `net.ipv4.tcp_tw_reuse = 1`

- 允许[TIME_WAIT](./tcp.md#time_wait)状态的套接字用于创建新的TCP连接。

###### `net.ipv4.tcp_max_tw_buckets = n`

- 允许[TIME_WAIT](./tcp.md#time_wait)套接字的最大数量。

###### `net.ipv4.tcp_keepalive_time = n`

- keepalive启用时，TCP发送keepalive的频率。

###### `net.ipv4.tcp_fin_timeout = n`

- 服务器主动关闭连接时，套接字保持在[FIN_WAIT2](./tcp.md#time_wait)的最长时间。

###### `net.ipv4.tcp_max_syn_backlog = n`

- TCP三次握手阶段接收SYN队列的最大长度。

###### `net.ipv4.tcp_rmem = min default max`

- TCP接收缓冲区的最小值、默认值、最大值。

###### `net.ipv4.tcp_wmem = min defualt max`

- TCP发送缓冲区的最小值、默认值、最大值。

###### `net.ipv4.tcp_syncookies = 1`

- 开启SYN Cookies，当出现SYN等待队列溢出时，启用cookies来处理。
- SYN Cookie对TCP服务器端的三次握手协议作一些修改，专门用来防范SYN Flood攻击的一种手段。在TCP服务器收到TCP SYN包并返回TCP SYN+ACK包时，不分配一个专门的数据区，而是根据这个SYN包计算出一个cookie值。在收到TCP ACK包时，TCP服务器在根据那个cookie值检查这个TCP ACK包的合法性。如果合法，再分配专门的数据区进行处理未来的TCP连接。