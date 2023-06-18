# nginx

- [nginx](#nginx)
  - [安装](#安装)
  - [使用命令](#使用命令)
  - [nginx配置](#nginx配置)
    - [日志](#日志)
    - [coredump](#coredump)
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

## 相关工具

### [goaccess](https://www.goaccess.cc/)

```shell
goaccess [access.log] [-o report.html] [--log-format COMBINED] [--real-time-html]
```
