#ifndef __Singleton_h__
#define __Singleton_h__
#include "OSMutex.h"

template <class TYPE>
class Singleton
{
    public:
        static TYPE * instance(void)
        {
            if(m_singleton == NULL)
            {
				static OSMutex mutex;
				mutex.Lock();
				if(m_singleton == NULL)
                    m_singleton = new Singleton;
				mutex.Unlock();
            }
            return &m_singleton->m_instance;
        }
    protected:
        Singleton();
        TYPE m_instance;
        static Singleton<TYPE> *m_singleton;

};


template<class TYPE>
Singleton<TYPE>* Singleton<TYPE>::m_singleton = NULL;

template<class TYPE>
Singleton<TYPE>::Singleton()
{

}


#endif 