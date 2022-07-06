# 编译Go源码

- [编译Go源码](#编译go源码)
  - [安装Go](#安装go)
  - [获取Go源码](#获取go源码)
  - [运行编译脚本](#运行编译脚本)
  - [查看Go依赖库](#查看go依赖库)

## 安装Go

```shell
#ubuntu
sudo apt-get install golang
```

## 获取Go源码

```shell
git clone https://github.com/golang/go.git
```

## 运行编译脚本

```shell
cd go/src
#ubuntu
sudo ./make.bash
```

## 查看Go依赖库

```shell
#ubuntu
# readelf -d ../bin/go | grep NEEDED
0x0000000000000001 (NEEDED)             Shared library: [libpthread.so.0]
0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
```