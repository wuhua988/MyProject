#include "TestCaseCtx.h"
#include "TestCase.h"
#include "log.h"
#include "OS.h"
#include "Business/ProtoCmdBase.h"
#include "ExtendString.h"
AtomicCounter<UInt32> TestCaseCtx::m_FinishThreadNum = AtomicCounter<UInt32>(0);
AtomicCounter<UInt32> TestCaseCtx::m_StartReqNum = AtomicCounter<UInt32>(0);
AtomicCounter<UInt32> TestCaseCtx::m_ResponseNum = AtomicCounter<UInt32>(0);
AtomicCounter<UInt32> TestCaseCtx::m_timeoutNum = AtomicCounter<UInt32>(0);
AtomicCounter<UInt32> TestCaseCtx::m_createCmdObjs = AtomicCounter<UInt32>(0);
AtomicCounter<UInt32> TestCaseCtx::m_deleteCmdObjs = AtomicCounter<UInt32>(0);

TestCaseCtx::TestCaseCtx()
:m_startTime(143987795565999)
,m_endTime(0)
,m_TotleTime(0)
,m_minTime(143987795565999)
,m_maxTime(0)
{
	
}

TestCaseCtx::~TestCaseCtx()
{
	m_name2ctxMap.clear();
}

void TestCaseCtx::execTestCmd(const std::vector<std::string>& args)
{
	if(args.size() == 0)
	{
		LOG_DEBUG("args.size() == 0");
		return ;
	}
	std::string name = args[0];
	ExtendString::lower(name);
	std::map<std::string,TestCommand>::iterator iter = m_name2ctxMap.find(name);
	if(iter != m_name2ctxMap.end())
	{
		SInt64 sTime = OS::Milliseconds();
		gTestCaseCtxPtr::instance()->SetStartTime(sTime);
		iter->second.proc(args);
	}else{
		LOG_DEBUG("Can't Find TestCase,name:%s",args[0].c_str());
	}
}

void TestCaseCtx::regCmd(const TestCommand& cmd)
{
	std::string name = cmd.str_cmdName;
	ExtendString::lower(name);
	std::map<std::string,TestCommand>::iterator iter = m_name2ctxMap.find(name);
	if(iter == m_name2ctxMap.end())
		m_name2ctxMap[name] = cmd;
}

void TestCaseCtx::regCmd(const std::string& str_cmdName,CmdProc proc)
{
    TestCommand cmd;
	cmd.str_cmdName = str_cmdName;
	cmd.proc = proc;
	regCmd(cmd);
}

void TestCaseCtx::regCmd()
{
	gTestCaseCtxPtr::instance()->regCmd("UserAuth",TestUserAuth);	
	gTestCaseCtxPtr::instance()->regCmd("GetUserInfo",TestGetUserInfo);
	gTestCaseCtxPtr::instance()->regCmd("GetFriendInfo",TestGetFriendInfo);
    gTestCaseCtxPtr::instance()->regCmd("GetFriendList",TestGetFriendList);
    gTestCaseCtxPtr::instance()->regCmd("QuitUser",TestQuitUser);
    gTestCaseCtxPtr::instance()->regCmd("UpdateUserInfo",TestUpdateUserInfo);
    gTestCaseCtxPtr::instance()->regCmd("ChgUserInfo",TestChgUserInfo);
    gTestCaseCtxPtr::instance()->regCmd("GetUserState",TestGetUserState);
}

void TestCaseCtx::SetCurArg(const std::vector<std::string>& curArg)
{
	OSMutexLocker lock(&m_curArgMutex);
	m_curArg = curArg;
}

std::vector<std::string>& TestCaseCtx::GetCurArg()
{
	OSMutexLocker lock(&m_curArgMutex);
	return m_curArg;
}

void TestCaseCtx::Reset()
{
	m_startTime = 143987795565999;
    m_endTime   = 0;
    m_TotleTime =0;
    m_minTime   =143987795565999;
    m_maxTime   =0;
	
    TestCaseCtx::m_FinishThreadNum = AtomicCounter<UInt32>(0);
	TestCaseCtx::m_StartReqNum = AtomicCounter<UInt32>(ProtoCmdBase::GetSeqID());
	TestCaseCtx::m_StartReqNum++;
    TestCaseCtx::m_ResponseNum = AtomicCounter<UInt32>(0);
    TestCaseCtx::m_timeoutNum = AtomicCounter<UInt32>(0);
    TestCaseCtx::m_createCmdObjs = AtomicCounter<UInt32>(0);
    TestCaseCtx::m_deleteCmdObjs = AtomicCounter<UInt32>(0);
}
