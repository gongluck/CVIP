# nginx

- [nginx](#nginx)
  - [安装](#安装)
  - [使用命令](#使用命令)
  - [nginx配置](#nginx配置)
    - [日志](#日志)
    - [coredump](#coredump)
    - [环境上下文](#环境上下文)
    - [worker进程](#worker进程)
    - [ngx\_http\_core\_module](#ngx_http_core_module)
  - [相关工具](#相关工具)
    - [goaccess](#goaccess)


## 安装

```bash
# https://nginx.org/

wget https://nginx.org/download/nginx-1.24.0.tar.gz
tar -zxvf nginx-1.24.0.tar.gz
cd nginx-1.24.0

apt-get install -y gcc g++ libpcre2 libpcre2-dev zlib1g zlib1g-dev openssl libssl-dev

./configure
make -j8
make install
```

## 使用命令

```bash
/usr/local/nginx/sbin/nginx [-c conf_path] [-p prefix_path] [-t(test_conf)] [-s reopen/reload/quit/stop]
```

## nginx配置

### 日志

- error日志

  ```nginx
  error_log filepath/stderr/[/dev/null] level;
  ```

- 指定客户端debug日志

  ```nginx
  debug_connection ip/cidr;
  ```

### coredump

- coredump文件大小

  ```nginx
  worker_rlimit_core size; 
  ```

- coredump目录

  ```nginx
  working_directory path;
  ```

### 环境上下文

- 定义环境变量

  ```nginx
  env var = value;
  ```

- 嵌入其他配置文件

  ```nginx
  include file;
  ```

### worker进程

- worker进程用户和用户组

  ```nginx
  user username [groupname];
  ```

- worker进程最大打开文件句柄数

  ```nginx
  worker_rlimit_nofile limit;
  ```

- worker进程个数

  ```nginx
  worker_processes n;
  ```

- 绑定进程到指定CPU核

  ```nginx
  worker_cpu_affinity cpumask [cpumask...];
  ```

- worker进程优先级

  ```nginx
  worker_priority nice;
  ```

- 每个worker最大连接数

  ```nginx
  worker_connections n;
  ```

- 打开accept锁

  ```nginx
  accept_mutex on;
  ```

- 可获取accept锁的间隔

  ```nginx
  accept_mutex_delay n;
  ```

- 批量建立新连接

  ```nginx
  multi_accept on;
  ```

### [ngx_http_core_module](https://nginx.org/en/docs/http/ngx_http_core_module.html)

- listen

  ```nginx
  listen address[:port] [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
  listen port [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
  listen unix:path [default_server] [ssl] [http2 | quic] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
  ```

- server_name

  ```nginx
  # 匹配请求中的 Host
  # server_name ""; 匹配没有 Host 的请求
  server_name name ...;
  ```

- server_name_in_redirect

  ```nginx
  # 重定向请求时使用 server_name 的第一个项修改 Host
  server_name_in_redirect on;
  ```

- location

  ```nginx
  # =   完全匹配
  # ~   大小写敏感  
  # ~*  忽略大小写
  # ^~  前部匹配
  location [ = | ~ | ~* | ^~ ] uri { ... }
  # @   内部重定向
  location @name { ... }
  ```

- root

  ```nginx
  # location 匹配后解析为 path/uri
  root path;
  ```

- alias

  ```nginx
  # location 匹配后解析为 path/filename
  alias path;
  ```

- error_page

  ```nginx
  error_page code ... [=[response]] uri;
  ```

- try_files

  ```nginx
  try_files file ... uri;
  try_files file ... =code;
  ```

- client_body_in_file_only

  ```nginx
  # http 包体保存到文件
  client_body_in_file_only on | clean | off;
  ```

- client_body_in_single_buffer

  ```nginx
  # http 包体保存到内存
  client_body_in_single_buffer on | off;
  ```

- client_body_buffer_size

  ```nginx
  # 存储包体的缓冲区大小，超出后需写入磁盘
  client_body_buffer_size size;
  ```

## 相关工具

### [goaccess](https://www.goaccess.cc/)

```shell
goaccess [access.log] [-o report.html] [--log-format COMBINED] [--real-time-html]
```
