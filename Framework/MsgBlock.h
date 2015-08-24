#ifndef _MsgBlock_h_
#define _MsgBlock_h_
#include "MsgHeader.h"
#include "OSQueue.h"
class MsgBlock
{
public:
    MsgBlock(size_t len,UInt32 uiServerID,UInt32 uiConnID,const MsgHeader& header);
	
	inline char*       rd_ptr (void) const;
    inline void        rd_ptr (size_t n);
    inline char*       wr_ptr (void) const ;
    inline void        wr_ptr(size_t n) ;
    inline char*       get_ptr (void) const;
    inline size_t      length(void) const ;
    inline size_t      rd_pos(void) const ;
    inline UInt32      ConnID() const;
    inline UInt32      ServerID() const;
    inline MsgHeader*  Header();
    OSQueueElem m_QueueElem;
    
    static MsgBlock* PackMsg(MsgHeader& header
	                        ,char const* strData
							,UInt32 iDataLen
							,UInt32 uiServerID
							,UInt32 uiConnID);
private:
    MsgBlock();
    char*     m_buf;  
    size_t    m_rdPos; //读位置
    size_t    m_wrPos; //写位置
    size_t    m_len;   //m_buf大小
    UInt32    m_connID;
    UInt32    m_serverID;
    MsgHeader m_header;	
};

inline char *MsgBlock::rd_ptr (void) const
{ 
    return m_buf + m_rdPos;
}

inline void MsgBlock::rd_ptr (size_t n)
{
    m_rdPos += n;
}

inline char *MsgBlock::wr_ptr (void) const 
{ 
    return m_buf + m_wrPos;
}

inline void MsgBlock::wr_ptr(size_t n) 
{ 
    m_wrPos += n;
}

inline char* MsgBlock::get_ptr (void) const
{
	return m_buf;
}
inline size_t MsgBlock::length(void) const 
{
	return m_len;
    //return m_wrPos > m_rdPos ? m_wrPos - m_rdPos : 0;
}

inline size_t MsgBlock::rd_pos(void) const
{
    return m_rdPos;
}

inline UInt32 MsgBlock::ConnID() const
{
    return m_connID;
}

inline UInt32 MsgBlock::ServerID() const
{
    return m_serverID;
}

inline MsgHeader* MsgBlock::Header()
{
	return &m_header;
}
#endif //_MsgBlock_h_