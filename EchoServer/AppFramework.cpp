#include "AppFramework.h"
#include "PBReflect.h"
#include "protocol/echo.pb.h"

void AppFramework::OnMessage(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    PBReflect::ErrorCode error;
    SharedPtr<Message> msgPtr = PBReflect::ParsePBMsg(pMsgBlock->get_ptr(),pMsgBlock->length(),pMsgBlock->Header()->Command(),&error);
    SharedPtr<App::EchoRequest> echoPtr = msgPtr.cast<App::EchoRequest>();

    //要调用PB的IsInitialized方法吗？
    if(error == PBReflect::kNoError)
    {
        LOG_DEBUG("Message ConnID:%u",       pMsgBlock->ConnID());
        LOG_DEBUG("Header->Version:%d",      pMsgBlock->Header()->Version());
        LOG_DEBUG("Header->Crypt:%d",        pMsgBlock->Header()->Crypt());
        LOG_DEBUG("Header->Length:%u",       pMsgBlock->Header()->Length());
        LOG_DEBUG("Header->Mode:%d",         pMsgBlock->Header()->Mode());
        LOG_DEBUG("Header->ContentType:%d",  pMsgBlock->Header()->ContentType());
        LOG_DEBUG("Header->Command:%d",      pMsgBlock->Header()->Command());
        LOG_DEBUG("Header->ApplicationId:%d",pMsgBlock->Header()->ApplicationId());
        LOG_DEBUG("Header->Error:%d",        pMsgBlock->Header()->Error());
        LOG_DEBUG("Header->Sequence:%u",     pMsgBlock->Header()->Sequence());

        if(!echoPtr.isNull())
        {
            string name = echoPtr->name();
            string msg = echoPtr->msg();
            LOG_DEBUG("Receive Msg Name:%s,Content:%s",name.c_str(),msg.c_str());
        }
        MsgHeader header = *pMsgBlock->Header();
        header.SetMode(2);
        App::EchoRequest echo_resp;
        echo_resp.set_name(echoPtr->name());
        echo_resp.set_msg(echoPtr->msg());
        string strseri;
        if(echo_resp.SerializeToString(&strseri))
        {			
            MsgBlock* pRespMsgBlock = MsgBlock::PackMsg(header,strseri.c_str(),strseri.length(),0,pMsgBlock->ConnID());
            if(pRespMsgBlock)
            {
                SendMsg(pRespMsgBlock);
            }
        }

    } else {
        LOG_DEBUG("Receive Msg Error:%s",PBReflect::ErrorToStr(error).c_str());
    }
    delete pMsgBlock;
}

int AppFramework::OnConnCreated(ConnInfo* pConnInfo) 
{
    LOG_DEBUG("AppFramework Receive Connection Created Event,ConnID:%u",pConnInfo->GetConnID());
    return 0;
}

int AppFramework::OnFailConnect(ConnInfo* pConnInfo)
{
    return 0;
}

int AppFramework::OnConnClosed(ConnInfo* pConnInfo)
{
    return 0;
}