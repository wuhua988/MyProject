#ifndef __CliCmd_h__
#define __CliCmd_h__
#include "Business/ClientProtoCmd.h"
#include "OS.h"

class CliCmd : public ClientProtoCmd
{
public:
    CliCmd();
    explicit CliCmd(Framework* pFramework);
	virtual ~CliCmd();	
	virtual bool ScheduleCache(SharedPtr<ProtoCmdBase> cmdPtr,UInt32 uiServerID);
	void OnResponse();
protected:
    virtual bool GetConnID(UInt32 uiServerID,UInt32& uiConnID);
	bool OnTimer(UInt32 uiServerID);
};

#endif //__CliCmd_h__