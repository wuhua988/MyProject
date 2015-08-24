#include "RedisConnection.h"
#include "SocketUtils.h"
#include "StringParser.h"

using std::vector;

const string whitespace(" \f\n\r\t\v");
const string CRLF("\r\n");
RedisConnection::RedisConnection(const string& addr, int conn_timeout,int rw_timeout, bool retry)
:WatcherConnection(NULL,NULL)
,m_socket(Socket::kBlockingSocketType)
,m_strAddr(addr)
,m_conn_timeout(conn_timeout)
,m_rw_timeout(rw_timeout)
,m_bRetry(retry)
{

}

RedisConnection::~RedisConnection()
{
    m_socket.Close();
}

int RedisConnection::Open(void * arg)
{
    if(m_socket.GetSocketFD() != Socket::kInvalidFileDesc)
        return 0;
    if(m_socket.GetSocketFD() == Socket::kInvalidFileDesc && OS_NoErr != m_socket.Open())
    {
        LOG_ERROR("Create Socket Failed!");
        return 1;
    }
    std::string ip;
    int port;

    if(get_ip_port_from_str(ip,port,m_strAddr))
    {
        UInt32 theIPAddr = SocketUtils::ConvertStringToAddr(ip.c_str());
        if(m_socket.Connect(theIPAddr, port) != OS_NoErr)
        {
            return 1;
        }
    }else{
        LOG_ERROR("Connect To:%s,Error!",m_strAddr.c_str());
        return 1;
    }

    return 0;
}

int RedisConnection::HandleEvent(int, SOCKET)
{
    return 0;
}
bool RedisConnection::SendData(MsgBlock* pMsgBlock)
{
    return true;
}
int RedisConnection::Close(SOCKET handle)
{
    m_socket.Close();
    return 0;
}

TCPSocket* RedisConnection::GetSocket()
{
    return &m_socket;
}

SOCKET RedisConnection::GetHandle()
{
    return m_socket.GetSocketFD();
}

// Reads a single line of character data from the given blocking socket.
// Returns the line that was read, not including EOL delimiter(s).  Both LF
// ('\n') and CRLF ("\r\n") delimiters are supported.  If there was an I/O
// error reading from the socket, connection_error is raised.  If max_size
// bytes are read before finding an EOL delimiter, a blank string is
// returned.
bool RedisConnection::ReadLine(string& strResult, ssize_t max_size) 
{
    assert(max_size > 0);    
    std::ostringstream oss;    
    enum { buffer_size = 64 };
    char buffer[buffer_size];
    memset(buffer, 0, buffer_size);

    ssize_t total_bytes_read = 0;
    bool found_delimiter = false;

    while (total_bytes_read < max_size && !found_delimiter)
    {
        // Peek at what's available    
        ssize_t bytes_received = 0;
        do bytes_received = recv(m_socket.GetSocketFD(), buffer, buffer_size, MSG_PEEK);
        while (bytes_received < 0 && (OSThread::GetErrno() == EINTR));

        if(bytes_received <= 0)
        {
            LOG_ERROR("%s",bytes_received == 0 ?"Connection Was Closed" : "Connection Erron When Read Line");
            return false;
        }

        // Some data is available; Length might be < buffer_size.
        // Look for newline in whatever was read though.

        char * eol = static_cast<char *>(memchr(buffer, '\n', bytes_received));

        // If found, write data from the buffer to the output string.
        // Else, write the entire buffer and continue reading more data.

        ssize_t to_read = bytes_received;
        total_bytes_read += bytes_received;
        if (eol) 
        {
            to_read = eol - buffer + 1;
            oss.write(buffer, to_read);
            found_delimiter = true;
        }
        else
            oss.write(buffer, bytes_received);

        // Now read from the socket to remove the peeked data from the socket's
        // read buffer.  This will not block since we've peeked already and know
        // there's data waiting.  It might fail if we were interrupted however.

        do bytes_received = recv(m_socket.GetSocketFD(), buffer, to_read, 0);
        while (bytes_received < 0 && (OSThread::GetErrno() == EINTR));
    }

    // Construct final line string. Remove trailing CRLF-based whitespace.

    strResult = oss.str();
    rtrim(strResult, CRLF);
    return true;
}

// Reads N bytes from given blocking socket.

bool RedisConnection::Read_N(string& strResult, ssize_t n)
{
    char * buffer = new char[n + 1];
    assert(buffer != NULL);
    buffer[n] = '\0';

    char * bp = buffer;
    ssize_t bytes_read = 0;

    while (bytes_read != n) 
    {
        ssize_t bytes_received = 0;
        do bytes_received = recv(m_socket.GetSocketFD(), bp, n - (bp - buffer), 0);
        while (bytes_received < 0 && (OSThread::GetErrno() == EINTR));

        if (bytes_received <= 0)
        {
            LOG_ERROR("%s,Size:%u", bytes_received == 0 ? "Redis Connection Was Closed" : "Redis Connection Error When Read_N",n);
            return false;
        }

        bytes_read += bytes_received;
        bp += bytes_received;
    }

    strResult.assign(buffer);
    delete [] buffer;
    return true;
}

bool RedisConnection::SendCommand(const string& strCmd)
{
    UInt32 outLengthSent;
    bool bRetry = true;

    while(true)
    {
        if(Open())
            return false;
        if(!(m_socket.Send(strCmd.c_str(),strCmd.size(),&outLengthSent) == OS_NoErr && outLengthSent == strCmd.size()))
        {
            if(bRetry)
            {
                Close();
                bRetry = false;
                LOG_ERROR("Send Command Error And Retry To Connect Server,Cmd:%s",strCmd.c_str());
                continue;
            }
            LOG_ERROR("Send Command Error,Cmd:%s",strCmd.c_str());
            return false;
        }
        break;
    }
    return true;
}





