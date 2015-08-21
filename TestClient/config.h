#ifndef __config_h__
#define __config_h__
#include <string>
#include "Singleton.h"

struct config
{
	config()
	{
		m_max_fds = 1024;
		m_core_size = 0;
		m_cmdThreadNum = 10;
	}
    std::string m_hostip;
    int         m_hostport;
	int         m_max_fds;
	int         m_core_size;
	UInt32         m_cmdThreadNum;
};

typedef Singleton<config> gConfigPtr;
#endif //__config_h__