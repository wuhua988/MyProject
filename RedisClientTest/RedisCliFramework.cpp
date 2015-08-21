#include "RedisCliFramework.h"

void RedisCliFramework::OnMessage(MsgBlock* pMsgBlock)
{
	assert(pMsgBlock != NULL);
}

int RedisCliFramework::OnConnCreated(ConnInfo* pConnInfo) 
{
	return 0;
}

int RedisCliFramework::OnFailConnect(ConnInfo* pConnInfo)
{
	return 0;
}

int RedisCliFramework::OnConnClosed(ConnInfo* pConnInfo)
{
	return 0;
}