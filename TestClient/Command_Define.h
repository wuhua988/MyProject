#ifndef __Command_Define_h__
#define __Command_Define_h__
#include "OSHeaders.h"

namespace cmd {
    namespace ums {
        const UInt16 AuthUserRequest = 0x4001;			// �û���֤
        const UInt16 QuitUserRequest = 0x4002;			// �û��˳�
        const UInt16 GetFriendListRequest = 0x4003;		// ȡ�����б�
        const UInt16 GetUserInfoRequest = 0x4004;		// ȡ�û���Ϣ
        const UInt16 GetFriendInfoRequest = 0x4005;		// ȡ������Ϣ
        const UInt16 GetGroupListRequest = 0x4006;		// ȡȺ�б�
        const UInt16 CreateGroupRequest = 0x4007;		// ����Ⱥ
        const UInt16 JoinGroupRequest = 0x4008;			// ����Ⱥ
        const UInt16 ExitGroupRequest = 0x4009;			// �˳�Ⱥ
        const UInt16 GetGroupUserListRequest = 0x400a;	// ȡȺ��Ա�б�
        const UInt16 SearchGroupUserListRequest = 0x400b;// ����Ⱥ��Ա
        const UInt16 AddFriendRequest = 0x400c;			// ��Ӻ���
        const UInt16 GetFriendVerifyInfoRequest = 0x400d;// ��ȡ������֤��Ϣ
        const UInt16 DeleteFriendRequest = 0x400e;		// ɾ������
        const UInt16 UpdateFriendBranchRequest = 0x400f;// �޸ĺ������ڷ���
        const UInt16 AddBranchRequest = 0x4010;			// ��Ӻ��ѷ���
        const UInt16 DeleteBranchRequest = 0x4011;		// ɾ�����ѷ���
        const UInt16 UpdateBranchInfoRequest = 0x4012;	// �޸ĺ��ѷ�����Ϣ
        const UInt16 KickoutGroupUserRequest = 0x4013;	// ���û���Ⱥ
        const UInt16 SetGroupManagerRequest = 0x4014;	// ����Ⱥ����Ա
        const UInt16 UpdateFriendRemarkRequest = 0x4015;// �޸ĺ��ѱ�ע
        const UInt16 GetBranchListRequest = 0x4016;		// ȡ���ѷ����б�
        const UInt16 SetFriendOptionRequest = 0x4017;	// ���ú���ѡ��
        const UInt16 DeleteGroupRequest = 0x4018;		// ��ɢȺ
        const UInt16 UpdateGroupUserCardRequest = 0x4019; // ����Ⱥ�û���Ϣ
        const UInt16 UpdateGroupInfoRequest = 0x401a;	// �޸�Ⱥ��Ϣ
        const UInt16 UpdateGroupOptionRequest = 0x401b;	// �޸�Ⱥѡ��
        const UInt16 UpdateGroupNoticeRequest = 0x401c;	// �޸�Ⱥ����
        const UInt16 GetGroupInfoRequest = 0x401d;      // ȡȺ��Ϣ
        const UInt16 UpdateUserInfoRequest = 0x401e;    // �޸ĸ�������
        const UInt16 UpdateUserMoodRequest = 0x401f;	// �޸ĸ���ǩ��
        const UInt16 UpdateUserNickNameRequest = 0x4020;// �޸��ǳ�
        const UInt16 UpdateAvatarRequest = 0x4021;      // �޸�ͷ��
        const UInt16 GetGroupUserInfoRequest = 0x4022;  // ȡȺ��Ա������Ϣ
        const UInt16 GetFriendBaseInfoRequest = 0x4023; // ȡ���ѻ�����Ϣ
		const UInt16 GetGroupOptionRequest = 0x4024;	// ��ȡȺѡ��
		const UInt16 GetGroupUserCardRequest = 0x4025; // ȡȺ�ɆT��Ƭ
		const UInt16 SetGroupMsgModeRequest = 0x4026;	 // ����Ⱥ��Ϣ��ʾģʽ
		const UInt16 SetGroupTalkModeRequest = 0x4027; // ����Ⱥ����ģʽ
		const UInt16 GetGroupManagerListRequest = 0x4028; // ȡȺ����Ա�б�����
    }

// Terminal
    namespace terminal {
        const UInt16 Login = 0x4101;                    // ��¼
        const UInt16 Keepalive = 0x4102;                // ����
        const UInt16 Logout = 0x4103;                   // ע��
        const UInt16 KickUserOffline = 0x4104;          // ���û�����
        const UInt16 GetFriendList = 0x4105;            // ȡ�����б�
        const UInt16 GetGroupList = 0x4106;             // ȡȺ�б�
        const UInt16 AddFriend = 0x4107;                // ��Ӻ���
        const UInt16 DeleteFriend = 0x4108;             // ɾ������
        const UInt16 ChangeFriendBranch = 0x4109;       // �޸ĺ������ڷ���
        const UInt16 GetUserInfo = 0x410a;              // ȡ�û���ϸ����
        const UInt16 UpdateFriendRemark = 0x410b;       // �޸ĺ��ѱ�ע
        const UInt16 GetFriendBasicInfo = 0x410c;       // ȡ���ѻ�����Ϣ
        const UInt16 GetFriendInfo = 0x410d;            // ȡ������ϸ����
        const UInt16 DeleteFriendNotify = 0x410e;       // ������ɾ��֪ͨ
		const UInt16 AddFriendVerify = 0x410f;		  // ������֤
        const UInt16 AddBranch = 0x4111;                // ��Ӻ��ѷ���
        const UInt16 DeleteBranch = 0x4112;             // ɾ�����ѷ���
        const UInt16 UpdateBranch = 0x4113;             // �޸ĺ��ѷ�������
        const UInt16 ChangeOnlineState = 0x4114;        // �ı�����״̬
        const UInt16 FriendOnlineStateNotify = 0x4115;  // ����״̬֪ͨ
        const UInt16 FriendsOnlineStateNotify = 0x4116; // ��������״̬֪ͨ
        const UInt16 UpdateUserInfo = 0x4117;           // �޸ĸ�������
        const UInt16 UpdateUserMood = 0x4118;           // �޸ĸ���ǩ��
        const UInt16 UpdateUserNickname = 0x4119;       // �޸��ǳ�
        const UInt16 UpdateUserAvatar = 0x411a;         // �޸�ͷ��
		const UInt16 GetBranchList = 0x411b;			  // ��ȡ�����б�
        const UInt16 JoinGroup = 0x4121;                // ����Ⱥ
        const UInt16 ExitGroup = 0x4122;                // �˳�Ⱥ
        const UInt16 GetGroupUserList = 0x4123;         // ȡȺ��Ա�б�
        const UInt16 KickoutGroupUser = 0x4124;         // �߳�ĳȺ��Ա
        const UInt16 SetGroupManager = 0x4125;          // ���ù���Ա
        const UInt16 CreateGroup = 0x4126;              // ����Ⱥ
        const UInt16 DeleteGroup = 0x4127;              // ��ɢȺ
        const UInt16 UpdateGroupRemark = 0x4128;        // �޸�Ⱥ��ע
        const UInt16 UpdateGroupNotice = 0x4129;        // �޸�Ⱥ����
        const UInt16 UpdateGroupNoticeNotify = 0x412a;  // ����Ⱥ����֪ͨ
        const UInt16 GetGroupInfo = 0x412b;             // ȡȺ��Ϣ
        const UInt16 UpdateGroupInfo = 0x412c;          // �޸�Ⱥ��Ϣ
        const UInt16 GetGroupUserCard = 0x412d;         // ȡȺ��Ƭ
        const UInt16 UpdateGroupUserCard = 0x412e;      // �޸�Ⱥ��Ƭ
        const UInt16 UpdateGroupOption = 0x412f;        // �޸�Ⱥ����
        const UInt16 GetGroupUserInfo = 0x4131;		  // ȡȺ��Ա������Ϣ
		const UInt16 Redirect = 0x4141;				  // �ض���
		const UInt16 AddFriendNotify = 0x4142;		  // ��Ӻ��ѽ��֪ͨ
		const UInt16 AddFriendConfirm = 0x4143; // ��Ӻ�����֤ȷ��(�ɱ���ӷ�����TAS)
		const UInt16 SetGroupMsgMode = 0x4144;  // ����Ⱥ��Ϣ��ʾģʽ
		const UInt16 SetGroupTalkMode = 0x4145; // ����Ⱥ����ģʽ
		const UInt16 SetFriendOption = 0x4146;  // ���ú���ѡ��
		const UInt16 GetFriendVerifyInfo = 0x4147; // ��ȡ������֤��Ϣ
		const UInt16 GetGroupOption = 0x4148;		 // ��ȡȺѡ��
		const UInt16 GetUserOnlineState = 0x4149;	 // ��ȡ�û�����״̬��Ϣ
		const UInt16 GetGroupManagerList = 0x414a; // ȡȺ����Ա�б�����
		const UInt16 JoinGroupVerify = 0x414b;	 // ��Ⱥ��֤
		const UInt16 JoinGroupConfirm = 0x414c;	 // ��Ⱥ��֤ȷ��
		const UInt16 JoinGroupNotify = 0x414d;	 // ��Ⱥ֪ͨ
		const UInt16 DeleteHistoryMessage = 0x414e; // ɾ����ʷ��¼
	}
    
    // Presence
    namespace presence {
        const UInt16 ChangeOnlineState = 0x4301;	   /* �ı�����״̬ */
        const UInt16 ChangeOnlineStateNotify = 0x4302; /* ��������״̬֪ͨ */
        const UInt16 GetOnlineState = 0x4303;		   /* ��ȡ��������״̬��Ϣ */
        const UInt16 MessageTransfer = 0x4308;			   /* �ı���ѹ�ϵ */
		const UInt16 RemoteLoginNotify = 0x4310;	   /* �û���ص�¼֪ͨ */
    }
}

#endif