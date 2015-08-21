#include "ServerProtoCmd.h"

ServerProtoCmd::ServerProtoCmd(Framework* pFramework)
: ProtoCmdBase(pFramework)
{
	
}

ServerProtoCmd::~ServerProtoCmd()
{
	m_clientMap.clear();
	if(!m_pOriMsgBlock)
		delete m_pOriMsgBlock;
}

void ServerProtoCmd::SendClientRequest()
{
	
}

void ServerProtoCmd::SendResponse()
{
	
}

void ServerProtoCmd::SendErrResponse()
{
	
}

void ServerProtoCmd::OnTimer(int state)
{
	LOG_TRACE("ServerCommand OnTimer Nothing To DO!");
}
