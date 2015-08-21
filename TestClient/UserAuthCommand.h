#ifndef __UserAuthCommand_h__
#define __UserAuthCommand_h__
#include <string>
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class UserAuthCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    UserAuthCommand();
    UserAuthCommand(Framework* pFramework);
    ~UserAuthCommand();
    
    virtual void OnTimer();
    virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
    void SetParam(const string& login_name  
                 ,const string& password
                 ,UInt32        terminal
                 ,const string& mac_addr
                 ,UInt32        login_type
                 ,const string& user_ip
                 ,UInt64        user_id
                 ,const string& token);
private:
    string m_login_name;   // 登录帐号 快速登录时为选填项，其余为必填项
    string m_password;     // 密码 快速登录时为选填项，其余为必填项
    UInt32 m_terminal;     // 终端类型 - web-1；安卓手机-2；IOS手机-3；安卓pad-4；IOSpad-5；PC客户端-6
    string m_mac_addr;     // MAC地址
    UInt32 m_login_type;   // 登录账号类型 账号登录[0]/TT号登录[1]/手机登录[2]/邮箱登录[3]/靓号登录[4]  、token快速登录[5]
    string m_user_ip;      //登录用户的ip地址 
    UInt64 m_user_id;      //用户ID 快速登录时为必填项，其他登录时为非必填项
    string m_token ;       //用户token 快速登录时为必填项，其他登录时为非必填项
};

#endif //__UserAuthCommand_h__