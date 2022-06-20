# Windows搭建Linux开发环境

- [Windows搭建Linux开发环境](#windows搭建linux开发环境)
  - [MSYS2(Minimal SYStem 2)](#msys2minimal-system-2)
    - [安装修改MSYS2](#安装修改msys2)
    - [使用VS命令行运行msys2](#使用vs命令行运行msys2)

## MSYS2(Minimal SYStem 2)

### 安装修改MSYS2

- 把安装⽬录下的```msys2_shell.cmd```中注释掉的```rem set MSYS2_PATH_TYPE=inherit```改成启用，这是为了将```vs```的环境继承给```MSYS2```
- 编辑```/etc/pacman.d/mirrorlist.mingw32```，在⽂件开头添加：
```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686/
Server = http://mirrors.ustc.edu.cn/msys2/mingw/i686/
```
- 编辑```/etc/pacman.d/mirrorlist.mingw64```，在⽂件开头添加：
```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64/
Server = http://mirrors.ustc.edu.cn/msys2/mingw/x86_64/
```
- 编辑```/etc/pacman.d/mirrorlist.msys```，在⽂件开头添加：
```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch/
Server = http://mirrors.ustc.edu.cn/msys2/msys/$arch/
```
- 重命名```msys64/usr/bin/link.exe```为```msys64/usr/bin/link.exe.bak```

### 使用VS命令行运行msys2
```shell
#32位
msys2_shell.cmd -mingw32
#64位
msys2_shell.cmd -mingw64
# msys2中执行
pacman -Sy
#32位
pacman -S mingw-w64-i686-toolchain
pacman -S mingw-w64-i686-nasm mingw-w64-i686-gcc mingw-w64-i686-SDL2
#64位
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-nasm mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2
#通用
pacman -S git
pacman -S make
pacman -S automake
pacman -S autoconf
pacman -S perl
pacman -S libtool
pacman -S pkg-config
```
