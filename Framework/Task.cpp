#include "Task.h"
#include "OS.h"
#include "log.h"

OSMutexRW TaskThreadPool::m_sMutexRW;

Task::Task() 
: m_taskQueueElem()
, m_timer(this,5,5)
{
    m_taskQueueElem.SetEnclosingObject(this);
}

void Task::Signal()
{
    if (TaskThreadPool::m_sNumTaskThreads > 0)
    {
        LOG_DEBUG("TaskName=%s,Is Added To Thread",m_taskName);                   
        TaskThreadPool::GetTaskThread()->m_taskQueue.EnQueue(&m_taskQueueElem);
    }else{
        LOG_DEBUG("TaskName=%s,But NO TaskThreads To Add!", m_taskName);
    }
}

void Task::SetTaskName(char* name)
{
    if (name == NULL)
        return;
    ::strncpy(m_taskName,name,sizeof(m_taskName));
    m_taskName[sizeof(m_taskName) -1] = 0; //terminate in case it is longer than m_taskName.
}

void TaskThread::Entry()
{
    Task* theTask = NULL;

    while(true) 
    {
        theTask = FetchTask();
        if (theTask == NULL)
            return;                    

        SInt64 ret = 0;
        LOG_DEBUG("TaskThread run TaskName=%s", theTask->m_taskName);
        ret = theTask->Run();

        if (ret < 0){}
        else if (ret == 0){}
        else{}        
    }
}

Task* TaskThread::FetchTask()
{
    while (true)
    {
        OSQueueElem* theElem = m_taskQueue.DeQueueBlocking(this, 0);
        if (theElem != NULL)
        {           
            return (Task*)theElem->GetEnclosingObject();
        }

        // If we are supposed to stop, return NULL, which signals the caller to stop
        if (OSThread::GetCurrent()->IsStopRequested())
            return NULL;
    }   
}

TaskThread** TaskThreadPool::m_sTaskThreadArray = NULL;
UInt32 TaskThreadPool::m_sNumTaskThreads = 0;

Bool16 TaskThreadPool::AddThreads(UInt32 numToAdd)
{
    Assert(m_sTaskThreadArray == NULL);
    m_sTaskThreadArray = new TaskThread * [numToAdd];

    for (UInt32 x = 0; x < numToAdd; x++)
    {
        m_sTaskThreadArray[x] = new TaskThread();
        m_sTaskThreadArray[x]->Start();
        LOG_DEBUG("TaskThreadPool::AddThreads m_sTaskThreadArray[%u]",x); 
    }
    m_sNumTaskThreads = numToAdd; 
    return true;
}

void TaskThreadPool::RemoveThreads()
{
    //Tell all the threads to stop
    for (UInt32 x = 0; x < m_sNumTaskThreads; x++)
        m_sTaskThreadArray[x]->SendStopRequest();

    //Because any (or all) threads may be blocked on the queue, cycle through
    //all the threads, signalling each one
    for(UInt32 y = 0; y < m_sNumTaskThreads; y++)
        m_sTaskThreadArray[y]->m_taskQueue.GetCond()->Signal();

    //Ok, now wait for the selected threads to terminate, deleting them and removing
    //them from the queue.
    for (UInt32 z = 0; z < m_sNumTaskThreads; z++)
        delete m_sTaskThreadArray[z];

    m_sNumTaskThreads = 0;
}

TaskThread* TaskThreadPool::GetTaskThread()
{
    static int last_thread = - 1;
    int index = (last_thread + 1) % m_sNumTaskThreads;
    TaskThread *pThread = m_sTaskThreadArray[index];
    last_thread = index;
    return pThread;
}