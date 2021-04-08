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
char        g_buf[BUFSIZE] = { 0 };
std::mutex	g_mutex;
std::map<uint32_t, ikcpcb*> g_kcpmap;

#define LOCK std::lock_guard<std::mutex> __lock(g_mutex)

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user)
{
	//LOG << "send client " << inet_ntoa(g_cli_addr.sin_addr) << "[" << g_cli_addr.sin_port << "]" << std::endl;
	int ret = sendto(g_srv_socket, buf, len, 0, (SOCKADDR*)user, sizeof(g_cli_addr));
	if (ret <= 0)
	{
		LOG << "send failed, ret : " << ret << std::endl;
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

	std::thread th([&]
		{
			int ret;
			char buf[BUFSIZE] = { 0 };
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				{
					LOCK;

					for (const auto& each : g_kcpmap)
					{
						// 从KCP"处理程序"获取用户层数据
						ret = ikcp_recv(each.second, buf, BUFSIZE);
						if (ret < 0)
						{
							continue;;
						}
						LOG << "recv[" << each.first << "] " << std::string(buf, ret) << std::endl;

						// 发送到KCP"处理程序" 处理结果在kcp->output回调
						ret = ikcp_send(each.second, buf, ret);
						if (ret < 0)
						{
							LOG << "ikcp_send failed, ret : " << ret << std::endl;
						}
						// flush
						ikcp_flush(each.second);

						ikcp_update(each.second, clock());
					}
				}
			}
			
		}
	);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		// 从客户端接收数据
		while (true)
		{
			int len = sizeof(g_cli_addr);
			ret = recvfrom(g_srv_socket, g_buf, BUFSIZE, 0, (SOCKADDR*)&g_cli_addr, &len);
			if (ret < 0)
			{
				break;
			}

			//LOG << "recv client " << inet_ntoa(g_cli_addr.sin_addr) << "[" << g_cli_addr.sin_port << "]" << std::endl;

			// 解析conn
			uint32_t conv = ikcp_getconv(g_buf);
			{
				LOCK;

				auto it = g_kcpmap.find(conv);
				if (it == g_kcpmap.end())
				{
					// 创建KCP实例
					g_kcpmap[conv] = ikcp_create(conv, (void*)&g_cli_addr);
					g_kcpmap[conv]->output = udp_output;
				}
			}
			
			// 输入到KCP"处理程序"
			ikcp_input(g_kcpmap[conv], g_buf, ret);
			// flush
			ikcp_flush(g_kcpmap[conv]);

			ikcp_update(g_kcpmap[conv], clock());
		}
	}

	closesocket(g_srv_socket);

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}