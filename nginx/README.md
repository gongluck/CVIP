# nginx

- [nginx](#nginx)
  - [安装](#安装)
  - [使用命令](#使用命令)
  - [相关工具](#相关工具)
    - [goaccess](#goaccess)


## 安装

```bash
# https://nginx.org/

wget https://nginx.org/download/nginx-1.24.0.tar.gz
tar -zxvf nginx-1.24.0.tar.gz 
cd nginx-1.24.0
./configure --prefix=./build
make -j8
make install
```

## 使用命令

```bash
./build/sbin/nginx -c ./conf/nginx.conf [-p (logpath)] [-s reload/quit]
```

## 相关工具

### [goaccess](https://www.goaccess.cc/)

```shell
goaccess [access.log] [-o report.html] [--log-format COMBINED] [--real-time-html]
```
