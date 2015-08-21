#ifndef __UpdateUserInfoCommand_h__
#define __UpdateUserInfoCommand_h__
#include <string>
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class UpdateUserInfoCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    UpdateUserInfoCommand();
    UpdateUserInfoCommand(Framework* pFramework);
	~UpdateUserInfoCommand();
	
	virtual void OnTimer();
	virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
	void SetParam(UInt64        user_id,	
                  const string& nick_name,	
                  UInt32        sex,		
                  const string& mobile,	
                  const string& birthday,	
                  const string& email,		
                  UInt32        age,		
                  UInt32        constellation,
                  const string& sign,		
                  const string& like,	       
                  const string& city,		
                  const string& personal_desc,
                  const string& real_name);
private:
    UInt64 m_user_id;		// 用户ID
    string m_nick_name;		// 昵称};
    UInt32 m_sex;			// 性别
    string m_mobile;		// 手机
    string m_birthday;		// 生日
    string m_email;			// Email
    UInt32 m_age;			// 年龄
    UInt32 m_constellation;	// 星座
    string m_sign;			// 个性签名
    string m_like;	        // 喜欢
    string m_city;			// 城市 - 所在地
    string m_personal_desc;	// 个人说明*
    string m_real_name;		// 真实姓名*
};
#endif //__UserAuthCommand_h__