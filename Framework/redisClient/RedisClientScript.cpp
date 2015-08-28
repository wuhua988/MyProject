#include "BuildCommand.h"
#include "RedisClient.h"

bool RedisClient::eval( CResult& values , const string& script , const VecString& keysVec ,
		const VecString& argsVec )
{
	BuildCommand cmd("EVAL");
	string status;
	int len = keysVec.size();

	cmd << script << len;
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << keysVec[i];
	}

	len = argsVec.size();
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << argsVec[i];
	}

	return _getReply(values,cmd);
}

bool RedisClient::evalSha( CResult& values , const string& sha , const VecString& keysVec ,
		const VecString& argsVec )
{
	BuildCommand cmd("EVALSHA");
	string status;
	int len = keysVec.size();

	cmd << sha << len;
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << keysVec[i];
	}

	len = argsVec.size();
	for ( int i = 0 ; i < len ; i++ )
	{
		cmd << argsVec[i];
	}

	return _getReply(values,cmd);
}

bool RedisClient::scriptLoad( string& values , const string& script )
{
	BuildCommand cmd("SCRIPT");

	cmd << "LOAD" << script;
	return _getString(cmd, values);
}
bool RedisClient::scriptExists( const string& script )
{
	BuildCommand cmd("SCRIPT");
	cmd << "EXISTS" << script;

	CResult rst;
	_getArry(cmd, rst);
	CResult::ListCResult lst = rst.getArry();
	CResult::ListCResult::const_iterator it = lst.begin();
	return it->getInt();
}

bool RedisClient::scriptFlush( void )
{
	BuildCommand cmd("SCRIPT");
	string status;
	cmd << "FLUSH";

	if ( _getStatus(cmd, status) )
	{
		if ( status.find("OK") < 10 || status.find("ok") < 10 )
		{
			return true;
		}
	}
	return false;
}
bool RedisClient::scriptKill()
{
	BuildCommand cmd("SCRIPT");
	cmd << "KILL";

	string retStr;
	if ( _getStatus(cmd, retStr) )
	{

		return true;
	}
	return false;
}
