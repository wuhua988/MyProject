#include <assert.h>
#include <stdlib.h>
#include "ttsocket.h"
#include "ttutil.h"

int InitSocket(void)
{
#if defined(TT_IS_WINDOWS)
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	return WSAStartup(wVersionRequested, &wsaData);
#else
	return 0;
#endif
}

int ClearScoket(void)
{
#if defined(TT_IS_WINDOWS)
	return WSACleanup();
#else
	return 0;
#endif
}

int GetSocketError(void)
{
#if defined(TT_IS_WINDOWS)
	return WSAGetLastError();
#else
	return errno;
#endif
}

SOCKET TCPSocket(void)
{
#if defined(TT_IS_WINDOWS)
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	return socket(PF_INET, SOCK_STREAM, 0);
#endif
}

SOCKET UDPSocket(void)
{
#if defined(TT_IS_WINDOWS)
	return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
	return socket(PF_INET, SOCK_DGRAM, 0);
#endif
}

int ConnectSocket(SOCKET sock, char* ip, int port)
{
    if (NULL == ip)
        return -1;

    struct sockaddr_in sinAddr;
    memset(&sinAddr, 0, sizeof(sinAddr));
    sinAddr.sin_family = AF_INET;
    sinAddr.sin_addr.s_addr = inet_addr(ip);
    sinAddr.sin_port = htons((WORD)port);
    return connect(sock, (struct sockaddr*)&sinAddr, sizeof(sinAddr));
}

int CloseSocket(SOCKET sock)
{
#if defined(TT_IS_WINDOWS)
	return closesocket(sock);
#else
	return close(sock);
#endif
}

int SendSocketVec(IN SOCKET sock, IN const SocketBufferVec* vec, IN int n, IN int flags)
{
#if defined(TT_IS_WINDOWS)

    WSABUF* win_wsabuf = (WSABUF*)malloc(sizeof(WSABUF)*n);
    if (NULL == win_wsabuf)
        return -1;
    for (int i=0; i<n; i++)
    {
        win_wsabuf[i].buf = (char*)vec->buf;
        win_wsabuf[i].len = vec->len;
    }

	DWORD send_bytes;
	int r = WSASend(sock, win_wsabuf, n, &send_bytes, flags, NULL, NULL);
    free(win_wsabuf);
	if(0 == r)
		return (int)send_bytes;
	return r;
#else
    struct iovec* lin_iovec = (struct iovec*)malloc(sizeof(iovec)*n);
    if (NULL == lin_iovec)
        return -1;

    for (int i=0; i<n; i++)
    {
        lin_iovec[i].iov_base = vec->buf;
        lin_iovec[i].iov_len = vec->len;
    }

	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = (struct iovec*)lin_iovec;
	msg.msg_iovlen = n;
	int r = sendmsg(sock, &msg, flags);
    free(lin_iovec);
	return r;
#endif
}

int RecvSocketVec(IN SOCKET sock, IN SocketBufferVec* vec, IN int n, IN int flags)
{
#if defined(TT_IS_WINDOWS)
	DWORD recv_bytes;
    WSABUF* win_wsabuf = (WSABUF*)malloc(sizeof(WSABUF)*n);
    if (NULL == win_wsabuf)
        return -1;
	int r = WSARecv(sock, win_wsabuf, n, &recv_bytes, (LPDWORD)&flags, NULL, NULL);
	if(0 == r)
    {
        free(win_wsabuf);
        return (int)recv_bytes;
    }

    for (int i=0; i<n; i++)
    {
        vec->buf = win_wsabuf[i].buf;
        vec->len = win_wsabuf[i].len;
    }
    free(win_wsabuf);
    return r;
#else
    struct iovec* lin_iovec = (struct iovec*)malloc(sizeof(iovec)*n);
    if (NULL == lin_iovec)
        return -1;

    struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = lin_iovec;
	msg.msg_iovlen = n;
	int r = recvmsg(sock, &msg, flags);
    if (r<=0)
    {
        free(lin_iovec);
        return r;
    }

    for (int i=0; i<n; i++)
    {
        vec->buf = lin_iovec[i].iov_base;
        vec->len = lin_iovec[i].iov_len;
    }
    free(lin_iovec);
    return r;
#endif
}

int WaitSocket(SOCKET sock, int events, int timeout_ms)
{
	int n;
	int timeout = (INFINITE==timeout_ms ? -1 : (int)(timeout_ms&0x7FFFFFFF));
#if defined(TT_IS_LINUX)
	struct pollfd pfds[1];
	int poll_count = 1;

	pfds[0].fd = sock;
	pfds[0].events = 0;
	if(events & 0x1)
		pfds[0].events |= POLLIN;
	if(events & 0x2)
		pfds[0].events |= POLLPRI;
	if(events & 0x4)
		pfds[0].events |= POLLOUT;
	pfds[0].revents = 0;

	n = poll(pfds, poll_count, timeout);
	while(n<0 && errno==EINTR)
	{
		if(timeout == 0)
			break;
		else if(0<timeout && timeout<=10)
		{
			Sleep(timeout);
			timeout = 0;
		}
		else if(timeout > 10)
		{
			Sleep(10);
			timeout -= 10;
		}
		else
		{
			assert(-1 == timeout);
		}
		n = poll(pfds, poll_count, timeout);
	}
	if(n <= 0)
		return n;

	short revents = (pfds[0].events & pfds[0].revents);
	if(0 != revents)
	{
		n = 0;
		if(revents&POLLIN)
			n |= 1;
		if(revents&POLLPRI)
			n |= 2;
		if(revents&POLLOUT)
			n |= 4;
		return n;
	}
	return 0;

#else
	struct fd_set rfds;
	struct fd_set wfds;
	struct fd_set exfds;
	struct timeval tv, *ptv;
	FD_ZERO(&rfds);
	if(events & 0x1)
		FD_SET(sock, &rfds);
	FD_ZERO(&exfds);
	if(events & 0x2)
		FD_SET(sock, &exfds);
	FD_ZERO(&wfds);
	if(events & 0x4)
		FD_SET(sock, &wfds);
	if(timeout < 0)
		ptv = NULL;
	else
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		ptv = &tv;
	}
	n = select((int)sock+1, &rfds, &wfds, &exfds, ptv);
	if(n <= 0)
		return n;
	n = 0;
	if(FD_ISSET(sock, &rfds))
		n |= 1;
	if(FD_ISSET(sock, &exfds))
		n |= 2;
	if(FD_ISSET(sock, &wfds))
		n |= 4;
	return n;

#endif
}

int CanReadSocketByTime(IN SOCKET sock, IN int timeout_ms)
{
	return WaitSocket(sock, 0x1, timeout_ms);
}

int CanWriteSocketByTime(IN SOCKET sock, IN int timeout_ms)
{	
	return WaitSocket(sock, 0x4, timeout_ms);
}

int ReadSocketByTime(IN SOCKET sock, char* buf, IN int len, IN int timeout)
{
	int r = CanReadSocketByTime(sock, timeout);
	if (r<=0)
		return r;
	return recv(sock, (char*)buf, (int)len, 0);
}

int WriteSocketByTime(IN SOCKET sock, char* data, IN int len, IN int timeout)
{
	int r = CanReadSocketByTime(sock, timeout);
	if (r<=0)
		return r;
	return send(sock, (char*)data, (int)len, 0);
}

int GetSocketPeer(SOCKET sock, int* port /*= NULL*/)
{
	struct sockaddr_in sn;
	socklen_t socklen = sizeof(sn);
	if(0 != getpeername(sock, (struct sockaddr*)&sn, &socklen))
	{
		if(port)
			*port = -1;
		return INADDR_NONE;
	}
	if(port)
		*port = ntohs(sn.sin_port);
	return sn.sin_addr.s_addr;
}

int GetSocketLocal(SOCKET sock, int* port /*= NULL*/)
{
	struct sockaddr_in sn;
	socklen_t socklen = sizeof(sn);
	if(0 != getsockname(sock, (struct sockaddr*)&sn, &socklen))
	{
		if(port)
			*port = -1;
		return INADDR_NONE;
	}
	if(port)
		*port = ntohs(sn.sin_port);
	return sn.sin_addr.s_addr;
}

int SetSocketKeepAlive(SOCKET sock, BOOL onoff)
{
	return setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (CHARPTR)&onoff, sizeof(onoff));
}

int SetSocketReceiveBufSize(SOCKET sock, int len)
{
	return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (CHARPTR)&len, sizeof(len));
}

int GetSocketReceiveBufSize(SOCKET sock, int* len)
{
	socklen_t l = sizeof(*len); 
	return getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (CHARPTR)len, &l);
}

int SetSocketSendBufSize(SOCKET sock, int len)
{
	return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (CHARPTR)&len, sizeof(len));	
}

int GetSocketSendBufSize(SOCKET sock, int* len)
{
	socklen_t l = sizeof(*len); 
	return getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (CHARPTR)len, &l);
}

int SetSocketReceiveTimeout(SOCKET sock, int timeout_ms)
{
	struct timeval timev;
	if(INFINITE == timeout_ms)
	{
		timev.tv_sec = 0x7FFFFFFF;
		timev.tv_usec = 0;
	}
	else
	{
		timev.tv_sec = (timeout_ms&0x7FFFFFFF) / 1000;
		timev.tv_usec = (timeout_ms&0x7FFFFFFF) % 1000 * 1000;
	}
	return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (CHARPTR)&timev, sizeof(timev));
}

int SetSocketSendTimeout(SOCKET sock, int timeout_ms)
{
	struct timeval timev;
	if(INFINITE == timeout_ms)
	{
		timev.tv_sec = 0x7FFFFFFF;
		timev.tv_usec = 0;
	}
	else
	{
		timev.tv_sec = (timeout_ms&0x7FFFFFFF) / 1000;
		timev.tv_usec = (timeout_ms&0x7FFFFFFF) % 1000 * 1000;
	}
	return setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (CHARPTR)&timev, sizeof(timev));
}

int SetSocketNonBlock(SOCKET sock, BOOL onoff)
{
#if defined TT_IS_WINDOWS
	DWORD val = (onoff ? 1 : 0); 
	return ioctlsocket(sock, FIONBIO, &val);	

#else
// 	int flags = fcntl(sock, F_GETFL, 0);
// 	return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	int val = (onoff ? 1 : 0); 
	return ioctl(sock, FIONBIO, &val);
#endif
}

int SetSocketReuseAddr(SOCKET sock, BOOL onoff)
{
	int opt = (onoff ? 1 : 0);
	return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (CHARPTR)&opt, sizeof(opt));	
}

int SetSocketLinger(SOCKET sock, BOOL onoff, int timeout_ms)
{
	struct linger l;
	l.l_onoff = (short)(onoff ? 1 : 0);
	l.l_linger = (short)(INFINITE==timeout_ms ? 0xffff : ((timeout_ms&0x7FFFFFFF) / 1000));
	if(0==l.l_linger && 0!=timeout_ms)
		l.l_linger = 1;
	return setsockopt(sock, SOL_SOCKET, SO_LINGER, (CHARPTR)&l, sizeof(l));
}

int SetSocketTcpNoDelay(SOCKET sock, BOOL isDelay)
{
	int val = (isDelay ? 1 : 0);
	return setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (CHARPTR)&val, sizeof(val));
}
