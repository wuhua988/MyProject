#ifndef __Command_Define_h__
#define __Command_Define_h__
#include "OSHeaders.h"

namespace cmd {
    namespace ums {
        const UInt16 AuthUserRequest = 0x4001;			// 用户认证
        const UInt16 QuitUserRequest = 0x4002;			// 用户退出
        const UInt16 GetFriendListRequest = 0x4003;		// 取好友列表
        const UInt16 GetUserInfoRequest = 0x4004;		// 取用户信息
        const UInt16 GetFriendInfoRequest = 0x4005;		// 取好友信息
        const UInt16 GetGroupListRequest = 0x4006;		// 取群列表
        const UInt16 CreateGroupRequest = 0x4007;		// 创建群
        const UInt16 JoinGroupRequest = 0x4008;			// 加入群
        const UInt16 ExitGroupRequest = 0x4009;			// 退出群
        const UInt16 GetGroupUserListRequest = 0x400a;	// 取群成员列表
        const UInt16 SearchGroupUserListRequest = 0x400b;// 搜索群成员
        const UInt16 AddFriendRequest = 0x400c;			// 添加好友
        const UInt16 GetFriendVerifyInfoRequest = 0x400d;// 获取好友验证信息
        const UInt16 DeleteFriendRequest = 0x400e;		// 删除好友
        const UInt16 UpdateFriendBranchRequest = 0x400f;// 修改好友所在分组
        const UInt16 AddBranchRequest = 0x4010;			// 添加好友分组
        const UInt16 DeleteBranchRequest = 0x4011;		// 删除好友分组
        const UInt16 UpdateBranchInfoRequest = 0x4012;	// 修改好友分组信息
        const UInt16 KickoutGroupUserRequest = 0x4013;	// 踢用户出群
        const UInt16 SetGroupManagerRequest = 0x4014;	// 设置群管理员
        const UInt16 UpdateFriendRemarkRequest = 0x4015;// 修改好友备注
        const UInt16 GetBranchListRequest = 0x4016;		// 取好友分组列表
        const UInt16 SetFriendOptionRequest = 0x4017;	// 设置好友选项
        const UInt16 DeleteGroupRequest = 0x4018;		// 解散群
        const UInt16 UpdateGroupUserCardRequest = 0x4019; // 设置群用户信息
        const UInt16 UpdateGroupInfoRequest = 0x401a;	// 修改群信息
        const UInt16 UpdateGroupOptionRequest = 0x401b;	// 修改群选项
        const UInt16 UpdateGroupNoticeRequest = 0x401c;	// 修改群公告
        const UInt16 GetGroupInfoRequest = 0x401d;      // 取群信息
        const UInt16 UpdateUserInfoRequest = 0x401e;    // 修改个人资料
        const UInt16 UpdateUserMoodRequest = 0x401f;	// 修改个性签名
        const UInt16 UpdateUserNickNameRequest = 0x4020;// 修改昵称
        const UInt16 UpdateAvatarRequest = 0x4021;      // 修改头像
        const UInt16 GetGroupUserInfoRequest = 0x4022;  // 取群成员基本信息
        const UInt16 GetFriendBaseInfoRequest = 0x4023; // 取好友基本信息
		const UInt16 GetGroupOptionRequest = 0x4024;	// 获取群选项
		const UInt16 GetGroupUserCardRequest = 0x4025; // 取群成員名片
		const UInt16 SetGroupMsgModeRequest = 0x4026;	 // 设置群消息提示模式
		const UInt16 SetGroupTalkModeRequest = 0x4027; // 设置群发言模式
		const UInt16 GetGroupManagerListRequest = 0x4028; // 取群管理员列表请求
    }

// Terminal
    namespace terminal {
        const UInt16 Login = 0x4101;                    // 登录
        const UInt16 Keepalive = 0x4102;                // 保活
        const UInt16 Logout = 0x4103;                   // 注销
        const UInt16 KickUserOffline = 0x4104;          // 踢用户下线
        const UInt16 GetFriendList = 0x4105;            // 取好友列表
        const UInt16 GetGroupList = 0x4106;             // 取群列表
        const UInt16 AddFriend = 0x4107;                // 添加好友
        const UInt16 DeleteFriend = 0x4108;             // 删除好友
        const UInt16 ChangeFriendBranch = 0x4109;       // 修改好友所在分组
        const UInt16 GetUserInfo = 0x410a;              // 取用户详细资料
        const UInt16 UpdateFriendRemark = 0x410b;       // 修改好友备注
        const UInt16 GetFriendBasicInfo = 0x410c;       // 取好友基本信息
        const UInt16 GetFriendInfo = 0x410d;            // 取好友详细资料
        const UInt16 DeleteFriendNotify = 0x410e;       // 被好友删除通知
		const UInt16 AddFriendVerify = 0x410f;		  // 好友验证
        const UInt16 AddBranch = 0x4111;                // 添加好友分组
        const UInt16 DeleteBranch = 0x4112;             // 删除好友分组
        const UInt16 UpdateBranch = 0x4113;             // 修改好友分组名称
        const UInt16 ChangeOnlineState = 0x4114;        // 改变在线状态
        const UInt16 FriendOnlineStateNotify = 0x4115;  // 好友状态通知
        const UInt16 FriendsOnlineStateNotify = 0x4116; // 批量好友状态通知
        const UInt16 UpdateUserInfo = 0x4117;           // 修改个人资料
        const UInt16 UpdateUserMood = 0x4118;           // 修改个性签名
        const UInt16 UpdateUserNickname = 0x4119;       // 修改昵称
        const UInt16 UpdateUserAvatar = 0x411a;         // 修改头像
		const UInt16 GetBranchList = 0x411b;			  // 获取分组列表
        const UInt16 JoinGroup = 0x4121;                // 加入群
        const UInt16 ExitGroup = 0x4122;                // 退出群
        const UInt16 GetGroupUserList = 0x4123;         // 取群成员列表
        const UInt16 KickoutGroupUser = 0x4124;         // 踢出某群成员
        const UInt16 SetGroupManager = 0x4125;          // 设置管理员
        const UInt16 CreateGroup = 0x4126;              // 创建群
        const UInt16 DeleteGroup = 0x4127;              // 解散群
        const UInt16 UpdateGroupRemark = 0x4128;        // 修改群备注
        const UInt16 UpdateGroupNotice = 0x4129;        // 修改群公告
        const UInt16 UpdateGroupNoticeNotify = 0x412a;  // 更新群公告通知
        const UInt16 GetGroupInfo = 0x412b;             // 取群信息
        const UInt16 UpdateGroupInfo = 0x412c;          // 修改群信息
        const UInt16 GetGroupUserCard = 0x412d;         // 取群名片
        const UInt16 UpdateGroupUserCard = 0x412e;      // 修改群名片
        const UInt16 UpdateGroupOption = 0x412f;        // 修改群设置
        const UInt16 GetGroupUserInfo = 0x4131;		  // 取群成员基本信息
		const UInt16 Redirect = 0x4141;				  // 重定向
		const UInt16 AddFriendNotify = 0x4142;		  // 添加好友结果通知
		const UInt16 AddFriendConfirm = 0x4143; // 添加好友验证确认(由被添加方发往TAS)
		const UInt16 SetGroupMsgMode = 0x4144;  // 设置群消息提示模式
		const UInt16 SetGroupTalkMode = 0x4145; // 设置群发言模式
		const UInt16 SetFriendOption = 0x4146;  // 设置好友选项
		const UInt16 GetFriendVerifyInfo = 0x4147; // 获取好友验证信息
		const UInt16 GetGroupOption = 0x4148;		 // 获取群选项
		const UInt16 GetUserOnlineState = 0x4149;	 // 获取用户在线状态信息
		const UInt16 GetGroupManagerList = 0x414a; // 取群管理员列表请求
		const UInt16 JoinGroupVerify = 0x414b;	 // 加群验证
		const UInt16 JoinGroupConfirm = 0x414c;	 // 加群验证确认
		const UInt16 JoinGroupNotify = 0x414d;	 // 加群通知
		const UInt16 DeleteHistoryMessage = 0x414e; // 删除历史记录
	}
    
    // Presence
    namespace presence {
        const UInt16 ChangeOnlineState = 0x4301;	   /* 改变在线状态 */
        const UInt16 ChangeOnlineStateNotify = 0x4302; /* 好友在线状态通知 */
        const UInt16 GetOnlineState = 0x4303;		   /* 获取好友在线状态信息 */
        const UInt16 MessageTransfer = 0x4308;			   /* 改变好友关系 */
		const UInt16 RemoteLoginNotify = 0x4310;	   /* 用户异地登录通知 */
    }
}

#endif