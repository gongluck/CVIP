# 编译```Go```源码

1.安装```Go```

```shell
#ubuntu
sudo apt-get install golang
```

2.获取```Go```源码

```shell
git clone https://github.com/golang/go.git
```

3.运行编译脚本

```shell
cd go/src
#ubuntu
sudo ./make.bash
```

4.查看```Go```依赖库

```shell
#ubuntu
readelf -d ../bin/go | grep NEEDED
#输出:
# 0x0000000000000001 (NEEDED)             Shared library: [libpthread.so.0]
# 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
```