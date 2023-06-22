# nginx

- [nginx](#nginx)
  - [安装](#安装)
  - [使用命令](#使用命令)
  - [模块](#模块)
    - [ngx\_core\_module](#ngx_core_module)
    - [ngx\_http\_core\_module](#ngx_http_core_module)
    - [ngx\_http\_proxy\_module](#ngx_http_proxy_module)
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

## 模块

### [ngx_core_module](https://nginx.org/en/docs/ngx_core_module.html)

### [ngx_http_core_module](https://nginx.org/en/docs/http/ngx_http_core_module.html)

### [ngx_http_proxy_module](https://nginx.org/en/docs/http/ngx_http_proxy_module.html)

## 相关工具

### [goaccess](https://www.goaccess.cc/)

```shell
goaccess [access.log] [-o report.html] [--log-format COMBINED] [--real-time-html]
```
