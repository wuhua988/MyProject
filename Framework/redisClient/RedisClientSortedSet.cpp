#include "BuildCommand.h"
#include "RedisClient.h"

UInt64 RedisClient::zadd(const string &key, const RedisClient::MapString &pairs)
{
    BuildCommand cmd( "ZADD" );
    cmd << key;
    MapString::const_iterator it = pairs.begin();
    MapString::const_iterator end = pairs.end();

    for ( ; it !=end ; ++it )
    {
        cmd << it->first;
        cmd << it->second;
    }
    int64_t num;
    _getInt(cmd,num);
    return num;
}

UInt64 RedisClient::zcard(const string& key)
{
    BuildCommand cmd( "ZCARD" );
    cmd << key;
    int64_t num;
    _getInt(cmd,num);
    return num;

}



UInt64 RedisClient::zcount(const string& key,const string& min,const string& max)
{
    BuildCommand cmd( "ZCOUNT" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}



string RedisClient::zincrby(const string& key,float increment,const string& member)
{
    BuildCommand cmd( "ZINCRBY" );
    cmd << key << increment<< member;
    string str;
    _getString(cmd,str);
    return str;
}





UInt64 RedisClient::zrange(const string &key, const int64_t start, const int64_t stop, RedisClient::VecString& reply)
{
    BuildCommand cmd( "ZRANGE" );
    cmd << key << start<< stop;
    return   _getArry(cmd,reply);
}
UInt64 RedisClient::zrange(const string &key, const int64_t start, const int64_t stop, RedisClient::MapString &reply)
{
    BuildCommand cmd( "ZRANGE" );
    cmd << key << start<< stop;
    cmd<<"WITHSCORES";
    return   _getArry(cmd,reply);
}


UInt64 RedisClient::zrangebyscore(const string &key, const string &min, const string &max, RedisClient::VecString &reply,int64_t offset,int64_t count)
{
    BuildCommand cmd( "ZRANGEBYSCORE" );
    cmd << key << min<< max;

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

UInt64 RedisClient::zrangebyscore(const string &key, const string &min, const string &max, RedisClient::MapString &reply,int64_t offset,int64_t count)
{
    BuildCommand cmd( "ZRANGEBYSCORE" );
    cmd << key << min<< max;
    cmd<<"WITHSCORES";

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;
    return   _getArry(cmd,reply);
}

bool RedisClient::zrank(const string& key,const string& member,int64_t& reply)
{
    BuildCommand cmd( "ZRANK" );
    cmd << key << member;

    return _getInt(cmd,reply);
}


UInt64 RedisClient::zrem(const string& key,VecString& members)

{
    BuildCommand cmd( "ZREM" );
    cmd << key;

    VecString::const_iterator it = members.begin();
    VecString::const_iterator end=members.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
    int64_t num;
    _getInt(cmd,num);
    return num;
}


UInt64 RedisClient::zremrangebyrank(const string& key,const int64_t start,const int64_t stop)
{
    BuildCommand cmd( "ZREMRANGEBYRANK" );
    cmd << key << start << stop;
    int64_t num;
    _getInt(cmd,num);
    return num;

}



UInt64 RedisClient::zremrangebyscore(const string& key,const string& min,const string& max)
{
    BuildCommand cmd( "ZREMRANGEBYSCORE" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}

UInt64 RedisClient::zrevrange(const string &key, const int64_t start, const int64_t stop, RedisClient::VecString &reply)
{
    BuildCommand cmd( "ZREVRANGE" );
    cmd << key << start<< stop;
    return   _getArry(cmd,reply);
}

UInt64 RedisClient::zrevrange(const string &key, const int64_t start, const int64_t stop, RedisClient::MapString &reply)
{
    BuildCommand cmd( "ZREVRANGE" );
    cmd << key << start<< stop;
    cmd<<"WITHSCORES";
    return   _getArry(cmd,reply);
}





UInt64 RedisClient::zrevrangebyscore(const string &key, const string &max, const string &min, RedisClient::VecString &reply, int64_t offset,int64_t count)
{
    BuildCommand cmd( "ZREVRANGEBYSCORE");
    cmd << key << max<< min;

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

UInt64 RedisClient::zrevrangebyscore(const string &key, const string &max, const string &min, RedisClient::MapString &reply, int64_t offset,int64_t count)
{
    BuildCommand cmd( "ZREVRANGEBYSCORE");
    cmd << key << max<< min;
    cmd<<"WITHSCORES";

    if(0!=count)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}



UInt64 RedisClient::zrevrank(const string& key,const string& member)
{
    BuildCommand cmd( "ZREVRANK" );
    cmd << key << member;
    int64_t num;
    _getInt(cmd,num);
    return num;
}





string RedisClient::zscore(const string& key,const string& member)
{
    BuildCommand cmd( "ZSCORE" );
    cmd << key <<  member;
    string str;
    _getString(cmd,str);
    return str;
}

void RedisClient::addAggregate(BuildCommand& cmd,int aggregate)
{
    static const char * aggre[]={"SUM","MIN","MAX"};
    cmd <<"AGGREGATE";
    cmd<<aggre[aggregate];
}



UInt64 RedisClient::zunionstore (const string& destination,const UInt64 numkeys,const VecString& keys,const VecString& weigets,int aggregate)
{

    BuildCommand cmd( "ZUNIONSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }

    cmd <<"WEIGHTS";
    VecString::const_iterator it2 = weigets.begin();
    VecString::const_iterator end2 = weigets.end();
    for ( ; it2 !=end2; it2++ )
    {
        cmd << *it2;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}


UInt64 RedisClient::zunionstore (const string& destination,const UInt64 numkeys,const VecString& keys,int aggregate)
{
    BuildCommand cmd( "ZUNIONSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}

//.................................................................................................................

UInt64 RedisClient::zinterstore (const string& destination,const UInt64 numkeys,const VecString& keys,const VecString& weigets,int aggregate)
{
    BuildCommand cmd( "ZINTERSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }

    cmd <<"WEIGHTS";
    VecString::const_iterator it2 = weigets.begin();
    VecString::const_iterator end2 = weigets.end();
    for ( ; it2 !=end2; it2++ )
    {
        cmd << *it2;
    }

    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;

}


UInt64 RedisClient::zinterstore (const string& destination,const UInt64 numkeys,const VecString& keys,int aggregate)
{

    BuildCommand cmd( "ZINTERSTORE" );
    cmd << destination <<  numkeys;

    VecString::const_iterator it = keys.begin();
    VecString::const_iterator end = keys.end();
    for ( ; it !=end; it++ )
    {
        cmd << *it;
    }
    addAggregate(cmd,aggregate);
    int64_t num;
    _getInt(cmd,num);
    return num;


}
bool RedisClient::zscan(const string &key, int64_t cursor, MapString &reply, const string &match, UInt64 count )
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

    BuildCommand cmd( "ZSCAN" );
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
    _getStringMapFromArry( it->getArry(),reply );
    return ( lastCur == 0 ? false : true );
}

UInt64 RedisClient::zrangebylex(const string &key, const string &min, const string &max, RedisClient::VecString &reply, int64_t offset,int64_t count)
{
    BuildCommand cmd( "ZRANGEBYLEX" );
    cmd << key << min<< max;
    if (count>0)
        cmd<<"LIMIT"<<offset<<count;

    return   _getArry(cmd,reply);
}

UInt64 RedisClient::zlexcount(const string &key, const string &min, const string &max)
{
    BuildCommand cmd( "ZLEXCOUNT" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;

}

UInt64 RedisClient::zremrangebylex(const string &key, const string &min, const string &max)
{
    BuildCommand cmd( "ZREMRANGEBYLEX" );
    cmd << key << min << max;
    int64_t num;
    _getInt(cmd,num);
    return num;
}
