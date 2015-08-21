#ifndef __TestCaseCtx_h__
#define __TestCaseCtx_h__
#include <vector>
#include <map>
#include <string>
#include "OSMutex.h"
#include "Singleton.h"
#include "AtomicCounter.h"

class Framework;
typedef void (*CmdProc)(const std::vector<std::string>& args);

struct TestCommand
{
	std::string str_cmdName;
    CmdProc proc;
};

class TestCaseCtx
{
public:
   	TestCaseCtx();
	~TestCaseCtx();
	
	void execTestCmd(const std::vector<std::string>& args);
	void regCmd(const TestCommand& cmd);
	void regCmd(const std::string& str_cmdName,CmdProc proc);
	static void regCmd();
	void SetCurArg(const std::vector<std::string>& curArg);
	std::vector<std::string>& GetCurArg();
	
	bool m_bContinue;
	static AtomicCounter<UInt32> m_FinishThreadNum;
	static AtomicCounter<UInt32> m_StartReqNum;
	static AtomicCounter<UInt32> m_ResponseNum;
	static AtomicCounter<UInt32> m_timeoutNum;
	static AtomicCounter<UInt32> m_createCmdObjs;
	static AtomicCounter<UInt32> m_deleteCmdObjs;

	
	void SetTotle(SInt64 value);
	SInt64 GetTotle();
	void SetMinTime(SInt64 value);
	SInt64 GetMinTime();
	void SetMaxTime(SInt64 value);
	SInt64 GetMaxTime();
	void SetStartTime(SInt64 value);
	SInt64 GetStartTime();
	void SetEndTime(SInt64 value);
	SInt64 GetEndTime();
    void Reset();
private:
    std::map<std::string,TestCommand> m_name2ctxMap;
	std::vector<std::string> m_curArg;
	OSMutex m_curArgMutex;
	OSMutex m_totle_mutex;
	OSMutex m_min_mutex;
	OSMutex m_max_mutex;
	OSMutex m_start_mutex;
	OSMutex m_end_mutex;
	SInt64 m_startTime;
	SInt64 m_endTime;
	SInt64 m_TotleTime;
	SInt64 m_minTime;
	SInt64 m_maxTime; 
};


inline void TestCaseCtx::SetTotle(SInt64 value)
{
	OSMutexLocker lock(&m_totle_mutex);
	m_TotleTime += value;
}

inline SInt64 TestCaseCtx::GetTotle()
{
	OSMutexLocker lock(&m_totle_mutex);
	return m_TotleTime;
}

inline void TestCaseCtx::SetMinTime(SInt64 value)
{
	OSMutexLocker lock(&m_min_mutex);
	if(m_minTime > value)
	    m_minTime = value;
}

inline SInt64 TestCaseCtx::GetMinTime()
{
	OSMutexLocker lock(&m_min_mutex);
	return m_minTime;
}

inline void TestCaseCtx::SetMaxTime(SInt64 value)
{
	OSMutexLocker lock(&m_max_mutex);
	if(m_maxTime < value)
		m_maxTime = value;
}

inline SInt64 TestCaseCtx::GetMaxTime()
{
	OSMutexLocker lock(&m_max_mutex);
	return m_maxTime;
}

inline void TestCaseCtx::SetStartTime(SInt64 value)
{
	OSMutexLocker lock(&m_start_mutex);
	if(m_startTime > value)
		m_startTime = value;
}

inline SInt64 TestCaseCtx::GetStartTime()
{
	OSMutexLocker lock(&m_start_mutex);
	return m_startTime;
}

inline void TestCaseCtx::SetEndTime(SInt64 value)
{
	OSMutexLocker lock(&m_end_mutex);
	if(m_endTime < value)
		m_endTime = value;
}

inline SInt64 TestCaseCtx::GetEndTime()
{
	OSMutexLocker lock(&m_end_mutex);
	return m_endTime;
}

typedef Singleton<TestCaseCtx> gTestCaseCtxPtr;
#endif //__TestCaseCtx_h__