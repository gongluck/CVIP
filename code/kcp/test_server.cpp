#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <map>

#include "ikcp.h"

std::string getime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

#define LOG std::cout << __FILE__ << "("<< __LINE__ << ") " << getime() << " "

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib,"WS2_32.lib")
void setcolour(int x) {
	HANDLE h = GetStdHandle(-11);
	SetConsoleTextAttribute(h, x);
}
#define LOGC(c) setcolour(c); LOG
#endif

SOCKET      g_srv_socket;
SOCKADDR_IN g_srv_addr;
const int	BUFSIZE = 1024;
const int	MTU = BUFSIZE;

typedef struct KCPSTRU
{
	ikcpcb* ikcp = nullptr;
	SOCKADDR_IN cli_addr = { 0 };
}kcpStru;
std::mutex	g_mutex;
std::map<uint32_t, kcpStru> g_kcpmap;

#define INTERVEL 1
#define LOCK std::lock_guard<std::mutex> __lock(g_mutex)

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user)
{
	int ret = sendto(g_srv_socket, buf, len, 0, (SOCKADDR*)user, sizeof(SOCKADDR_IN));
	if (ret <= 0)
	{
#ifdef WIN32
		LOGC(4) << "send failed : " << GetLastError() << std::endl;
#endif
	}
	else
	{
#ifdef SHOWRAW
		LOGC(2) << "raw sent " << std::string(buf, len) << std::endl;
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
		LOGC(4) << "WSAStartup failed!" << std::endl;
		return -1;
	}
#endif

	// 创建套接字
	g_srv_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_srv_socket == INVALID_SOCKET)
	{
#ifdef WIN32
		LOGC(4) << "socket() failed, error : " << WSAGetLastError() << std::endl;
		WSACleanup();
#endif
		return -1;
	}

	//设置为非阻塞模式
	u_long imode = 1;
	ret = ioctlsocket(g_srv_socket, FIONBIO, &imode);
	if (ret == -1)
	{
		LOGC(4) << "ioctlsocket failed!" << std::endl;
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
		LOGC(4) << "bind failed!" << std::endl;
#ifdef WIN32
		closesocket(g_srv_socket);
		WSACleanup();
#endif
		return -1;
	}

	LOGC(1) << "server started..." << std::endl;

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
						ret = ikcp_recv(each.second.ikcp, buf, BUFSIZE);
						if (ret < 0)
						{
							continue;
						}
						LOGC(1) << std::string(buf, ret) << std::endl;

						// 发送到KCP"处理程序" 处理结果在kcp->output回调
						ret = ikcp_send(each.second.ikcp, buf, ret);
						if (ret < 0)
						{
							LOGC(4) << "ikcp_send failed, ret : " << ret << std::endl;
							continue;
						}
						// flush
						ikcp_flush(each.second.ikcp);

						ikcp_update(each.second.ikcp, clock());
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

				LOCK;
				// 从客户端接收数据
				SOCKADDR_IN cli_addr;
				int len = sizeof(cli_addr);
				ret = recvfrom(g_srv_socket, buf, BUFSIZE, 0, (SOCKADDR*)&cli_addr, &len);
				if (ret < 0)
				{
#ifdef WIN32
					switch (GetLastError())
					{
					case WSAEWOULDBLOCK:
						break;
					case WSAECONNRESET:
					{
						//LOCK;

						for (auto it = g_kcpmap.begin(); it != g_kcpmap.end();)
						{
							if (memcmp(&it->second.cli_addr, &cli_addr, sizeof(SOCKADDR_IN)) == 0)
							{
								LOGC(2) << "client " << inet_ntoa(cli_addr.sin_addr) << "[" << cli_addr.sin_port << "] close." << std::endl;
								it = g_kcpmap.erase(it);
								LOGC(2) << "left " << g_kcpmap.size() << " clients." << std::endl;
							}
							else
							{
								++it;
							}
						}
					}
					break;
					default:
						LOG << "recvfrom failed : " << GetLastError() << std::endl;
						break;
					}
#endif
					continue;
				}
#ifdef SHOWRAW
				LOGC(2) << "raw recv " << std::string(buf, ret) << std::endl;
#endif

				// 解析conn
				uint32_t conv = ikcp_getconv(buf);
				{
					//LOCK;

					auto it = g_kcpmap.find(conv);
					if (it == g_kcpmap.end())
					{
						// 创建KCP实例
						g_kcpmap[conv].cli_addr = cli_addr;
						g_kcpmap[conv].ikcp = ikcp_create(conv, (void*)&g_kcpmap[conv].cli_addr);
						g_kcpmap[conv].ikcp->output = udp_output;
						g_kcpmap[conv].ikcp->stream = 0;
						LOGC(2) << "now " << g_kcpmap.size() << " clients." << std::endl;
						// 启动快速模式
						// 第二个参数 nodelay-启用以后若干常规加速将启动
						// 第三个参数 interval为内部处理时钟，默认设置为 10ms
						// 第四个参数 resend为快速重传指标，设置为2
						// 第五个参数 为是否禁用常规流控，这里禁止
						//ikcp_nodelay(g_kcpmap[conv].ikcp, 1, 10, 2, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟10毫秒.
						//普通模式
						ikcp_nodelay(g_kcpmap[conv].ikcp, 0, 40, 0, 0);
						ikcp_wndsize(g_kcpmap[conv].ikcp, 2, 2);
						ikcp_setmtu(g_kcpmap[conv].ikcp, MTU);
					}
				}

				// 输入到KCP"处理程序"
				ret = ikcp_input(g_kcpmap[conv].ikcp, buf, ret);
				if (ret < 0)
				{
					LOGC(4) << "ikcp_input failed : " << ret << std::endl;
					continue;
				}
				// flush
				ikcp_flush(g_kcpmap[conv].ikcp);

				ikcp_update(g_kcpmap[conv].ikcp, clock());
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
			ikcp_release(each.second.ikcp);
			each.second.ikcp = nullptr;
		}
		g_kcpmap.clear();
	}

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}