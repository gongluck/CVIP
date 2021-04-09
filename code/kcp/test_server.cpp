#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <map>

#include "ikcp.h"

#define LOG std::cout << __FILE__ << "("<< __LINE__ << ")"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
#endif

SOCKET      g_srv_socket;
SOCKADDR_IN g_srv_addr;
SOCKADDR_IN g_cli_addr;
const int	BUFSIZE = 1024;
std::mutex	g_mutex;
std::map<uint32_t, ikcpcb*> g_kcpmap;

#define INTERVEL 10
#define LOCK std::lock_guard<std::mutex> __lock(g_mutex)

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user)
{
	//LOG << "send client " << inet_ntoa(g_cli_addr.sin_addr) << "[" << g_cli_addr.sin_port << "]" << std::endl;
	int ret = sendto(g_srv_socket, buf, len, 0, (SOCKADDR*)user, sizeof(g_cli_addr));
	if (ret <= 0)
	{
#ifdef WIN32
		LOG << "send failed : " << GetLastError() << std::endl;
#endif
	}
	return 0;
}

int main()
{
	int ret;

#ifdef WIN32
	WSADATA wsd;
	// 初始化套接字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		LOG << "WSAStartup failed!" << std::endl;
		return -1;
	}
#endif

	// 创建套接字
	g_srv_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_srv_socket == INVALID_SOCKET)
	{
#ifdef WIN32
		LOG << "socket() failed, error : " << WSAGetLastError() << std::endl;
		WSACleanup();
#endif
		return -1;
	}

	//设置为非阻塞模式
	u_long imode = 1;
	ret = ioctlsocket(g_srv_socket, FIONBIO, &imode);
	if (ret == -1)
	{
		LOG << "ioctlsocket failed!" << std::endl;
#ifdef WIN32
		closesocket(g_srv_socket);
		WSACleanup();
#endif
		return -1;
	}

	// 设置服务器地址
	g_srv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	g_srv_addr.sin_family = AF_INET;
	g_srv_addr.sin_port = htons(5000);

	// 绑定套接字
	ret = bind(g_srv_socket, (SOCKADDR*)&g_srv_addr, sizeof(g_srv_addr));
	if (ret == -1)
	{
		LOG << "bind failed!" << std::endl;
#ifdef WIN32
		closesocket(g_srv_socket);
		WSACleanup();
#endif
		return -1;
	}

	LOG << "server started..." << std::endl;

	bool exit = false;
	std::thread th1([&]
		{
			int ret;
			char buf[BUFSIZE] = { 0 };
			while (!exit)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(INTERVEL));
				{
					LOCK;

					for (const auto& each : g_kcpmap)
					{
						// 从KCP"处理程序"获取用户层数据
						ret = ikcp_recv(each.second, buf, BUFSIZE);
						if (ret < 0)
						{
							continue;
						}
						LOG << "recv[" << each.first << "] " << std::string(buf, ret) << std::endl;

						// 发送到KCP"处理程序" 处理结果在kcp->output回调
						ret = ikcp_send(each.second, buf, ret);
						if (ret < 0)
						{
							LOG << "ikcp_send failed, ret : " << ret << std::endl;
							continue;
						}
						// flush
						ikcp_flush(each.second);

						ikcp_update(each.second, clock());
					}
				}
			}

		}
	);

	std::thread th2([&]
		{
			int ret;
			char buf[BUFSIZE] = { 0 };
			while (!exit)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(INTERVEL));

				// 从客户端接收数据
				int len = sizeof(g_cli_addr);
				ret = recvfrom(g_srv_socket, buf, BUFSIZE, 0, (SOCKADDR*)&g_cli_addr, &len);
				if (ret < 0)
				{
#ifdef WIN32
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						LOG << "recvfrom failed : " << GetLastError() << std::endl;
					}
#endif
					continue;
				}

				//LOG << "recv client " << inet_ntoa(g_cli_addr.sin_addr) << "[" << g_cli_addr.sin_port << "]" << std::endl;

				// 解析conn
				uint32_t conv = ikcp_getconv(buf);
				{
					LOCK;

					auto it = g_kcpmap.find(conv);
					if (it == g_kcpmap.end())
					{
						// 创建KCP实例
						g_kcpmap[conv] = ikcp_create(conv, (void*)&g_cli_addr);
						g_kcpmap[conv]->output = udp_output;
						g_kcpmap[conv]->stream = 0;
						// 启动快速模式
						// 第二个参数 nodelay-启用以后若干常规加速将启动
						// 第三个参数 interval为内部处理时钟，默认设置为 10ms
						// 第四个参数 resend为快速重传指标，设置为2
						// 第五个参数 为是否禁用常规流控，这里禁止
						ikcp_nodelay(g_kcpmap[conv], 1, 10, 2, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟10毫秒.
						//普通模式
						//ikcp_nodelay(g_kcpmap[conv], 0, 40, 0, 0);
						ikcp_wndsize(g_kcpmap[conv], 2, 2);
						ikcp_setmtu(g_kcpmap[conv], 1200);
					}
				}

				// 输入到KCP"处理程序"
				ret = ikcp_input(g_kcpmap[conv], buf, ret);
				if (ret < 0)
				{
					LOG << "ikcp_input failed : " << ret << std::endl;
					continue;
				}
				// flush
				ikcp_flush(g_kcpmap[conv]);

				ikcp_update(g_kcpmap[conv], clock());
			}
		}
	);

	char line[1024] = { 0 };
	while (std::cin.getline(line, sizeof(line)))
	{
		if (line[0] == 'q')
		{
			exit = true;
			break;
		}
	}

	if (th1.joinable())
	{
		th1.join();
	}
	if (th2.joinable())
	{
		th2.join();
	}
	closesocket(g_srv_socket);
	{
		LOCK;

		for (auto& each : g_kcpmap)
		{
			ikcp_release(each.second);
			each.second = nullptr;
		}
		g_kcpmap.clear();
	}

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}