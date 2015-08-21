#ifndef _MsgHeader_h_
#define _MsgHeader_h_
#include <arpa/inet.h>
#include <string.h>
#include "OSHeaders.h"
const UInt32 MAX_SEND_BUF_SIZE = 1024 * 1024 * 2; 
const UInt32 MAX_RECV_BUF_SIZE = 1024 * 128;    
class MsgHeader
{
public:    
    MsgHeader()
	{
		mark[0] = 'T';
		mark[1] = 'M';
	}
    enum{
        TMCP_HEAD_LENGTH = 24 ///<消息头的长度
    };
    enum{
        TMCP_Mod_Request = 1,
        TMCP_Mod_Response = 2,
        TMCP_Mod_Notify = 9,
    };

    enum{
        TMCP_Content_json = 1,
        TMCP_Content_tlv  = 2,
    };   
    inline UInt8 Version() const
    {
        return version;
    }

	inline void SetVersion(UInt8 val) 
    {
        version = val;
    }
	
    inline UInt8 Crypt() const
    {
        return crypt;
    }
	
	inline void SetCrypt(UInt8 val) 
    {
        crypt = val;
    }
	
    inline UInt32 Length() const
    {
        return length;
    }
	
	inline void SetLength(UInt32 val)
	{
		length = val;
	}
	
    inline UInt8 Mode() const
    {
        return mode;
    }
	
	inline void SetMode(UInt8 val)
    {
        mode = val; 
    }
	
    inline UInt8 ContentType() const
    {
        return content_type;
    }
	
	inline void SetContentType(UInt8 val)
    {
        content_type = val;
    }
	
    inline UInt16 Command() const
    {
        return command;
    }
	
	inline void SetCommand(UInt16 val)
    {
        command = val;
    }
	
    inline UInt16 ApplicationId() const
    {
        return app_id;
    }
	
	inline void SetApplicationId(UInt16 val)
    {
        app_id = val;
    }
	
    inline UInt16 Error() const
    {
        return error;
    }
	
	inline void SetError(UInt16 val)
    {
        error = val;
    }
	
    inline UInt32 Sequence() const
    {
        return sequence;
    }
	
    inline void SetSequence(UInt32 val)
    {
        sequence = val;
    }
	
	//将消息头打包成网络字节序的数据包返回, 
	inline char* ToNet();
	inline bool FromNet(char const* pStrHead);    
    
private:	
    char   mark[2];
    UInt8  version;
    UInt8  crypt;
    UInt32 length;
    UInt8  mode;
    UInt8  content_type;
    UInt16 command;
    UInt16 app_id;
    UInt16 error;
    UInt32 sequence;
    UInt16 ext_flags ;
    UInt16 res ;
	
	char m_strHead[TMCP_HEAD_LENGTH]; 
};

inline char* MsgHeader::ToNet()
{
    UInt32 byte4;
    UInt16 byte2;
    UInt8  pos = 0;

    m_strHead[pos] = 'T';              pos++;
    m_strHead[pos] = 'M';              pos++;
	m_strHead[pos] = version;          pos++;
    m_strHead[pos] = crypt;            pos++;
	byte4 = OS_HTONL(length);          memcpy(m_strHead + pos, &byte4, 4); pos += 4;
	m_strHead[pos] = mode;             pos++;
	m_strHead[pos] = content_type;     pos++;	
    byte2 = OS_HTONS(command);         memcpy(m_strHead + pos, &byte2, 2); pos += 2;
	byte2 = OS_HTONS(app_id);          memcpy(m_strHead + pos, &byte2, 2); pos += 2;
	byte2 = OS_HTONS(error);           memcpy(m_strHead + pos, &byte2, 2); pos += 2;
    byte4 = OS_HTONL(sequence);        memcpy(m_strHead + pos, &byte4, 4); pos += 4;
    byte2 = OS_HTONS(ext_flags);       memcpy(m_strHead + pos, &byte2, 2); pos += 2;
	byte2 = OS_HTONS(res);             memcpy(m_strHead + pos, &byte2, 2); pos += 2;
    return m_strHead;
}

inline bool MsgHeader::FromNet(char const* pStrHead)
{
    UInt32 byte4;
    UInt16 byte2;
    UInt8  pos = 0;
	
	mark[0] = pStrHead[pos];                                        pos++;
	mark[1] = pStrHead[pos];                                        pos++;
    version = pStrHead[pos];                                        pos++;    //UInt8
    crypt = pStrHead[pos];                                          pos++;    //UInt8
	memcpy(&byte4, pStrHead + pos, 4); length = OS_NTOHL(byte4);    pos += 4; //UInt32 
    mode = pStrHead[pos];                                           pos++;    //UInt8
    content_type = pStrHead[pos];                                   pos++;    //UInt8
	memcpy(&byte2, pStrHead + pos, 2); command = OS_NTOHS(byte2);   pos += 2; //UInt16
	memcpy(&byte2, pStrHead + pos, 2); app_id = OS_NTOHS(byte2);    pos += 2; //UInt16
    memcpy(&byte2, pStrHead + pos, 2); error = OS_NTOHS(byte2);     pos += 2; //UInt16
    memcpy(&byte4, pStrHead + pos, 4); sequence = OS_NTOHL(byte4);  pos += 4; //UInt32 
	memcpy(&byte2, pStrHead + pos, 2); ext_flags = OS_NTOHS(byte2); pos += 2; //UInt16
	memcpy(&byte2, pStrHead + pos, 2); res = OS_NTOHS(byte2);       pos+=2;   //UInt16
	
	if (mark[0] != 'T' || mark[1] != 'M' )
        return false;
    return true;
}

#endif //_MsgHeader_h_

