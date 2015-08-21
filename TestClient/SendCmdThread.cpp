#include "SendCmdThread.h"
#include "TestCaseCtx.h"
#include "ExtendString.h"
#include "config.h"

SendCmdThread::SendCmdThread()
:m_bContinue(false)
{
	
}

SendCmdThread::~SendCmdThread()
{
	
}

void SendCmdThread::Entry()
{
	while(1)
	{
		while(m_bContinue && gTestCaseCtxPtr::instance()->m_bContinue)
		{
			std::vector<std::string> curArg = gTestCaseCtxPtr::instance()->GetCurArg();
			if(curArg.size() == 0)
			{
				m_bContinue = false;
				break;
			}
			gTestCaseCtxPtr::instance()->execTestCmd(curArg);
			std::string name = curArg[2];
	        ExtendString::lower(name);
			if(name == "no")
				m_bContinue = false;
		}
		
		gTestCaseCtxPtr::instance()->m_FinishThreadNum++;
		if(gTestCaseCtxPtr::instance()->m_FinishThreadNum.value() == gConfigPtr::instance()->m_cmdThreadNum)
			printf("all request was sent out !!!");
		
		OSThread::Sleep(1000);//one second
	}
}
