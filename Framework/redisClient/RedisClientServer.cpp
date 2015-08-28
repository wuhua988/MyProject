#include "BuildCommand.h"
#include "RedisClient.h"

string RedisClient::bgrewriteaof()
{
    BuildCommand cmd( "BGREWRITEAOF" );
    string str;
    _getStatus(cmd,str);
    return str;
}

string RedisClient::bgsave()
{
    BuildCommand cmd( "BGSAVE" );
    string str;
    _getStatus(cmd,str);
    return str;
}
string  RedisClient::clientGetname()
{
    BuildCommand cmd( "CLIENT" );
    cmd<<"GETNAME";
    string str;
    _getString(cmd,str);
    return str;

}
bool RedisClient::clientKill(const string& ip,const uint32_t port)
{
    BuildCommand cmd( "CLIENT" );
    cmd<<"KILL";
    cmd<<ip<<port;
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}
void RedisClient::clientList(CResult& result)
{
    BuildCommand cmd( "CLIENT" );
    cmd<<"LIST";
    _getReply(result,cmd);
}

bool RedisClient::clientSetname (const string& connectionName)
{
    BuildCommand cmd( "CLIENT" );
    cmd<<"SETNAME";
    cmd<<connectionName;

    string status;

    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}

UInt64 RedisClient::configGet(const string& parameter,VecString& reply)
{
    BuildCommand cmd( "CONFIG" );
    cmd<<"GET";
    cmd<<parameter;
    return _getArry(cmd,reply);
}

void RedisClient::configResetstat()
{
    BuildCommand cmd( "CONFIG" );
    cmd<<"RESETSTAT";
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        LOG_ERROR( "CONFIG RESETSTAT: data recved is not OK" );
}

bool RedisClient::configRewrite()
{
    BuildCommand cmd( "CONFIG" );
    cmd<<"REWRITE";
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}

bool RedisClient::configSet(const string& parameter,const string& value)
{
    BuildCommand cmd( "CONFIG" );
    cmd<<"SET";
    cmd<<parameter<<value;
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );
}

UInt64 RedisClient::dbsize()
{
    BuildCommand cmd( "DBSIZE" );
    int64_t num;
    _getInt(cmd,num);
    return num;

}

string RedisClient::debugObject(const string& key)
{
    BuildCommand cmd( "DEBUG" );
    cmd<<"OBJECT";
    cmd<<key;
    string str;
    _getString(cmd,str);
    return str;
}

void RedisClient::debugSegfault()
{
    BuildCommand cmd( "DEBUG" );
    cmd<<"SEGFAULT";
    //_socket.clearBuffer();
    //_sendCommand(cmd);
}

void RedisClient::flushall()
{
    BuildCommand cmd( "FLUSHALL" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        LOG_ERROR( "FLUSHALL: data recved is not OK" );
}

void RedisClient::flushdb()
{
    BuildCommand cmd( "FLUSHDB" );
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        LOG_ERROR( "FLUSHDB: data recved is not OK" );
}

UInt64 RedisClient::info(VecString& reply)
{
    //BuildCommand cmd( "INFO" );
    //_socket.clearBuffer();
    //_sendCommand(cmd);
    //string line;
    //_socket.readLine(line);
    //int64_t sumLen = _valueFromString<int64_t>( line.substr(1) );
    //int64_t len=0;
    //while(len<sumLen)
    //{
    //    _socket.readLine(line);
    //    reply.push_back(line);
    //    len+=line.length()+2;
    //}
    //return len;
	return 0;
}

UInt64 RedisClient::lastsave()
{
    BuildCommand cmd( "LASTSAVE" );
    int64_t num;
    _getInt(cmd,num);
    return num;
}

void RedisClient::monitor(void* input,void* output,void (*p)(string& str,void* in,void* out))
{
    string reply;
    BuildCommand cmd("MONITOR");
    //_socket.clearBuffer();
    //_socket.setReceiveTimeout(0);
    //_sendCommand(cmd);
    //if ((input==NULL) &&(p==NULL)&&(output==NULL))
    //while(1)
    //{
    //    _socket.readLine(reply);
    //    reply=reply.substr(1);
    //    std::cout<<reply<<std::endl;
    //}
    //while(1)
    //{
    //    _socket.readLine(reply);
    //    reply=reply.substr(1);
    //    p(reply,input,output);
    //}

}

bool RedisClient::save()
{
    BuildCommand cmd( "SAVE" );
    string status;
    _getStatus( cmd, status );
    return ("OK"==status? true : false );

}

string RedisClient::shutdown()
{
    BuildCommand cmd( "SHUTDOWN" );
    string status;
    _getStatus( cmd, status );
    return status;
}

void RedisClient::slaveof(const string& host,const string& port)
{
    BuildCommand cmd( "SLAVEOF" );
    cmd<<host<<port;
    string status;
    _getStatus( cmd, status );
    if ( "OK"!=status)
        LOG_ERROR( "SLAVEOF: data recved is not OK" );
}

void RedisClient::slowlog(const RedisClient::VecString &subcommand, CResult &reply)
{
    BuildCommand cmd( "SLOWLOG" );
    VecString::const_iterator it = subcommand.begin();
    VecString::const_iterator  end=subcommand.end();
    for ( ; it !=end; ++it )
    {
        cmd << *it;
    }
	
	_getReply(reply,cmd);
}


void RedisClient::time(string& currentseconds,string& microseconds)
{
    CResult result;
    BuildCommand cmd( "TIME" );
    _getArry(cmd,result);
    CResult::ListCResult::const_iterator it = result.getArry().begin();
    currentseconds=it->getString();
    ++it;
    microseconds=it->getString();
}
