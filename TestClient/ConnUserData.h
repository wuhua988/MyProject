#ifndef __ConnPrivateData_h__
#define __ConnPrivateData_h__
#include <map>
#include "OSMutex.h"
#include "Business/ProtoCmdBase.h"
#include "SharedPtr.h"
class ConnUserData
{
public:
    ConnUserData();
    ~ConnUserData();
    SharedPtr<ProtoCmdBase> GetCmd(UInt32 uiSeqID);
	SharedPtr<ProtoCmdBase> GetCmd_R(UInt32 uiSeqID);
    bool RemoveCmd(UInt32 uiSeqID);
    bool AddCmd(UInt32 uiSeqID,SharedPtr<ProtoCmdBase> cmdPtr);
private:
    ConnUserData(const ConnUserData& other);
    ConnUserData& operator=(const ConnUserData& other);
	typedef std::map<UInt32,SharedPtr<ProtoCmdBase> > seq2CmdMap;
    seq2CmdMap m_seq2Cmd;
    OSMutex m_cmdMutex;
};

#endif //__ConnPrivateData_h__