#ifndef _ttsocket_h_
#define _ttsocket_h_

#if defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
	#include <WS2tcpip.h>
	#include <ws2ipdef.h>

	typedef SOCKET	SOCKET;
//	typedef WSABUF	SocketBufferVec;

	typedef char* CHARPTR;

	#if defined(_MSC_VER)
		#pragma comment(lib, "ws2_32.lib")
		#pragma warning(disable: 4127)
	#endif

#else
	#include <sys/types.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#include <unistd.h>
	#include <string.h>
	#include <fcntl.h>
	#include <poll.h>

	#define INFINITE       -1  // Infinite timeout
	#define INVALID_SOCKET ~0

	typedef int SOCKET;
//	typedef int socklen_t;
//	typedef struct iovec SocketBufferVec;
	typedef char* CHARPTR;
#endif

#include "tttypes.h"

typedef struct
{
    void* buf;
    INT_PTR len;
} SocketBufferVec;

/*
	return: 0 - success; other - error
*/
int InitSocket(void);
/*
	return: 0 - success; other - error
*/
int ClearScoket(void);
int GetSocketError(void);
SOCKET TCPSocket(void);
SOCKET UDPSocket(void);

int ConnectSocket(SOCKET sock, char* ip, int port);

/*
	return: 0 - success; other - error
*/
int CloseSocket(SOCKET sock);

/*
	return: 0< - send bytes; other - error
*/
int SendSocketVec(IN SOCKET sock, IN const SocketBufferVec* vec, IN int n, IN int flags);

/*
	return: 0< - recv bytes; other - error
*/
int RecvSocketVec(IN SOCKET sock, IN SocketBufferVec* vec, IN int n, IN int flags);

/*
	events: 0x01 - read; 0x02 - urgent; 0x04 - write; or 0x01|0x02|0x04;
	return: <0 - error; 0 - timeout; 0x01 - read; 0x02 - urgent; 0x04 - write;  or 0x01|0x02|0x04; 
 */
int WaitSocket(SOCKET sock, int events, int timeout_ms);

/*
	return: <0 - error; 0 - timeout; 0x01 - read;
 */
int CanReadSocketByTime(IN SOCKET sock, IN int timeout_ms);

/*
	return: <0 - error; 0 - timeout; 0x04 - write;
 */
int CanWriteSocketByTime(IN SOCKET sock, IN int timeout_ms);


/*
	return: <0 - error; 0 - timeout; other - read bytes;
 */
int ReadSocketByTime(IN SOCKET sock, char* buf, IN int len, IN int timeout);

/*
	return: <0 - error; 0 - timeout; other - write bytes;
 */
int WriteSocketByTime(IN SOCKET sock, char* data, IN int len, IN int timeout);

/*
	return: int ip - success; INADDR_NONE - error;
 */
int GetSocketPeer(IN SOCKET sock, OUT int* port /*= NULL*/);
int GetSocketLocal(IN SOCKET sock, OUT int* port /*= NULL*/);

/*
	return 0 - success; other - error
*/
int SetSocketKeepAlive(IN SOCKET sock, IN BOOL onoff);
int SetSocketReceiveBufSize(IN SOCKET sock, IN int len);
int GetSocketReceiveBufSize(IN SOCKET sock, OUT int* len);
int SetSocketSendBufSize(IN SOCKET sock, IN int len);
int GetSocketSendBufSize(IN SOCKET sock, OUT int* len);

int SetSocketReceiveTimeout(IN SOCKET sock, IN int timeout_ms);
int SetSocketSendTimeout(IN SOCKET sock, IN int timeout_ms);

int SetSocketReuseAddr(IN SOCKET sock, IN BOOL onoff);
int SetSocketNonBlock(IN SOCKET sock, IN BOOL onoff);
int SetSocketLinger(IN SOCKET sock, IN BOOL onoff, IN int timeout_ms);
int SetSocketTcpNoDelay(IN SOCKET sock, IN BOOL isDelay);

#endif




