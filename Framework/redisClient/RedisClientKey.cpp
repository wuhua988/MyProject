#include "BuildCommand.h"
#include "RedisClient.h"

SInt64 RedisClient::keys( const std::string &pattern , VecString &values )
{
	BuildCommand cmd("KEYS");
	cmd << pattern;

	_getArry(cmd, values);
	return values.size();
}

SInt64 RedisClient::del( RedisClient::VecString &keys )
{

	BuildCommand cmd("DEL");

	VecString::const_iterator it = keys.begin();
	VecString::const_iterator end = keys.end();
	for ( ; it != end ; ++it )
	{
		cmd << *it;
	}

	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::exists( const string& key )
{
	BuildCommand cmd("EXISTS");
	cmd << key;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;

}

bool RedisClient::expireAt( const string& key , const UInt64& timestamp )
{
	BuildCommand cmd("EXPIREAT");
	cmd << key << timestamp;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::pExpireAt( const string& key , const UInt64& timestamp )
{
	BuildCommand cmd("PEXPIREAT");
	cmd << key << timestamp;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::expire( const string& key , const UInt64& seconds )
{
	BuildCommand cmd("EXPIRE");
	cmd << key << seconds;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::pExpire( const string& key , const UInt64& msec )
{
	BuildCommand cmd("PEXPIRE");
	cmd << key << msec;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

SInt64 RedisClient::ttl( const string& key )
{
	BuildCommand cmd("TTL");
	cmd << key;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

SInt64 RedisClient::pttl( const string& key )
{
	BuildCommand cmd("PTTL");
	cmd << key;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::persist( const string& key )
{
	BuildCommand cmd("PERSIST");
	cmd << key;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

bool RedisClient::move( const string& key , const int& dstDBIndex )
{
	BuildCommand cmd("MOVE");
	cmd << key << dstDBIndex;
	SInt64 num = 0;
	_getInt(cmd, num);
	return num;
}

string RedisClient::object( const EobjSubCommand& subcommand , const string& key )
{
	BuildCommand cmd("OBJECT");
	SInt64 num = 0;
	string retStr;
	std::stringstream ss;
	switch ( subcommand )
	{
	case REFCOUNT :
		cmd << "REFCOUNT" << key;
		if ( _getInt(cmd, num) )
		{
			ss << num;
			return ss.str();
		}
		break;

	case IDLETIME :
		cmd << "IDLETIME" << key;
		if ( _getInt(cmd, num) )
		{
			ss << num;
			return ss.str();
		}
		break;

	case ENCODING :
		cmd << "ENCODING" << key;

		if ( _getString(cmd, retStr) )
		{
			return retStr;
		}
		break;

	}

	return REDIS_NIL;
}

string RedisClient::randomKey( )
{
	BuildCommand cmd("RANDOMKEY");
	string retKey;

	if ( _getString(cmd, retKey) )
		return retKey;
	else
		return REDIS_NIL;
}

bool RedisClient::rename( const string& key , const string& newKey )
{
	BuildCommand cmd("RENAME");
	string status;
	cmd << key << newKey;

	return _getStatus(cmd, status);
}

bool RedisClient::renameNx( const string& key , const string& newKey )
{
	BuildCommand cmd("RENAMENX");
	SInt64 num = 0;
	cmd << key << newKey;

	if ( _getInt(cmd, num) )
	{
		return num;
	}
	return false;
}

bool RedisClient::sort( const string& key , VecString& values , const bool& desc )
{
	BuildCommand cmd("SORT");
	cmd << key;

	if ( desc )
		cmd << "DESC";

	if ( _getArry(cmd, values) )
	{
		return values.size();
	}
	return false;
}

bool RedisClient::type( const string& key , string& type )
{
	BuildCommand cmd("TYPE");
	cmd << key;

	return _getStatus(cmd, type);
}

int RedisClient::scan( VecString& values , const int& index , const string& pattern ,
		const int& count )
{
	BuildCommand cmd("SCAN");
	string val;
	SInt64 nextNo;
	CResult arry;

	cmd << index;

	if ( !pattern.empty() )
	{
		LOG_DEBUG("PATTERN:%s", pattern.c_str());
		cmd << "MATCH" << pattern;
	}

	if ( count > 0 && count != 10 )
	{
		LOG_DEBUG("PATTERN:%s", pattern.c_str());
		cmd << "COUNT" << count;
	}

	if ( !_getArry(cmd, arry) )
		return -1;

	CResult::ListCResult arrList = arry.getArry();
	if ( arrList.size() != 2 )
		return -2;

	CResult::ListCResult::const_iterator it = arrList.begin();

	val = it->getString(); //throw TypeErr
	std::istringstream istr(val);
	istr >> nextNo;

	if ( istr.fail() )
	{
		LOG_ERROR("%s: data received is unexpected",val.c_str());
	}
	LOG_DEBUG("nextNo:%ld", nextNo);

	++it;
	CResult::ListCResult::const_iterator itKeybgein = it->getArry().begin();
	CResult::ListCResult::const_iterator itKeyend = it->getArry().end();

	values.clear();
	while ( itKeybgein != itKeyend )
	{
		val = itKeybgein->getString();
		values.push_back(val);
		itKeybgein++;
	}

	return nextNo;
}

bool RedisClient::dump( const string& key , string& retStr )
{
	BuildCommand cmd("DUMP");
	cmd << key;
	return _getString(cmd, retStr);
}

bool RedisClient::restore( const string& key , const string& buf , const int& ttl )
{
	BuildCommand cmd("RESTORE");
	string status;
	cmd << key << ttl << buf;

	return _getStatus(cmd, status);
}

bool RedisClient::migrate(  const string& key ,const string& host , const uint16_t& port ,
		const uint16_t& db , const uint16_t& timeout )
{
	CResult result;
	BuildCommand cmd("MIGRATE");
	cmd << host << port << key << db << timeout;

	SharedPtr<RedisConnection> connObj = GetRedisConnection();
	if(connObj->SendCommand(cmd))
	    _getReply(result,connObj.get());
	else{
		LOG_ERROR("Send MIGRATE Command Error,cmd:%s",cmd.getCommand().c_str());
		return false;
	}

	ReplyType type = result.getType();
	if ( REDIS_REPLY_STATUS == type )
	{
		//"+NOKEY" may returned
		if (  result.compare(0,2,"OK")==0 || result.compare(0,2,"ok")==0 )
			return true;
	}

	return false;
}
