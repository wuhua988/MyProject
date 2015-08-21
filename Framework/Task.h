#ifndef __TASK_H__
#define __TASK_H__

#include "OSQueue.h"
#include "OSThread.h"
#include "OSMutexRW.h"
#include "Business/BusinessTimer.h"

class TaskThread;
class Task
{
public:
    Task();
    virtual      ~Task() {}
    virtual      SInt64 Run() = 0;
    void         Signal();
    void         SetTaskName(char* name);
	virtual void OnTimer();	
	void         StartTimer();
	void         StopTimer();
private:
    OSQueueElem         m_taskQueueElem;
	char                m_taskName[48];
	BusinessTimer<Task> m_timer;
    friend class TaskThread;
};

class TaskThread : public OSThread
{
public:
    TaskThread() : OSThread(){}
    virtual ~TaskThread() { this->StopAndWaitForThread(); }
           
private:
    virtual void Entry();
    Task*        FetchTask();
    
    OSQueue_Blocking m_taskQueue;    
    
    friend class Task;
    friend class TaskThreadPool;
};


class TaskThreadPool 
{
public:
    static Bool16      AddThreads(UInt32 numToAdd); 
    static void        RemoveThreads();
	static TaskThread* GetTaskThread();
    static UInt32      GetNumThreads() { return m_sNumTaskThreads; }
    
private:
    static TaskThread** m_sTaskThreadArray;
    static UInt32       m_sNumTaskThreads;    
    static OSMutexRW    m_sMutexRW;
    
    friend class Task;
    friend class TaskThread;
};

#endif
