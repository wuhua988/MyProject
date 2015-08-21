#include "ConnUserData.h"

ConnUserData::ConnUserData()
{
	
}
ConnUserData::~ConnUserData()
{
	OSMutexLocker theLocker(&m_cmdMutex);
	m_seq2Cmd.clear();
}

SharedPtr<ProtoCmdBase> ConnUserData::GetCmd(UInt32 uiSeqID)
{
	OSMutexLocker theLocker(&m_cmdMutex);
	seq2CmdMap::iterator iter = m_seq2Cmd.find(uiSeqID);
	if(iter == m_seq2Cmd.end())
		return SharedPtr<ProtoCmdBase>();
	return iter->second;
}

SharedPtr<ProtoCmdBase> ConnUserData::GetCmd_R(UInt32 uiSeqID)
{
	OSMutexLocker theLocker(&m_cmdMutex);
	SharedPtr<ProtoCmdBase> cmdPtr;
	seq2CmdMap::iterator iter = m_seq2Cmd.find(uiSeqID);
	if(iter == m_seq2Cmd.end())
		return cmdPtr;
	cmdPtr = iter->second;
	m_seq2Cmd.erase(iter);
	return cmdPtr;
}

bool ConnUserData::RemoveCmd(UInt32 uiSeqID)
{
	OSMutexLocker theLocker(&m_cmdMutex);
	seq2CmdMap::iterator iter = m_seq2Cmd.find(uiSeqID);
	if(iter == m_seq2Cmd.end())
		return false;
	m_seq2Cmd.erase(iter);
	return true;
}

bool ConnUserData::AddCmd(UInt32 uiSeqID,SharedPtr<ProtoCmdBase> cmdPtr)
{
	OSMutexLocker theLocker(&m_cmdMutex);
	seq2CmdMap::iterator iter = m_seq2Cmd.find(uiSeqID);
	if(iter != m_seq2Cmd.end())
	{
		LOG_DEBUG("The SeqID:%u,is Already Existed in map",uiSeqID);
		return false;
	}
	
	m_seq2Cmd[uiSeqID] = cmdPtr;
	return true;
}