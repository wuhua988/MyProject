#ifndef __ProtoCmdBase_h__
#define __ProtoCmdBase_h__
#include <string>
#include "OSHeaders.h"
#include "../MsgBlock.h"
#include "../Framework.h"
#include "DynBase.h"

class ProtoCmdBase
{
public:
    explicit ProtoCmdBase(Framework* pFramework = NULL);
	virtual ~ProtoCmdBase();
	
    void         SetCmdID(UInt16 cmdID);
    UInt16       GetCmdID();
	void         SetFramework(Framework* pFramework);
	Framework*   GetFramework();
    virtual void OnResponse(MsgBlock* pMsgBlock) = 0;
	
	static bool  Register(ClassInfo* ci);           //”≥…‰
	static       ProtoCmdBase* CreateObject(UInt16 cmdID);  
    static       UInt32 GetSeqID();	
protected:
    void         SendMessage(UInt32 uiServerID
	                        ,UInt32 uiConnID	
	                        ,UInt32 sequenceID
		  	      		    ,UInt16 cmdID
		  	      		    ,UInt8 contentType
		  	      		    ,UInt16 errCode
		  	      		    ,const std::string &data);
    void         SendMessage(MsgBlock* pMsgBlock);
	UInt16                       m_uCmdID;
	SInt64                       m_requestTime;
	SInt64                       m_responseTime;
private:                         
	Framework*                   m_pFramework;
	static AtomicCounter<UInt32> m_seqID;
	
    ProtoCmdBase(const ProtoCmdBase& other);
	ProtoCmdBase& operator = (const ProtoCmdBase& other);
};

#endif //__CtlProtoCmd_h__