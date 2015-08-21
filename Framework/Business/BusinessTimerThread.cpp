#include "BusinessTimerThread.h"

BusinessTimerThread::BusinessTimerThread(Framework* pFw)
: EventBaseThread(pFw)
{
	
}

BusinessTimerThread::~BusinessTimerThread()
{
	
}

void BusinessTimerThread::RegTimer(Timer* pTimer)
{
	GetEvLoop()->RegTimer(pTimer);
}

void BusinessTimerThread::CancelTimeout(Timer* pTimer)
{
	GetEvLoop()->CancelTimeout(pTimer);
}



