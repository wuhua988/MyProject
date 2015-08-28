#include "BuildCommand.h"
#include "RedisClient.h"

void RedisClient::lpush(const std::string &key, const VecString& value, CResult& result )
{
    BuildCommand cmd("LPUSH");
    cmd << key;

    VecString::const_iterator it = value.begin();
    VecString::const_iterator end = value.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
	
    SharedPtr<RedisConnection> connObj = GetRedisConnection();
    if(connObj->SendCommand(cmd))
        _getReply(result,connObj.get());
    else{
        LOG_ERROR("Send LPUSH Command Error,cmd:%s",cmd.getCommand().c_str());
	}
}


SInt64 RedisClient::lpush(const std::string &key, const VecString &value)
{
    CResult result;

    lpush( key, value, result );

    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s", result.getErrorString().c_str() );
    }

    if ( type != REDIS_REPLY_INTEGERER )
    {
        LOG_ERROR( "LPUSH: data recved is not integerer");
    }

    return result.getInt();
}



void RedisClient::lpop( const std::string& key, CResult& result )
{
    BuildCommand cmd( "LPOP" );
    cmd << key;

	SharedPtr<RedisConnection> connObj = GetRedisConnection();
    if(connObj->SendCommand(cmd))
        _getReply(result,connObj.get());
    else{
        LOG_ERROR("Send lpop Command Error,cmd:%s",cmd.getCommand().c_str());
	}
}

bool RedisClient::lpop(const std::string &key, std::string &value)
{
    CResult result;
    lpop( key, result );

    ReplyType type = result.getType();
    if ( type == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s", result.getErrorString().c_str());
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STRING )
    {
        value = result.getString();
        return true;
    }else
    {
        LOG_ERROR( "LPOP: data recved is not string" );
    }
	return false;
}



