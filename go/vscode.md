# ```VS Code```搭建```Go```环境

- [```VS Code```搭建```Go```环境](#vs-code搭建go环境)
  - [设置代理](#设置代理)
  - [智能提示](#智能提示)

## 设置代理

```shell
#win
SET GO111MODULE=on
#linux
export GO111MODULE=on

go env -w GOPROXY=https://goproxy.cn
go env -w GOPROXY=https://github.com/goproxy
go env -w GOPROXY=https://mirrors.aliyun.com/goproxy
go env -w GOPROXY=https://goproxy.io
go env -w GOPROXY=https://goproxy.cn,direct
```

## 智能提示

```shell
go get -u github.com/nsf/gocode
go get -u github.com/uudashr/gopkgs/cmd/gopkgs
go get -u github.com/ramya-rao-a/go-outline
go get -u github.com/acroca/go-symbols
go get -u golang.org/x/tools/cmd/guru
go get -u golang.org/x/tools/cmd/gorename
go get -u github.com/fatih/gomodifytags
go get -u github.com/haya14busa/goplay/cmd/goplay
go get -u github.com/josharian/impl
go get -u github.com/rogpeppe/godef
go get -u sourcegraph.com/sqs/goreturns
go get -u github.com/golang/lint/golint
go get -u github.com/cweill/gotests
go get -u github.com/derekparker/delve/cmd/dlv
```