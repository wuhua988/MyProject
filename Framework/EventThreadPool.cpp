#include "EventThreadPool.h"
#include "EventWorkThread.h"
#include "Business/BusinessTimerThread.h"
#include "EventMasterThread.h"
#include "OS.h"

EventMasterThread* EventThreadPool::m_pMasterThread = NULL;
std::vector<EventWorkThread*> EventThreadPool::m_threadVec;
std::vector<BusinessTimerThread*> EventThreadPool::m_TimerthreadVec;
void EventThreadPool::AddThreads(UInt32 numToAdd,UInt32 type,Framework* pFW)
{
    assert(pFW != NULL);
    if(m_pMasterThread == NULL)
    {
        m_pMasterThread = new EventMasterThread(pFW);
        m_pMasterThread->Init();
		m_pMasterThread->Start();
    }
	
	for(UInt32 i = 0; i < numToAdd; i++)
    {
        EventBaseThread* pThread;
		if(type == Type_Work)
		{
			pThread = new EventWorkThread(pFW);
			assert(pThread);
			pThread->GetEvLoop()->SetTimerMgrExclusion(false);
		}
		else
		{
			pThread = new BusinessTimerThread(pFW);
			assert(pThread);			
		}
        pThread->Init();
	   	pThread->Start();
		if(type == Type_Work)
	   	    m_threadVec.push_back((EventWorkThread*)pThread);
		else 
			m_TimerthreadVec.push_back((BusinessTimerThread*)pThread);
    }   
}

void EventThreadPool::RemoveThreads()
{
    
}

EventWorkThread* EventThreadPool::GetWorkThread(UInt32 index)
{
    if(index > m_threadVec.size() || index < 0)
		return NULL;
	return m_threadVec[index];
}

BusinessTimerThread* EventThreadPool::GetTimerThread(UInt32 index)
{
    if(index > m_TimerthreadVec.size() || index < 0)
		return NULL;
	return m_TimerthreadVec[index];
}

EventWorkThread* EventThreadPool::GetRandomWorkThread()
{
	/*SInt64 theMicroseconds = OS::Microseconds();
    ::srand((unsigned int)theMicroseconds);
    UInt32 index = rand() % m_threadVec.size();
	if(index < 0 || index > m_threadVec.size())
		return NULL;*/
	//得[a,b)的随机整数，使用(rand() % (b-a))+ a;
    //得[a,b]的随机整数，使用(rand() % (b-a+1))+ a;
    //得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1;
	if(m_threadVec.size() == 0)
	{
		LOG_ERROR("The WorkThread Number Is 0.");
		return NULL;
	}
	static int last_thread = - 1;
    int index = (last_thread + 1) % m_threadVec.size();
    EventWorkThread *pThread = m_threadVec[index];
    last_thread = index;
	return pThread;
}

BusinessTimerThread* EventThreadPool::GetRandomTimerThread()
{
	if(m_TimerthreadVec.size() == 0)
	{
		LOG_ERROR("The TimerThread Number Is 0.");
		return NULL;
	}
	static int last_thread = - 1;
    int index = (last_thread + 1) % m_TimerthreadVec.size();
    BusinessTimerThread *pThread = m_TimerthreadVec[index];
    last_thread = index;
	return pThread;
}

EventMasterThread* EventThreadPool::GetMasterThread()
{
    return m_pMasterThread;
}