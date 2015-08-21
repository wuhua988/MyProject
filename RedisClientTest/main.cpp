#include "OS.h"
#include "SocketUtils.h"
#include "redisClient/RedisClient.h"
#include "redisClient/RedisPoolCluster.h"
#include "log.h"

void InitPb();
int main(int argc, char * argv[]) 
{
    //Initialize utility classes
	log_init(LL_DEBUG, "redisApp", "./");
    OS::Initialize();
    OSThread::Initialize();
    SocketUtils::Initialize(false);
	
	RedisPoolCluster rpl;
	rpl.AddPool("127.0.0.1:6379",100);
	RedisClient cli(&rpl);
	bool ok = cli.Set("RedisTest","TestOK");

    while(1)
    {
        sleep(1);
    }
}
