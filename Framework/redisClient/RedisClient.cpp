#include "RedisClient.h"
#include "RedisPoolCluster.h"
#include "RedisConnection.h"
#include "BuildCommand.h"
#include "RedisConnectPool.h"
#include "StringParser.h"

RedisClient::RedisClient(RedisPoolCluster* pRedisPoolCluster)
: m_pRedisPoolCluster(pRedisPoolCluster)
{
    assert(pRedisPoolCluster != NULL);
}

RedisClient::~RedisClient()
{
}

RedisClient::string_type RedisClient::missing_value("**nonexistent-key**");
const string CRLF("\r\n");
const string status_reply_ok("OK");
const string prefix_status_reply_error("-ERR ");
const char prefix_status_reply_value = '+';
const char prefix_single_bulk_reply = '$';
const char prefix_multi_bulk_reply = '*';
const char prefix_int_reply = ':';

bool RedisClient::Set(const string_type& key, const string_type& value)
{
    BuildCommand cmd("SET");
    cmd << key << value;
    SharedPtr<RedisConnection> connObj = GetRedisConnection();

    if(connObj->SendCommand(cmd))
    {
        return Recv_OK_Reply(connObj.get());
    }    
    return false;
}

RedisClient::string_type RedisClient::Get(const string_type& key)
{
    BuildCommand cmd("GET");
    cmd << key;
    SharedPtr<RedisConnection> connObj = GetRedisConnection();

    if(connObj->SendCommand(cmd))
    {
        return Recv_Bulk_Reply(connObj.get());
    }    
    return string();
}

void RedisClient::MGet(const string_vector & keys, string_vector & out)
{

}

bool RedisClient::Exists(const string_type & key)
{
    return true;
}

void RedisClient::Del(const string_type & key)
{

}

bool RedisClient::Recv_OK_Reply(RedisConnection* pRedisConn)
{
    if (Recv_Single_Line_Reply(pRedisConn) != status_reply_ok) 
    {
        LOG_ERROR("Expected OK Response");
        return false;
    }
    return true;
}

void RedisClient::Recv_Int_OK_Reply(RedisConnection* pRedisConn)
{
    if (Recv_Int_Reply(pRedisConn) != 1)
        LOG_ERROR("Expecting Int Reply Of 1");
}

std::string RedisClient::Recv_Single_Line_Reply(RedisConnection* pRedisConn)
{
    std::string line;
    if (!pRedisConn->ReadLine(line))
        return string();

    if (line.find(prefix_status_reply_error) == 0) 
    {
        string error_msg = line.substr(prefix_status_reply_error.length());
        if(error_msg.empty()) 
            LOG_ERROR("unknown error");
    }

    if(line[0] != prefix_status_reply_value)
        LOG_ERROR("UnExpected Prefix For Status Reply");

    return line.substr(1);
}

RedisClient::int_type RedisClient::Recv_Bulk_Reply(char prefix,RedisConnection* pRedisConn)
{
    std::string line;
    if (!pRedisConn->ReadLine(line))
        return -1;
    if (line.empty()) 
        LOG_ERROR("Recv Empty Bulk Reply String Is  Reply");

    if (line[0] != prefix)
        LOG_ERROR("UnExpected Prefix For Bulk Reply");
    return value_from_string<RedisClient::int_type>(line.substr(1));
}

std::string RedisClient::Recv_Bulk_Reply(RedisConnection* pRedisConn)
{
    int_type length = Recv_Bulk_Reply(prefix_single_bulk_reply,pRedisConn);

    if (length == -1)
        return RedisClient::missing_value;

    int_type real_length = length + 2;    // CRLF

    string data;
    if(!pRedisConn->Read_N(data,real_length))
        LOG_ERROR("Read Size:%u Error",real_length);
    if(data.empty())
        LOG_ERROR("Invalid Bulk Reply Data Empty");

    if(data.length() != static_cast<string::size_type>(real_length))
        LOG_ERROR("Invalid Bulk Reply Data; Data Of UnExpected Length");

    data.erase(data.size() - 2);
    return data;
}

RedisClient::int_type RedisClient::Recv_Multi_Bulk_Reply(string_vector& out,RedisConnection* pRedisConn)
{
    int_type length = Recv_Bulk_Reply(prefix_multi_bulk_reply,pRedisConn);

    if (length == -1)
        LOG_ERROR("No Such Key");

    for(int_type i = 0; i < length; ++i)
        out.push_back(Recv_Bulk_Reply(pRedisConn));

    return length;
}

RedisClient::int_type RedisClient::Recv_Multi_Bulk_Reply(string_set& out,RedisConnection* pRedisConn)
{
    int_type length = Recv_Bulk_Reply(prefix_multi_bulk_reply,pRedisConn);

    if (length == -1)
        LOG_ERROR("No Such Key");

    for(int_type i = 0; i < length; ++i) 
        out.insert(Recv_Bulk_Reply(pRedisConn));
    return length;
}

RedisClient::int_type RedisClient::Recv_Int_Reply(RedisConnection* pRedisConn)
{
    std::string line;	
    if (!pRedisConn->ReadLine(line))
        return -1;
    if (line.empty())
        LOG_ERROR("Invalid Integer Reply Empty");

    if (line[0] != prefix_int_reply)
        LOG_ERROR("UnExpected Prefix For Integer Reply");

    return value_from_string<RedisClient::int_type>(line.substr(1));
}

SharedPtr<RedisConnection> RedisClient::GetRedisConnection()
{
    SharedPtr<RedisConnectPool> poolObj = m_pRedisPoolCluster->Peek().cast<RedisConnectPool>();
    if(poolObj.isNull())
    {
        LOG_ERROR("There Are No Redis Pool");
        return SharedPtr<RedisConnection>();
    }

    SharedPtr<RedisConnection> connObj = poolObj->Peek().cast<RedisConnection>();
    return connObj;
}

