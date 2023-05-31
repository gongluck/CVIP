# openresty

- [openresty](#openresty)
  - [安装](#安装)
  - [OPM](#opm)
  - [LUAROCKS](#luarocks)
  - [Lua Nginx Module](#lua-nginx-module)
  - [LuaJit](#luajit)
  - [NYI](#nyi)
  - [示例](#示例)
  - [火焰图](#火焰图)


## 安装

```shell
# https://openresty.org/cn/linux-packages.html

sudo apt-get -y install --no-install-recommends wget gnupg ca-certificates
wget -O - https://openresty.org/package/pubkey.gpg | sudo gpg --dearmor -o /usr/share/keyrings/openresty.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/openresty.gpg] http://openresty.org/package/ubuntu $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/openresty.list > /dev/null
sudo apt-get update
sudo apt-get -y install openresty
```

## [OPM](https://github.com/openresty/opm)

OPM（OpenResty Package Manager）是 OpenResty 自带的包管理器，安装好 OpenResty 之后，就可以直接使用。

```shell
# 查找库
opm search [...]
```

## LUAROCKS

LuaRocks 是 OpenResty 的另一个包管理器，包含 OpenResty 相关的包，还包含 Lua 世界的库。

```shell
# 查找库
luarocks search [...]
```

## Lua Nginx Module

![Lua Nginx Module](https://github.com/gongluck/images/blob/main/openresty/modules.png)

## LuaJit

- JIT 编译器会从热函数的入口或者热循环的某个位置开始，尝试编译对应的 Lua 代码路径。编译的过程，把 LuaJIT 字节码先转换成 LuaJIT 定义的中间码（IR），然后再生成针对目标体系结构的机器码。
- LuaJIT 的性能优化，本质上就是让尽可能多的 Lua 代码可以被 JIT 编译器生成机器码，而不是回退到 Lua 解释器的解释执行模式。
- 除了兼容 Lua 5.1 的语法并支持 JIT 外，LuaJIT 还紧密结合了 FFI（Foreign Function Interface），可以直接在 Lua 代码中调用外部的 C 函数和使用 C 的数据结构。
- OpenResty 的 master 和 worker 进程中，都包含一个 LuaJIT VM。在同一个进程内的所有协程，都会共享这个 VM，并在这个 VM 中运行 Lua 代码。
- 在 OpenResty 层面，Lua 的协程会与 NGINX 的事件机制相互配合。如果 Lua 代码中出现类似查询 MySQL 数据库这样的 I/O 操作，就会先调用 Lua 协程的 yield 把自己挂起，然后在 NGINX 中注册回调；在 I/O 操作完成（也可能是超时或者出错）后，再由 NGINX 回调 resume 来唤醒 Lua 协程。这样就完成了 Lua 协程和 NGINX 事件驱动的配合，避免在 Lua 代码中写回调。
- [Nginx API for Lua](https://github.com/openresty/lua-nginx-module/#nginx-api-for-lua)

## NYI

- LuaJIT 中 JIT 编译器的实现还不完善，有一些原语还无法编译，当 JIT 编译器在当前代码路径上遇到不支持的操作时，便会退回到解释器模式。
- JIT 编译器不支持的原语，全称为 Not Yet Implemented。
- LuaJIT 自带的 jit.dump 和 jit.v 模块都可以打印出 JIT 编译器工作的过程。
  ```lua
  # init_by_lua 中添加
  local v = require "jit.v"
  v.on("/tmp/jit.log")
  ```
  ```bash
  # 使用 resty
  # -j 是和 LuaJIT 相关的选项
  # dump 和 v 对应着开启 jit.dump 和 jit.v 模式
  resty -j v -e 'for i=1, 1000 do
  local newstr, n, err = ngx.re.gsub("hello, world", "([a-z])[a-z]+", "[$0,$1]", "i")
  end'
  ```

## [示例](../code/openresty)

## 火焰图

```bash
# 编译openresty
apt install -y systemtap-sdt-dev
wget https://openresty.org/download/openresty-1.15.8.2.tar.gz
tar zxvf openresty-1.15.8.2.tar.gz
cd openresty-1.15.8.2
# --without-luajit-gc64
./configure --prefix=/usr/local/openresty --without-luajit-gc64 --with-pcre-jit --with-stream --with-http_v2_module --without-mail_pop3_module --without-mail_imap_module --without-mail_smtp_module --with-http_stub_status_module --with-http_realip_module --with-http_addition_module --with-http_auth_request_module --with-http_secure_link_module --with-http_random_index_module --with-http_gzip_static_module --with-http_sub_module --with-http_dav_module --with-http_flv_module --with-http_mp4_module --with-http_gunzip_module --with-threads --with-dtrace-probes --with-stream --with-http_ssl_module
make
make install
```

```bash
# 安装系统符号
wget http://ddebs.ubuntu.com/pool/main/l/linux/linux-image-unsigned-5.15.0-67-generic-dbgsym_5.15.0-67.74_amd64.ddeb
sudo dpkg -i linux-image-unsigned-5.15.0-67-generic-dbgsym_5.15.0-67.74_amd64.ddeb
# 安装依赖工具
apt install systemtap -y
git clone https://github.com/openresty/stapxx
git clone https://github.com/openresty/openresty-systemtap-toolkit.git
git clone https://github.com/brendangregg/FlameGraph.git
export PATH=~/stapxx:~/FlameGraph:~/openresty-systemtap-toolkit:$PATH
# 生成火焰图
./stapxx/samples/lj-lua-stacks.sxx --arg time=10 --skip-badvars -x 741193 > tmp.bt
fix-lua-bt tmp.bt > flame.bt
stackcollapse-stap.pl flame.bt > flame.cbt
flamegraph.pl --encoding="ISO-8859-1" --title="Lua-land on-CPU flamegraph" flame.cbt > flame.svg
```