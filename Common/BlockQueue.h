#ifndef __BLOCK_QUEUE_H_
#define __BLOCK_QUEUE_H_

#include "OSHeaders.h"
#include "OSMutex.h"
#include "OSCond.h"
#include <list>

template<typename T>
class BlockQueue
{
public:
	BlockQueue(UInt32 inTimeoutInMilSecs = 100)
	:m_uTimeoutInMilSecs(inTimeoutInMilSecs)
	{
	}
	void Set_Timeout(UInt32 inTimeoutInMilSecs)
	{
		m_uTimeoutInMilSecs =  inTimeoutInMilSecs;
	}
	virtual ~BlockQueue() 
	{
		m_list.clear();
	}

	int EnQueue(T data)
	{		
		{
            OSMutexLocker theLocker(&fMutex);
            m_list.push_back(data);
        }
        fCond.Signal();
	}

	T DeQueue()
    {
        OSMutexLocker theLocker(&fMutex);		
        T data = m_list.front();
        m_list.pop_front();
        return data;
    }
	
	T DeQueueBlocking()
    {
        OSMutexLocker theLocker(&fMutex);
        if (m_list.empty()) 
            fCond.Wait(&fMutex, m_uTimeoutInMilSecs);

        T data = m_list.front();
        m_list.pop_front();
        return data;
    }

private:
    std::list<T> m_list;
    OSCond fCond;
    OSMutex fMutex;
	int m_uTimeoutInMilSecs;
};

#endif // __BLOCK_QUEUE_H_









