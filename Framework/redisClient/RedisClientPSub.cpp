#include "BuildCommand.h"
#include "RedisClient.h"

void RedisClient::psubscribe( VecString& pattern, CResult& result)
{
	//BuildCommand cmd( "PSUBSCRIBE" );
	//VecString::const_iterator it = pattern.begin();
	//for ( ; it != pattern.end(); ++it )
	//{
	//	cmd << *it;
	//}
	//result.clear();
	//_getArry( cmd, result );
	//while(true)
	//{
	//	result.clear();
	//	_getReply( result );
	//}
}



UInt64 RedisClient::publish( const string& channel, const string& message )
{
	BuildCommand cmd( "PUBLISH" );
	cmd << channel << message;
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

UInt64 RedisClient::psubchannels( VecString& value, const VecString& pattern )
{
	BuildCommand cmd( "PUBSUB" );
	cmd << "CHANNELS";

	if ( pattern.size() != 0 )
	{
		VecString::const_iterator it = pattern.begin();
		for ( ; it != pattern.end(); ++it )
		{
			cmd << *it ;
		}
	}

	return _getArry( cmd, value );
}

UInt64 RedisClient::psubnumsub( RedisClient::MapString& value, const VecString& channel )
{
	BuildCommand cmd( "PUBSUB" );
	cmd << "NUMSUB";
	if ( channel.size() != 0 )
	{
		VecString::const_iterator it = channel.begin();
		for ( ; it != channel.end(); ++it )
		{
			cmd << *it ;
		}
	}

	return _getArry( cmd, value );
}

UInt64 RedisClient::psubnumpat()
{
	BuildCommand cmd( "PUBSUB" );
	cmd << "NUMPAT";
	int64_t num = 0;
	_getInt( cmd, num );
	return num;
}

void RedisClient::punsubscribe( CResult& result, const VecString& pattern )
{
	BuildCommand cmd( "PUNSUBSCRIBE" );
	if ( pattern.size() != 0 )
	{
		VecString::const_iterator it = pattern.begin();
		for ( ; it != pattern.end(); ++it )
		{
			cmd << *it ;
		}
	}
	
	_getReply(result,cmd);
}

void RedisClient::subscribe( VecString& channel, CResult& result )
{
	BuildCommand cmd( "SUBSCRIBE" );
	VecString::const_iterator it = channel.begin();
	for ( ; it != channel.end(); ++it )
	{
		cmd << *it ;
	}

	//result.clear();
	//_getArry( cmd, result );
	//while(true)
	//{
	//	result.clear();
	//	_getReply( result );
	//}
}

void RedisClient::unsubscribe( CResult& result, const VecString& channel )
{
	BuildCommand cmd( "UNSUBSCRIBE" );
	if ( channel.size() != 0 )
	{
		VecString::const_iterator it = channel.begin();
		for ( ; it != channel.end(); ++it )
		{
			cmd << *it ;
		}
	}
    _getReply(result,cmd);
}
