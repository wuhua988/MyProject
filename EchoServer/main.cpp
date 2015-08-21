#include "OS.h"
#include "SocketUtils.h"
#include "PBReflect.h"
#include "protocol/echo.pb.h"
#include "AppFramework.h"

void InitPb();
static void Usage(void);
static void Parse_Options(int argc, char **argv);
string gStrIP;
int gPort;
int main(int argc, char * argv[]) 
{
    //Initialize utility classes
	log_init(LL_DEBUG, "log", "./");
	InitPb();
    AppFramework fw;
    fw.Init(5,0);
	fw.Run();
	Parse_Options(argc,argv);
    fw.NoticeTcpListen(gStrIP.c_str(),gPort);
    while(1)
    {
        sleep(1);
    }
}

static void Parse_Options(int argc, char **argv) 
{
    for (int i = 1; i < argc; i++) 
	{
        int lastarg = i == argc - 1;
        if (!strcmp(argv[i],"-h") && !lastarg) 
		{
            gStrIP = std::string(argv[++i]);
        } 
		else if (!strcmp(argv[i],"--help")) 
		{
            Usage();
        } 
		else if (!strcmp(argv[i],"-p") && !lastarg) 
		{
            gPort = atoi(argv[++i]);       
        } 
		else 
		{
            if (argv[i][0] == '-') 
			{
                fprintf(stderr,"Unrecognized option or bad number of args for: '%s'\n",
                    argv[i]);
                exit(1);
            } else 
			{
                /* Likely the command name, stop here. */
                break;
            }
        }
    }
}

void InitPb()
{
	PBReflect::RegisterPbMsg(1983,App::EchoRequest::descriptor()->full_name());
}

static void Usage(void) 
{
    printf("Usage: echoServer    [OPTIONS] [cmd [arg [arg ...]]]\n"
           "  -h <hostname>      Server hostname (default: 127.0.0.1).\n"
           "  -p <port>          Server port (default: 6379).\n"
           "\n"
           "Examples:\n"
           "  echoServer -h 192.168.15.120 -p 1000 \n"
           "\n");
    exit(1);
}
