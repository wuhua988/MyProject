#ifndef __ChgUserInfoCommand_h__
#define __ChgUserInfoCommand_h__
#include <string>
#include "CliCmd.h"
#include "protocol/presence.pb.h"
using std::string;

class ChgUserInfoCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    ChgUserInfoCommand();
    ChgUserInfoCommand(Framework* pFramework);
	virtual ~ChgUserInfoCommand();
	
	virtual void OnTimer();
	virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
	void SetParam(UInt64 user_id,                    
	              const  string& acc_ident,                    
	              const  string& trans_ident,                 
	              bool   is_camera,                 
	              UInt32 ttype,
	              tt_presence::UserState user_state,
	              tt_presence::UserInfoMessageType msg_type,
	              const string& msg_content);
private:
    UInt64 m_user_id ;                           //用户ID
	string m_acc_ident;                          //接入标示
	string m_trans_ident;                        //转发标示
	bool   m_is_camera;                          //是否有摄像头
	UInt32 m_ttype;                              //终端类型
	tt_presence::UserState m_user_state;         //用户状态
	tt_presence::UserInfoMessageType m_msg_type; //要更改消息的类型
	string m_msg_content;                        //消息的内容
};

#endif //__ChgUserInfoCommand_h__