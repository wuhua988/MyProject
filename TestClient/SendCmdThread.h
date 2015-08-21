#ifndef __SendCmdThread_h__
#define __SendCmdThread_h__
#include "OSThread.h"

class SendCmdThread : public OSThread
{
public:
    virtual ~SendCmdThread();
	SendCmdThread();
    virtual void Entry();
	void SetContinue(bool bContinue)
	{m_bContinue = bContinue;}
private:
    bool m_bContinue;
};

#endif //__SendCmdThread_h__