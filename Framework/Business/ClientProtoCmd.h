#ifndef __ClientProtoCmd_h__
#define __ClientProtoCmd_h__

#include "ProtoCmdBase.h"
#include "BusinessTimer.h"

class ServerProtoCmd;
class BusinessTimerThread;
class ClientProtoCmd : public ProtoCmdBase
{
public:
    ClientProtoCmd(ServerProtoCmd* pServerCmd,Framework* pFramework = NULL);
    virtual ~ClientProtoCmd();
	
    virtual void OnTimer();
    virtual bool SendRequest() = 0;
    virtual bool ScheduleCache(SharedPtr<ProtoCmdBase> cmdPtr,UInt32 uiServerID);
protected:
    virtual bool GetConnID(UInt32 uiServerID,UInt32& uiConnID) = 0;	
    void         StartTimer();
    void         StopTimer();
protected:
    UInt32                        m_uiUseConnID; //the connid who send it to network
    UInt32                        m_uiSeq;       //the Sequence of this cmd who used
	BusinessTimer<ClientProtoCmd> m_timer;
private:
    ServerProtoCmd*               m_pServerProtocolCmd;	
    BusinessTimerThread*          m_pTimerThread;
	
};

#endif //__ClientProtoCmd_h__
