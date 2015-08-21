#include "TestCase.h"
#include "SharedPtr.h"
#include "CommonDef.h"
#include "TestCliFramework.h"
#include "TestCaseCtx.h"
#include "UserAuthCommand.h"
#include "GetUserInfoCommand.h"
#include "GetFriendInfoCommand.h"
#include "GetFriendListCommand.h"
#include "QuitUserCommand.h"
#include "UpdateUserInfoCommand.h"
#include "ChgUserInfoCommand.h"
#include "GetUserStateCommand.h"
UInt64 interval = 100;
UInt64 user_id = 51;
void TestUserAuth(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
		{
			return;
		}
		//if(!(reqNum % 100))
			OSThread::Sleep(interval);
	    SharedPtr<UserAuthCommand> pObj = new UserAuthCommand(gTestCliFw::instance());
	    pObj->SetParam("ttman1"                                            // 登录帐号 快速登录时为选填项，其余为必填项
	                  ,"14e90e8f77db463fbeed2ed155b68f0f"  /*wang138966*/  // 密码 快速登录时为选填项，其余为必填项
	                  ,6                                                   // 终端类型 - web-1；安卓手机-2；IOS手机-3；安卓pad-4；IOSpad-5；PC客户端-6
	                  ,"94-DE-80-E2-BC-4E"                                 // MAC地址
	                  ,0                                                   // 登录账号类型 账号登录[0]/TT号登录[1]/手机登录[2]/邮箱登录[3]/靓号登录[4]  、token快速登录[5]
	                  ,"196.168.12.125"                                    //登录用户的ip地址 
	                  ,100                                                 //用户ID 快速登录时为必填项，其他登录时为非必填项
	                  ,"Hello");                                           //用户token 快速登录时为必填项，其他登录时为非必填项
	    if(pObj->ScheduleCache(pObj,UMSServerID))
	        pObj->SendRequest();
	}
}

void TestGetUserInfo(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<GetUserInfoCommand> pObj = new GetUserInfoCommand(gTestCliFw::instance());
	    pObj->SetParam(user_id);                                              //user id
	    if(pObj->ScheduleCache(pObj,UMSServerID))
	        pObj->SendRequest();
	}
}

void TestGetFriendInfo(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<GetFriendInfoCommand> pObj = new GetFriendInfoCommand(gTestCliFw::instance());
	    pObj->SetParam(user_id                                                 //user id
		              ,600);                                               //friend id
	    if(pObj->ScheduleCache(pObj,UMSServerID))
	        pObj->SendRequest();
	}
}

void TestGetFriendList(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<GetFriendListCommand> pObj = new GetFriendListCommand(gTestCliFw::instance());
	    pObj->SetParam(1                                                 //version id                  
		              ,user_id);                                             //user id
	    if(pObj->ScheduleCache(pObj,UMSServerID))
	        pObj->SendRequest();
	}
}

void TestQuitUser(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<QuitUserCommand> pObj = new QuitUserCommand(gTestCliFw::instance());
	    pObj->SetParam(user_id,                                            //user id
		              1,                                               //terminal_type
					  "123456tocken");                                 //token
	    if(pObj->ScheduleCache(pObj,UMSServerID)) 
	        pObj->SendRequest();
	}
}

void TestUpdateUserInfo(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<UpdateUserInfoCommand> pObj = new UpdateUserInfoCommand(gTestCliFw::instance());
		pObj->SetParam(user_id,      // 用户ID
                  "nick",	     // 昵称};
                  1,		     // 性别
                  "15012564152", // 手机
                  "19980515",	 // 生日
                  "www@163.com", // Email
                  18,		     // 年龄
                  6,             // 星座
                  "fighting!",	 // 个性签名
                  "girls",	     // 喜欢
                  "beijing",	 // 城市 - 所在地
                  "good man",    // 个人说明*
                  "realname");   // 真实姓名*
	    if(pObj->ScheduleCache(pObj,UMSServerID))
	        pObj->SendRequest();
	}
}

void TestChgUserInfo(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<ChgUserInfoCommand> pObj = new ChgUserInfoCommand(gTestCliFw::instance());
		pObj->SetParam(user_id,                      //用户ID          
	              "192.168.12.256:8856",         //接入标示          
	              "192.168.12.236:4568",         //转发标示         
	              true,                          //是否有摄像头
	              1,                             //终端类型
	              tt_presence::USER_STATE_ONLINE,//用户状态
	              tt_presence::MSG_TYPE_STATE ,  //要更改消息的类型
	              "test_message");               //消息的内容                               
		if(pObj->ScheduleCache(pObj,PresenceServerID))
	        pObj->SendRequest();
	}
}

void TestGetUserState(const std::vector<std::string>& args)
{
	int reqNum = atoi(args[1].c_str());
	for(int i = 0; i < reqNum; i++)
	{
		if(!gTestCaseCtxPtr::instance()->m_bContinue)
			return;
		OSThread::Sleep(interval);
	    SharedPtr<GetUserStateCommand> pObj = new GetUserStateCommand(gTestCliFw::instance());
		std::vector<UInt64> userid_vec;
		userid_vec.push_back(100);
		userid_vec.push_back(101);
		userid_vec.push_back(102);
		userid_vec.push_back(103);
		userid_vec.push_back(104);
	    pObj->SetParam(userid_vec);                              
		if(pObj->ScheduleCache(pObj,PresenceServerID))
	        pObj->SendRequest();
	}
}



