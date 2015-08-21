#ifndef __BusinessTimer_h__
#define __BusinessTimer_h__
#include "../Timer.h"
#include "log.h"

//class ClientProtocolCommand;
template<typename T>
class BusinessTimer : public Timer
{
public:
    BusinessTimer(T* pObj,const Float64 dExpire,const Float64 dInterval = 0);
    virtual void Callback(const UInt32 revents);
private:
    T* m_pObj;
	//UInt32 m_seq;
	//UInt32 m_connID;
	//UInt32 m_serverID;
};

template<typename T>
BusinessTimer<T>::BusinessTimer(T* pObj,const Float64 dExpire,const Float64 dInterval)
:Timer(dExpire,dInterval)
{
    assert(pObj);
    m_pObj = pObj;
}

template<typename T>
void BusinessTimer<T>::Callback(const UInt32 revents)
{
	assert(m_pObj != NULL);	
	//LOG_ERROR("timerElem->Value:%ld,m_Expire:%f",m_timerElem.GetValue(),m_Expire);
    m_pObj->OnTimer();
}
#endif //__BusinessTimer_h__