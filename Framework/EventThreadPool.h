#ifndef __EventThreadPool_h__
#define __EventThreadPool_h__

#include <vector>
#include "OSHeaders.h"

class EventMasterThread;
class EventWorkThread;
class BusinessTimerThread;
class Framework;
class EventThreadPool
{
public:  
    
	enum ThreadType
	{
		Type_Work = 1,
		Type_BusinessTimer
	};
    static void AddThreads(UInt32 numToAdd,UInt32 type,Framework* pFW); 
    static void RemoveThreads();
    static EventWorkThread* GetWorkThread(UInt32 index);
	static BusinessTimerThread* GetTimerThread(UInt32 index);
    static UInt32 GetWorkNumThreads() { return m_threadVec.size(); }
	static UInt32 GetTimerNumThreads() { return m_TimerthreadVec.size(); }
    static EventWorkThread* GetRandomWorkThread();//assign
	static BusinessTimerThread* GetRandomTimerThread();
    static EventMasterThread* GetMasterThread();
private:
    static EventMasterThread* m_pMasterThread;
private:
    static std::vector<EventWorkThread*> m_threadVec;
	static std::vector<BusinessTimerThread*> m_TimerthreadVec;
};

#endif //__EventThreadPool_h__
