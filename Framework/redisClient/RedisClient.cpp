#include "RedisClient.h"
#include "RedisPoolCluster.h"
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

const char RedisClient::PREFIX_REPLY_STATUS = '+';
const char RedisClient::PREFIX_REPLY_ERR = '-';
const char RedisClient::PREFIX_REPLY_INT = ':';
const char RedisClient::PREFIX_BULK_REPLY = '$';
const char RedisClient::PREFIX_MULTI_BULK_REPLY = '*';

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

//==============================method offen used====================================

bool RedisClient::_getReply( CResult &result,RedisConnection* pRedisConn)
{
    result.clear();	
	std::string line;
	if (!pRedisConn->ReadLine(line))
		return false;
    LOG_DEBUG("row data:%s",line.c_str());
    switch ( line[0] )
    {
    case PREFIX_REPLY_INT:
        result.setType( REDIS_REPLY_INTEGERER );
        result = line.substr( 1 );
        break;
    case PREFIX_REPLY_STATUS:
        result.setType( REDIS_REPLY_STATUS );
        result = line.substr( 1 );
        break;
    case PREFIX_REPLY_ERR:
        result.setType( REDIS_REPLY_ERROR );
        result = line.substr( 1 );
        break;
    case PREFIX_BULK_REPLY:
        result.setType( REDIS_REPLY_STRING );
        _replyBulk( result,line,pRedisConn );
        break;
    case PREFIX_MULTI_BULK_REPLY:
        result.setType( REDIS_REPLY_ARRAY );
        _replyMultiBulk( result,line,pRedisConn );
        break;
    default:
        LOG_ERROR("unknow type");
        break;
    }
    return true;
}

bool RedisClient::_getReply( CResult &result,BuildCommand& cmd)
{
    SharedPtr<RedisConnection> connObj = GetRedisConnection();
    if(connObj->SendCommand(cmd))
	{
        _getReply(result,connObj.get());
		return true;
	}
    else{
        LOG_ERROR("Send Command Error,cmd:%s",cmd.getCommand().c_str());
	}
	return false;
}

bool RedisClient::_replyBulk(CResult& result, const std::string &len,RedisConnection* pRedisConn)
{
    // get the number of CResult received .
    int64_t protoLen = _valueFromString<int64_t>(len.substr(1));

    if (protoLen == -1)
    {
        result = "";
        result.setType(REDIS_REPLY_NIL);
        return false;
    }

	int64_t real_length = protoLen + 2;    // CRLF
    if(!pRedisConn->Read_N(result,real_length))
	    LOG_ERROR("Read Size:%u Error",real_length);
    if(result.empty())
        LOG_ERROR("Invalid Bulk Reply Data Empty");

    if(result.length() != static_cast<string::size_type>(real_length))
        LOG_ERROR("Invalid Bulk Reply Data; Data Of UnExpected Length");

    result.erase(result.size() - 2);
    result.setType( REDIS_REPLY_STRING );
    return true;
}

uint64_t RedisClient::_replyMultiBulk(CResult& result, const std::string &line,RedisConnection* pRedisConn)
{
    // get the number of CResult received .
   int64_t replyNum = _valueFromString<int64_t>( line.substr(1) );
   CResult ele;
   for ( int i = 0; i< replyNum; i++ )
   {
       _getReply(ele,pRedisConn);
       result.addElement( ele );
   }
   
   return result.getArry().size();
}

void RedisClient::_getStringVecFromArry(const CResult::ListCResult &arry, RedisClient::VecString &values )
{
    CResult::ListCResult::const_iterator it = arry.begin();
    CResult::ListCResult::const_iterator end = arry.end();

    for ( ; it != end; ++it )
    {
        values.push_back( static_cast<string>(*it) );
    }
}

void RedisClient::_getStringMapFromArry(const CResult::ListCResult &arry, RedisClient::MapString &pairs)
{
    CResult::ListCResult::const_iterator it = arry.begin();
    CResult::ListCResult::const_iterator it2 = it;
    CResult::ListCResult::const_iterator end = arry.end();

    for ( ; it != end; ++it )
    {
        it2 = it++;		// the next element is value.
        pairs.insert( MapString::value_type( *it2, *it ) );
    }
}

bool RedisClient::_getStatus( BuildCommand& cmd , string& status)
{
    CResult result;	
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
	if(!pRedisConn->SendCommand(cmd))
	    return false;
    _getReply( result,pRedisConn);

    ReplyType type = result.getType();
    if ( REDIS_REPLY_NIL ==  type )
    {
        return false;
    }
    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_STATUS != type )
    {
       LOG_ERROR("%s: data recved is not status",cmd.getCommand().c_str());
    }
    status = result.getStatus();
    return true;
}



bool RedisClient::_getInt(  BuildCommand& cmd , int64_t& number)
{
    CResult result;
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
    if(!pRedisConn->SendCommand(cmd))
	    return false;
    _getReply( result,pRedisConn );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_NIL ==  type )
    {
        return false;
    }
    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_INTEGERER != type )
    {
       LOG_ERROR("%s: data recved is not iintergerer",cmd.getCommand().c_str());
    }
    number  = result.getInt();
    return true;
}

bool RedisClient::_getString(  BuildCommand& cmd , string& value)
{
    CResult result;
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
    if(!pRedisConn->SendCommand(cmd))
	    return false;
    _getReply( result,pRedisConn );

    ReplyType type = result.getType();
    if ( REDIS_REPLY_NIL ==  type )
    {
        return false;
    }
    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_STRING != type )
    {
       LOG_ERROR("%s: data recved is not string",cmd.getCommand().c_str());
    }
    value = result.getString();
    return true;
}

bool RedisClient::_getArry(BuildCommand &cmd, CResult &result)
{
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
    if(!pRedisConn->SendCommand(cmd))
	    return false;
    _getReply( result,pRedisConn );

    ReplyType type = result.getType();

    if ( REDIS_REPLY_NIL == type )
    {
        return false;
    }
    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_ARRAY != type )
    {
       LOG_ERROR("%s: data recved is not array",cmd.getCommand().c_str());
    }

    return true;
}


uint64_t RedisClient::_getArry(BuildCommand &cmd, VecString &values)
{
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
    if(!pRedisConn->SendCommand(cmd))
	    return false;
    CResult result;
    _getReply( result,pRedisConn );

    ReplyType type = result.getType();

    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_ARRAY != type )
    {
       LOG_ERROR("%s: data recved is not array",cmd.getCommand().c_str());
    }

    _getStringVecFromArry( result.getArry(), values );
    return values.size();
}

uint64_t RedisClient::_getArry(BuildCommand &cmd, RedisClient::MapString &pairs)
{
	SharedPtr<RedisConnection> pRedisConn = GetRedisConnection();
    if(!pRedisConn->SendCommand(cmd))
	    return false;
    CResult result;
    _getReply( result,pRedisConn );

    ReplyType type = result.getType();

    if ( REDIS_REPLY_ERROR == type )
    {
        LOG_ERROR("Error Result:%s",result.getErrorString().c_str());
    }
    if ( REDIS_REPLY_ARRAY != type )
    {
       LOG_ERROR("%s: data recved is not array",cmd.getCommand().c_str());
    }

    _getStringMapFromArry( result.getArry(), pairs );
    return pairs.size();
}
