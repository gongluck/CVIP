# Go 更新安装

## 删除旧版

```shell
sudo rm -rf /usr/local/go
sudo apt-get remove golang
sudo apt-get remove golang-go
sudo apt-get autoremove
```

## 下载二进制

```shell
wget https://golang.google.cn/dl/go1.21.0.linux-arm64.tar.gz
sudo tar -zxvf go1.21.0.linux-arm64.tar.gz -C /usr/local
```

## 设置环境

```shell
#打开profile
sudo vim /etc/profile
#export GOROOT=/usr/local/go
#export GOPATH=$HOME/go
#export GOBIN=$GOPATH/bin
#export PATH=$GOPATH:$GOBIN:$GOROOT/bin:$PATH
source /etc/profile

cd ~
sudo vim .bashrc
#source /etc/profile

go env -w GOPROXY="https://goproxy.cn"
go env -w GO111MODULE=on
```
