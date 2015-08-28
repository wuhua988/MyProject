#include "BuildCommand.h"
#include "RedisClient.h"

void RedisClient::watch(const RedisClient::VecString &keys)
{
    BuildCommand cmd( "WATCH" );

    VecString::const_iterator it = keys.begin();
    for ( ; it != keys.end(); ++it )
    {
        cmd << *it ;
    }

    string status;
    _getStatus( cmd, status );

    if ( "OK" != status )
    {
        LOG_ERROR( "WATCH recv unexpected data:%s " , status.c_str());
    }
}

void RedisClient::unwatch()
{
    BuildCommand cmd( "UNWATCH" );
    string status;
    _getStatus( cmd, status );

   if ( "OK" != status )
   {
       LOG_ERROR( "UNWATCH recv unexpected data:%s " , status.c_str());
   }
 }


void RedisClient::multi( void )
{
    BuildCommand cmd("MULTI");
    string status;
    _getStatus( cmd, status );

    if ( "OK" != status )
    {
        LOG_ERROR( "MULTI recv unexpected data:%s " , status.c_str());
    }
}

bool RedisClient::transactionCmd(const std::string &cmmand,VecString &params)
{
    CResult result;
    BuildCommand cmd( cmmand );

    VecString::const_iterator it = params.begin();
    VecString::const_iterator end = params.end();

    for ( ; it != end; ++it )
    {
        cmd << *it;
    }

    _getReply( result,cmd );
   if ( result.getType() != REDIS_REPLY_STATUS || result != "QUEUED" )
   {
       return false;
   }else
   {
       return true;
   }
}

//void RedisClient::discard( void )
//{
//    _socket.clearBuffer();;
//
//    BuildCommand cmd( "DISCARD" );
//
//    _sendCommand( cmd );
//    if ( _replyStatus( ) == "OK" )
//    {
//           return;
//    }else
//    {
//            throw ProtocolErr("DISCARD: data recved is not OK");
//    }
//}
//

void RedisClient::exec( CResult &result )
{
    BuildCommand cmd( "EXEC" );

    _getReply( result,cmd );
    if ( result.getType() != REDIS_REPLY_ARRAY )
    {
        LOG_ERROR( "EXEC received uncecepted data" );
    }
}





