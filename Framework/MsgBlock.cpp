#include "MsgBlock.h"

MsgBlock::MsgBlock(size_t len,UInt32 uiServerID,UInt32 uiConnID,const MsgHeader& header)
:m_QueueElem()
{
    m_header = header;
    if(len > 0)
        m_buf = new char[len];
    else
        m_buf = NULL;
    m_rdPos = 0;
    m_wrPos = 0; 
    m_len = len;  
    m_connID = uiConnID;
    m_serverID = uiServerID;
    m_QueueElem.SetEnclosingObject(this);
}

MsgBlock* MsgBlock::PackMsg(MsgHeader& header
                           ,char const* strData
                           ,UInt32 iDataLen
                           ,UInt32 uiServerID
                           ,UInt32 uiConnID)
{
    header.SetLength(iDataLen);
    MsgBlock* pBlock = new MsgBlock(MsgHeader::TMCP_HEAD_LENGTH + iDataLen,uiServerID,uiConnID,header);
    memcpy(pBlock->wr_ptr(), header.ToNet(), MsgHeader::TMCP_HEAD_LENGTH);
    pBlock->wr_ptr(MsgHeader::TMCP_HEAD_LENGTH);
    if (strData)
    {
        memcpy(pBlock->wr_ptr(), strData, iDataLen);
        pBlock->wr_ptr(iDataLen);
    }
    return pBlock;
}