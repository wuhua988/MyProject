#ifndef __Framework_h__
#define __Framework_h__
#include "OSHeaders.h"
#include "OSMutex.h"
#include "WatcherMgr.h"
#include "MsgBlock.h"
#include "ConnInfo.h"

class WatcherBase;
class ConnectPool;
class Framework
{
public:
    Framework();
    ~Framework();
    void         SendMsg(MsgBlock* pMsgBlock);
    virtual void OnMessage(MsgBlock* pMsgBlock);
                 
    virtual int  OnConnCreated(ConnInfo* pConnInfo);   
    virtual int  OnFailConnect(ConnInfo* pConnInfo); //主动连接失败。
    virtual int  OnConnClosed(ConnInfo* pConnInfo);
    ConnInfo*    GetConnInfo(UInt32 uiServerID,UInt32 uiConnID);
                 
   //virtual void OnTime();
    virtual void OnSignal(int signum);
    virtual int  Init(int eventThreadNum, int businThreadNum);
    virtual int  Run();
    
    int          NoticeTcpListen(char const* host,UInt16 iPort);        
    int          NoticeTcpConnect(UInt32 uiServerID
	                             ,char const* szAddr
		         				 ,UInt16 iPort
		         				 ,UInt32 uiConnNum
		         				 ,Float64 fExpire = 2
		         				 ,Float64 fInterval = 2
		         				 ,void* userData = NULL);
    int          NoticeCloseConnection(UInt32 uiServerID,UInt32 connID);
private:
    int m_iEventThreadNum;
    int m_iBusinThreadNum;
};
#endif //__Framework_h__