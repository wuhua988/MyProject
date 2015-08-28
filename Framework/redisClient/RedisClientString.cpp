#include "BuildCommand.h"
#include "RedisClient.h"

//-----------------------------string method--------------------------------------


UInt64 RedisClient::append( const string& key, const string& value )
{
	BuildCommand cmd( "APPEND" );
	cmd << key << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

UInt64 RedisClient::bitcount( const string& key, int64_t start, int64_t end )
{
	BuildCommand cmd( "BITCOUNT" );
	cmd << key << start << end;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


UInt64 RedisClient::bitop( const string& operation, const string& destkey, VecString& keys )
{
	BuildCommand cmd( "BITOP" );
	cmd << operation << destkey;
	VecString::const_iterator it = keys.begin();
	for ( ; it != keys.end(); ++it )
	{
		cmd << *it;
	}
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

int64_t RedisClient::decr( const string& key )
{
	BuildCommand cmd( "DECR" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

int64_t RedisClient::decrby( const string& key, int64_t decrement )
{
	BuildCommand cmd( "DECRBY" );
	cmd << key << decrement;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

bool RedisClient::get( const std::string &key, std::string &value )
{
    BuildCommand cmd( "GET" );
    cmd << key;
    return _getString( cmd, value );
}

uint8_t RedisClient::getbit( const string& key, uint32_t offset )
{
	BuildCommand cmd( "GETBIT" );
	cmd << key << offset;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

bool RedisClient::getrange( const string& key, int64_t start, int64_t end, string &value )
{
	value.clear();
	BuildCommand cmd( "GETRANGE" );
	cmd << key << start << end;

	return _getString( cmd, value );
}

bool RedisClient::getset(const string& key, const string &value, string &oldvalue )
{
	oldvalue.clear();
	BuildCommand cmd( "GETSET" );
	cmd << key << value;
	return _getString( cmd, oldvalue );
}

int64_t RedisClient::incr( const string& key )
{
	BuildCommand cmd( "INCR" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

int64_t RedisClient::incrby( const string& key, int64_t increment )
{
	BuildCommand cmd( "INCRBY" );
	cmd << key << increment;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

bool RedisClient::incrbyfloat( const string& key, float increment, float& value )
{
	BuildCommand cmd( "INCRBYFLOAT" );
	cmd << key << increment;

	string strVal;
	bool ret = _getString( cmd, strVal );
	value = _valueFromString<float>( strVal );

	return ret;
}

void RedisClient::mget(VecString& keys, CResult& result )
{
	BuildCommand cmd( "MGET" );
	VecString::const_iterator it = keys.begin();
	for ( ; it != keys.end(); ++it )
	{
		cmd << *it;
	}

	_getArry( cmd, result );
}

void RedisClient::mset( RedisClient::MapString &value )
{
	BuildCommand cmd( "MSET" );
	RedisClient::MapString::const_iterator it = value.begin();
	for ( ; it != value.end(); ++it )
	{
		cmd << it->first;
		cmd << it->second;
	}

	string status;
	_getStatus( cmd, status );
	if ( status != "OK" )
		LOG_ERROR( "MSET: data recved is not OK" );
}

uint8_t RedisClient::msetnx( RedisClient::MapString &value )
{
	BuildCommand cmd( "MSETNX" );
	RedisClient::MapString::const_iterator it = value.begin();
	for ( ; it != value.end(); ++it )
	{
		cmd << it->first;
		cmd << it->second;
	}

	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

void RedisClient::_set(const string &key, const string &value, CResult &result, const string& suffix , long time,const string suffix2 )
{
    BuildCommand cmd( "SET" );
    cmd << key << value;

    if ( suffix != "" )
    {
        cmd << suffix;
    }

    if ( time != 0 )
    {
        std::stringstream ss;
        ss << time ;
        cmd << ss.str();
    }
    if ( suffix2 != "" )
    {
        cmd << suffix2;
    }

    _getReply( result,cmd );
}

void RedisClient::set(const std::string &key,const std::string &value)
{
    CResult result;
    _set( key, value, result );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s", result.getErrorString().c_str());
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        if ( result.getStatus() == "OK" )
        {
            return;
        }else
        {
            LOG_ERROR( "SET: data recved is not OK" );
        }
    }else
    {
        LOG_ERROR( "SET: data recved is not status" );
    }
}

bool RedisClient::setEX(const std::string &key, const std::string &value, long time, SET_OPTION opt )
{
    string suffix;
    if ( DEFAULT == opt )
    {
        suffix.clear();
    }
    else if ( NX == opt )
    {
        suffix = "NX";
    }else if ( XX == opt )
    {
        suffix = "XX";
    }else
    {
        return false;
    }

    CResult result;
    _set( key, value, result,"EX", time,suffix );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s", result.getErrorString().c_str());
    }
    else if ( result.getType() == REDIS_REPLY_STATUS )
    {
       return true;
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else
    {
        LOG_ERROR( "SET: data recved is not status" );
    }
	return false;
}

bool RedisClient::setPX(const std::string &key, const std::string &value, long time, SET_OPTION opt)
{
    string suffix;
    if ( DEFAULT == opt )
    {
        suffix.clear();
    }
    else if ( NX == opt )
    {
        suffix = "NX";
    }else if ( XX == opt )
    {
        suffix = "XX";
    }else
    {
        return false;
    }

    CResult result;
    _set( key, value, result,"PX", time,suffix );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s", result.getErrorString().c_str());
    }
    else if ( result.getType() == REDIS_REPLY_STATUS )
    {
       return true;
    }else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else
    {
        LOG_ERROR( "SET: data recved is not status" );
    }
	return false;
}

bool RedisClient::setNX(const std::string &key, const std::string &value)
{
    CResult result;
    _set( key, value, result,"NX" );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
       LOG_ERROR("%s", result.getErrorString().c_str());
    } else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        return true;
    }else
    {
        LOG_ERROR( "SET: data recved is not status" );
    }
	return false;
}

bool RedisClient::setXX(const std::string &key, const std::string &value)
{
    CResult result;
    _set( key, value, result,"XX" );

    if ( result.getType() == REDIS_REPLY_ERROR )
    {
       LOG_ERROR("%s", result.getErrorString().c_str());
    } else if ( result.getType() == REDIS_REPLY_NIL )
    {
        return false;
    }else if ( result.getType() == REDIS_REPLY_STATUS )
    {
        return true;
    }else
    {
        LOG_ERROR( "SET: data recved is not status" );
    }
	return false;
}


uint8_t RedisClient::setbit( const string& key, uint32_t offset, const string& value )
{
	BuildCommand cmd( "SETBIT" );
	cmd << key << offset << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


UInt64 RedisClient::setrange( const string& key, uint32_t offset, const string& value )
{
	BuildCommand cmd( "SETRANGE" );
	cmd << key << offset << value;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}


UInt64 RedisClient::strlen( const string& key )
{
	BuildCommand cmd( "STRLEN" );
	cmd << key;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}
