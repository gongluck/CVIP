#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "ikcp.h"

#define LOG std::cout << __FILE__ << "("<< __LINE__ << ")"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
#endif

SOCKET      g_cli_socket;
SOCKADDR_IN g_srv_addr;
SOCKADDR_IN g_cli_addr;
const int	BUFSIZE = 1024;
char        g_buf[BUFSIZE] = { 0 };

int udp_output(const char* buf, int len, ikcpcb* kcp, void* user) 
{
	int ret = sendto(g_cli_socket, buf, len, 0, (sockaddr*)user, sizeof(g_srv_addr));
	if (ret <= 0)
	{
		LOG << "send failed, ret : " << ret << std::endl;
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
		LOG << "WSAStartup failed!" << std::endl;
		return -1;
	}
#endif

	// 创建套接字
	g_cli_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_cli_socket == -1)
	{
#ifdef WIN32
		LOG << "create socket failed, error : " << WSAGetLastError() << std::endl;
		WSACleanup();
#endif
		return -1;
	}

	//设置为非阻塞模式
	u_long imode = 1;
	ret = ioctlsocket(g_cli_socket, FIONBIO, &imode);
	if (ret == -1)
	{
		LOG << "ioctlsocket failed!" << std::endl;
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

	LOG << "use conn : " << stid << std::endl;

	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		ikcp_update(kcp, clock());

		// 发送到KCP"处理程序" 处理结果在kcp->output回调
		static uint64_t index = 0;
		sprintf(g_buf, "hello, kcp! %lld", ++index);
		ikcp_send(kcp, g_buf, strlen(g_buf));
		// flush
		ikcp_flush(kcp);

		while (true) 
		{
			int len = sizeof(g_srv_addr);
			ret = recvfrom(g_cli_socket, g_buf, BUFSIZE, 0, (sockaddr*)&g_srv_addr, &len);
			if (ret < 0)
			{
				break;
			}

			// 输入到KCP"处理程序"
			ikcp_input(kcp, g_buf, ret);
			// flush
			ikcp_flush(kcp);
		}
		while (true) 
		{
			// 从KCP"处理程序"获取用户层数据
			ret = ikcp_recv(kcp, g_buf, BUFSIZE);
			if (ret < 0)
			{
				break;
			}
			LOG << "recv : " << std::string(g_buf, ret) << std::endl;
		}
	}

	closesocket(g_cli_socket);

#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}