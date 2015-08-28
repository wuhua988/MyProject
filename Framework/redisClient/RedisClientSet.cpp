#include "BuildCommand.h"
#include "RedisClient.h"

UInt64 RedisClient::sadd(const string &key, const RedisClient::VecString &members)
{
    BuildCommand cmd( "SADD" );
    cmd << key;
    VecString::const_iterator it = members.begin();
    VecString::const_iterator end = members.end();

    for ( ; it != end ; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd , num );
    return num;
}


UInt64 RedisClient::scard(const string &key)
{
    BuildCommand cmd( "SCARD" );
    cmd << key;
    int64_t num;
    _getInt( cmd , num );
    return num;
}




UInt64 RedisClient::sdiff(const RedisClient::VecString &keys, RedisClient::VecString &values)
{
    BuildCommand cmd( "SDIFF" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd, values );
    return values.size();
}


UInt64 RedisClient::sdiffstore(const string &destKey, const RedisClient::VecString &keys )
{
    BuildCommand cmd( "SDIFFSTORE" );
     cmd << destKey;
     VecString::const_iterator it = keys.begin();
     VecString::const_iterator end = keys.end();
     for ( ; it != end; ++it )
     {
         cmd << *it;
     }

     int64_t num = 0;
     _getInt( cmd, num );
     return num;
}

UInt64 RedisClient::sinter(const RedisClient::VecString &keys, VecString &values)
{
    BuildCommand cmd( "SINTER" );
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd, values );
    return values.size();
}

UInt64 RedisClient::sinterstore( const string& destKey ,const RedisClient::VecString &keys )
{
    BuildCommand cmd( "SINTERSTORE" );
    cmd << destKey;
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    int64_t num = 0;
    _getInt( cmd, num );
    return num;
}

bool RedisClient::sismember(const string &key, const string &member)
{
    BuildCommand cmd( "SISMEMBER" );
    cmd << key << member;

    int64_t num = 0;
    _getInt( cmd, num );
    return ( num == 1 ? true: false );
}

UInt64 RedisClient::smembers( const string &key, RedisClient::VecString &members )
{
    BuildCommand cmd( "SMEMBERS" );
    cmd << key;
    _getArry( cmd, members );
    return members.size();
}

bool RedisClient::smove(const string &source, const string &dest, const string &member)
{
    BuildCommand cmd( "SMOVE" );
    cmd << source << dest << member;
    int64_t num;
    _getInt( cmd, num );
    return	( num==0 ? false : true );
}

bool RedisClient::spop(const string &key, string &member)
{
    member.clear();
    BuildCommand cmd( "SPOP" );
    cmd << key;

    return ( _getString( cmd, member ) );
}

bool RedisClient::srandmember(const string &key, string &member)
{
    BuildCommand cmd ( "SRANDMEMBER" );
    cmd << key;
    return ( _getString( cmd, member) );
}

UInt64 RedisClient::srandmember(const string &key, int count, RedisClient::VecString &members)
{
    BuildCommand cmd( "SRANDMEMBER" );
    cmd << key << count ;

    _getArry( cmd, members );
    return members.size();
}

UInt64 RedisClient::srem(const string &key, RedisClient::VecString &members)
{
    BuildCommand cmd ( "SREM" );
    cmd << key;

    VecString::const_iterator it = members.begin();
    VecString::const_iterator end = members.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd, num );
    return num;
}

UInt64 RedisClient::sunion(const RedisClient::VecString &keys , VecString &members)
{
    BuildCommand cmd( "SUNION" );

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getArry( cmd, members );
    return members.size();
}

UInt64 RedisClient::sunionstroe(const string &dest, const RedisClient::VecString &keys)
{
    BuildCommand cmd( "SUNIONSTORE" );
    cmd << dest;
    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it != end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt( cmd, num );
    return num;
}

bool RedisClient::sscan(const string &key, int64_t cursor, VecString &values, const string &match, UInt64 count)
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

     BuildCommand cmd( "SSCAN" );
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

    _getStringVecFromArry( it->getArry(), values );
    return ( lastCur == 0 ? false : true );
}
