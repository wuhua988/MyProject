#include "OS.h"
#include "SocketUtils.h"
#include "redisClient/RedisClient.h"
#include "redisClient/RedisPoolCluster.h"
#include "log.h"

void InitPb();
int main(int argc, char * argv[]) 
{
    //Initialize utility classes
	log_init(LL_DEBUG, "log", "./");
	
	RedisPoolCluster rpl;
	rpl.AddPool("127.0.0.1:6379",100);
	RedisClient cli(&rpl);
	cli.set("RedisTest","TestOK");

	string value;
	if(cli.get("RedisTest",value))
	{
		LOG_DEBUG("value:%s",value.c_str());
	}else{
		LOG_DEBUG("Error");
	}
    while(1)
    {
        sleep(1);
    }
}
