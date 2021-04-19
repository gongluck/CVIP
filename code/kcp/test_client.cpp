#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>

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

SOCKET      g_cli_socket;
SOCKADDR_IN g_srv_addr;
SOCKADDR_IN g_cli_addr;
const int	BUFSIZE = 1024;
const int	MTU = BUFSIZE;
#define SENDINTERVEL 1
#define RECVINTERVEL 1

std::mutex	g_mutex;
#define LOCK std::lock_guard<std::mutex> __lock(g_mutex)

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user)
{
	int ret = sendto(g_cli_socket, buf, len, 0, (sockaddr*)user, sizeof(g_srv_addr));
	if (ret <= 0)
	{
		LOGC(4) << "send failed : " << GetLastError() << std::endl;
	}
	else
	{
#ifdef SHOWRAW
		LOGC(3) << "raw sent : " << std::string(buf, len) << std::endl;
#endif
	}
	return 0;
}

int main(void)
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
	g_cli_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_cli_socket == -1)
	{
#ifdef WIN32
		LOGC(4) << "create socket failed, error : " << WSAGetLastError() << std::endl;
		WSACleanup();
#endif
		return -1;
	}

	//设置为非阻塞模式
	u_long imode = 1;
	ret = ioctlsocket(g_cli_socket, FIONBIO, &imode);
	if (ret == -1)
	{
		LOGC(4) << "ioctlsocket failed!" << std::endl;
#ifdef WIN32
		closesocket(g_cli_socket);
		WSACleanup();
#endif
		return -1;
	}

	// 设置服务器地址
	g_srv_addr.sin_family = AF_INET;
	g_srv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	g_srv_addr.sin_port = htons(5000);

	// 创建KCP实例
	std::ostringstream oss;
	oss << std::this_thread::get_id();
	std::string stid = oss.str();
	unsigned long long tid = std::stoull(stid);
	ikcpcb* kcp = ikcp_create(tid, (void*)&g_srv_addr);
	kcp->output = udp_output;
	kcp->stream = 0;
	// 启动快速模式
	// 第二个参数 nodelay-启用以后若干常规加速将启动
	// 第三个参数 interval为内部处理时钟，默认设置为 10ms
	// 第四个参数 resend为快速重传指标，设置为2
	// 第五个参数 为是否禁用常规流控，这里禁止
	//ikcp_nodelay(kcp, 1, 10, 2, 1); // 设置成1次ACK跨越直接重传, 这样反应速度会更快. 内部时钟10毫秒.
	//普通模式
	ikcp_nodelay(kcp, 0, 40, 0, 0);
	ikcp_wndsize(kcp, 2, 2);
	ikcp_setmtu(kcp, MTU);

	LOGC(1) << "use conn : " << stid << std::endl;

	bool exit = false;
	std::thread th1([&]
		{
			int ret;
			char buf[BUFSIZE] = { 0 };
			while (!exit)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(SENDINTERVEL));

				// 发送到KCP"处理程序" 处理结果在kcp->output回调
				static uint64_t index = 0;
				sprintf(buf, "[%s]:hello, kcp! %lld", stid.c_str(), ++index);

				LOCK;
				ret = ikcp_send(kcp, buf, strlen(buf));
				if (ret < 0)
				{
					--index;
					LOGC(4) << "ikcp_send failed, ret : " << ret << std::endl;
					continue;
				}
				// flush
				ikcp_flush(kcp);

				ikcp_update(kcp, clock());
			}
		}
	);

	std::thread th2([&]
		{
			int ret;
			char buf[BUFSIZE] = { 0 };
			while (!exit)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(RECVINTERVEL));
				int len = sizeof(g_srv_addr);
				ret = recvfrom(g_cli_socket, buf, BUFSIZE, 0, (sockaddr*)&g_srv_addr, &len);
				if (ret < 0)
				{
#ifdef WIN32
					switch (GetLastError())
					{
					case WSAEWOULDBLOCK:
					case WSAEMSGSIZE:
					break;
					default:
						LOGC(4) << "recvfrom failed : " << GetLastError() << std::endl;
						//std::this_thread::sleep_for(std::chrono::seconds(5));
						break;
					}
#endif
					continue;
				}
#ifdef SHOWRAW
				LOGC(2) << "raw recv : " << std::string(buf, ret) << std::endl;
#endif

				LOCK;
				// 输入到KCP"处理程序"
				ret = ikcp_input(kcp, buf, ret);
				if (ret < 0)
				{
					LOGC(4) << "ikcp_input failed, ret : " << ret << std::endl;
					continue;
				}
				// flush
				ikcp_flush(kcp);

				ikcp_update(kcp, clock());

				// 从KCP"处理程序"获取用户层数据
				ret = ikcp_recv(kcp, buf, BUFSIZE);
				if (ret > 0)
				{
					LOGC(1) << "recv : " << std::string(buf, ret) << std::endl;
				}
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
	
	ikcp_release(kcp);
	kcp = nullptr;
	closesocket(g_cli_socket);

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}