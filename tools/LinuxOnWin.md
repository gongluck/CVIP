# Windows 搭建 Linux 开发环境

- [Windows 搭建 Linux 开发环境](#windows-搭建-linux-开发环境)
  - [MSYS2(Minimal SYStem 2)](#msys2minimal-system-2)
    - [安装修改 MSYS2](#安装修改-msys2)
    - [使用 VS 命令行运行 msys2](#使用-vs-命令行运行-msys2)
  - [TDM-GCC](#tdm-gcc)
    - [安装配置 TDM-GCC-64](#安装配置-tdm-gcc-64)
    - [运行 msys.bat 进入环境](#运行-msysbat-进入环境)

## MSYS2(Minimal SYStem 2)

### 安装修改 MSYS2

- 把安装⽬录下的`msys2_shell.cmd`中注释掉的`rem set MSYS2_PATH_TYPE=inherit`改成启用，这是为了将`vs`的环境继承给`MSYS2`
- 编辑`/etc/pacman.d/mirrorlist.mingw32`，在⽂件开头添加：

```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686/
Server = http://mirrors.ustc.edu.cn/msys2/mingw/i686/
```

- 编辑`/etc/pacman.d/mirrorlist.mingw64`，在⽂件开头添加：

```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64/
Server = http://mirrors.ustc.edu.cn/msys2/mingw/x86_64/
```

- 编辑`/etc/pacman.d/mirrorlist.msys`，在⽂件开头添加：

```shell
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch/
Server = http://mirrors.ustc.edu.cn/msys2/msys/$arch/
```

- 重命名`/usr/bin/link.exe`为`/usr/bin/link.exe.bak`

### 使用 VS 命令行运行 msys2

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

## TDM-GCC

### 安装配置 TDM-GCC-64

- 安装（解压）`msys`到`TDM-GCC`的安装目录中
- 将`call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"`加到`msys.bat`的第一行中
- 复制`yasm-1.3.0-win64.exe`（重命名为`yasm.exe`）到`msys`的`bin`目录下
- 重命名`msys`的`bin`目录下的`link.exe`（`linkold.exe`）

### 运行 msys.bat 进入环境
