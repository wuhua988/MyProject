#include <map>
#include "ProtoCmdBase.h"
#include "log.h"
#include "OS.h"

AtomicCounter<UInt32> ProtoCmdBase::m_seqID = AtomicCounter<UInt32>(0);
static std::map<UInt16,ClassInfo*> *classInfoMap = NULL;
ProtoCmdBase::ProtoCmdBase(Framework* pFramework)
:m_uCmdID(0)
,m_requestTime(0)
,m_responseTime(0)
,m_pFramework(pFramework)
{}

ProtoCmdBase::~ProtoCmdBase()
{}

void ProtoCmdBase::SetCmdID(UInt16 cmdID)
{
	m_uCmdID = cmdID;
}

UInt16 ProtoCmdBase::GetCmdID()
{
	return m_uCmdID;
}

void ProtoCmdBase::SetFramework(Framework* pFramework)
{
	m_pFramework = pFramework;
}

Framework* ProtoCmdBase::GetFramework()
{
	return m_pFramework;
}

void ProtoCmdBase::SendMessage(UInt32 uiServerID
                              ,UInt32 uiConnID
                              ,UInt32 sequenceID
							  ,UInt16 cmdID
							  ,UInt8 contentType
							  ,UInt16 errCode
							  ,const std::string &data)
{
	MsgHeader header;
    header.SetVersion(1);
    header.SetCrypt(1);
    header.SetMode(1);
    header.SetContentType(contentType);
    header.SetCommand(cmdID);
    header.SetApplicationId(1);
	header.SetError(errCode);
    header.SetSequence(sequenceID); 
	MsgBlock* pMsgBlock = MsgBlock::PackMsg(header,data.c_str(),data.length(),uiServerID,uiConnID);
	SendMessage(pMsgBlock);
	m_requestTime = OS::Milliseconds();
}

void ProtoCmdBase::SendMessage(MsgBlock* pMsgBlock)
{
	assert(m_pFramework != NULL);
	assert(pMsgBlock != NULL);
	m_pFramework->SendMsg(pMsgBlock);
}

ProtoCmdBase* ProtoCmdBase::CreateObject(UInt16 cmdID)
{
	std::map<UInt16,ClassInfo*>::const_iterator iter = classInfoMap->find(cmdID);
	if(classInfoMap->end() != iter)	{
		return iter->second->CreateObject();
	}	
	return NULL;
}

bool ProtoCmdBase::Register(ClassInfo* ci)
{
	if(!classInfoMap)
	{
		classInfoMap = new std::map<UInt16,ClassInfo*>();
	}
	
	if(ci)
	{
		if(classInfoMap->find(ci->m_cmdID) == classInfoMap->end())
		{
			classInfoMap->insert(std::map<UInt16,ClassInfo*>::value_type(ci->m_cmdID,ci));
		}
	}
	return true;
}

UInt32 ProtoCmdBase::GetSeqID()
{
	m_seqID++;
	return m_seqID.value();
}