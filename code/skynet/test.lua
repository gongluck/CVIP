-- ./skynet /mnt/e/Code/CVIP/code/skynet/test.conf

local skynet = require "skynet"
local socket = require "skynet.socket"

local function event_loop(clientfd)
    while true do
        local data = socket.readline(clientfd)--从网络获取 以\n为分隔符的数据包
        if not data then
            return
        end
        print(clientfd, "recv:", data)
        socket.write(clientfd, data.."\n")
    end
end

local function accept(clientfd, addr)-- 回调函数的作用 就是可以将 fd绑定到其他actor
    print("accept a connect:", clientfd, addr)
    socket.start(clientfd) -- 将clientfd注册到epoll
    skynet.fork(event_loop, clientfd) -- 实现一个简单的echo服务，可以通过 telnet 127.0.0.1 8001来连接skynet
end

skynet.start(function ()
    local listenfd = socket.listen("0.0.0.0", 8001) -- socket bind listen 
    socket.start(listenfd, accept) -- 将listenfd注册到epoll，收到连接会回调accept函数
    -- skynet.fork(function ()
    --     local connfd = socket.open("127.0.0.1", 6379)-- skynet作为客户端连接 redis-server；连接成功 fd 注册到epoll
    --     if not connfd then
    --         print("网络连接失败")
    --         return
    --     end
    --     socket.write(connfd, "*1\r\n$4\r\nPING\r\n")--这里实现了一个简单的ping pong
    --     local data = socket.readline(connfd, "\r\n")
    --     print("recv redis-server data:", data)
    -- end)
end)
