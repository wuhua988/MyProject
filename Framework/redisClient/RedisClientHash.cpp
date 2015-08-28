#include "BuildCommand.h"
#include "RedisClient.h"

//------------------------------hash method-----------------------------------
UInt8 RedisClient::hset(const std::string &key, const std::string &field, const std::string &value)
{
   BuildCommand cmd( "HSET" );
   cmd << key << field << value;
   int64_t num = 0;
   _getInt( cmd , num );
   return num;
}

bool RedisClient::hget( const std::string &key, const std::string &field, string &value )
{
    BuildCommand cmd( "HGET" );
    cmd << key << field;
    return _getString( cmd , value );
}

UInt64 RedisClient::hdel( const string &key, const RedisClient::VecString &fields )
{
    BuildCommand cmd( "HDEL" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    int64_t num = 0;
    _getInt( cmd , num );
    return num;
}

bool RedisClient::hexists(const string &key, const string &field)
{
    BuildCommand cmd( "HEXISTS" );
    cmd << key << field;
    int64_t num = 0;
    _getInt( cmd, num );
    return ( num==1 ? true:false );
}

UInt64 RedisClient::hgetall(const string &key, RedisClient::MapString &pairs)
{
    BuildCommand cmd( "HGETALL" );
    cmd << key;

    _getArry( cmd, pairs );
    return pairs.size();
}

UInt64 RedisClient::hincrby(const string &key, const string &field, UInt64 increment)
{
    BuildCommand cmd( "HINCRBY" );
    cmd << key << field << increment;

    int64_t num = 0;
    _getInt( cmd, num );
    return num;
}

float RedisClient::hincrbyfloat(const string &key, const string &field, float increment)
{
    BuildCommand cmd( "HINCRBYFLOAT" );
    cmd << key << field << increment;
    string value;
    _getString( cmd , value );
    return _valueFromString<float>(  value );
}

UInt64 RedisClient::hkeys(const string &key, RedisClient::VecString &values)
{
    BuildCommand cmd( "HKEYS" );
    cmd << key;

    _getArry( cmd, values );
    return values.size();
}

UInt64 RedisClient::hlen(const string &key)
{
   BuildCommand cmd( "HLEN" );
   cmd << key;
   int64_t num = 0;
   _getInt( cmd, num );
   return num;
}

void RedisClient::hmget(const string &key, const RedisClient::VecString &fields, CResult &result)
{
    BuildCommand cmd( "HMGET" );
    cmd << key;

    VecString::const_iterator it = fields.begin();
    VecString::const_iterator end = fields.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd , result );
}

void RedisClient::hmset(const string &key, const RedisClient::MapString &pairs, CResult &result)
{
    BuildCommand cmd( "HMSET" );
    cmd << key;
    MapString::const_iterator it = pairs.begin();
    MapString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << it->first;
        cmd << it->second;
    }

	SharedPtr<RedisConnection> connObj = GetRedisConnection();
	
	if(connObj->SendCommand(cmd))
        _getReply( result,connObj.get());
}

void RedisClient::hmset(const string &key, const RedisClient::MapString &pairs)
{
    BuildCommand cmd( "HMSET" );
    cmd << key;
    MapString::const_iterator it = pairs.begin();
    MapString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << it->first;
        cmd << it->second;
    }
    string status;
    _getStatus( cmd, status );
}

bool RedisClient::hsetnx(const string &key, const string &field, const string &value)
{
    BuildCommand cmd( "HSETNX" );
    cmd << key << field << value;
    int64_t num = 0;
    _getInt( cmd, num );
    return ( num==1 ? true:false );
}

UInt64 RedisClient::hvals(const string &key, RedisClient::VecString &values)
{
    BuildCommand cmd( "HVALS" );
    cmd << key ;
    _getArry( cmd, values );
    return values.size();
}

bool RedisClient::hscan(const string &key, int64_t cursor, MapString &values, const string &match, UInt64 count )
{
    static UInt64 lastCur = 0;
    UInt64 realCur = 0;
    CResult result;

    if ( cursor >= 0 )
    {
        realCur = cursor;
    }else
    {
        realCur = lastCur;
    }

    BuildCommand cmd( "HSCAN" );
    cmd << key << realCur;

    if ( "" != match )
    {
          cmd << "MATCH" << match;
    }

    if ( 0 != count )
    {
           cmd << "COUNT" << count;
    }

    _getArry( cmd, result );
    CResult::ListCResult::const_iterator it = result.getArry().begin();
   lastCur = _valueFromString<UInt64>( it->getString() );
   ++it;
   _getStringMapFromArry( it->getArry(), values );
   return ( lastCur == 0 ? false : true );
}
