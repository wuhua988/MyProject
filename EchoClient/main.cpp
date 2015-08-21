#include "OS.h"
#include <sstream>
#include "SocketUtils.h"
#include "PBReflect.h"
#include "protocol/echo.pb.h"
#include "ClientFramework.h"
#include "linenoise.h"
#include "ClientConnectionMgr.h"

#define SvrID 5
const char* history_path = "history.txt";
void InitPb();
void ConnectToServer(const std::vector<std::string>& vec);
void SendMsgToServer(const std::vector<std::string>& vec);
void DisConnectServer(const std::vector<std::string>& vec);
static void Completion_Callback(const char *buf, linenoiseCompletions *lc);
void Split_Args(const char* cmdStr,std::vector<std::string>& argVec);
int main(int argc, char * argv[]) 
{
    //Initialize utility classes
	log_init(LL_DEBUG, "client", "./");
	InitPb();
    ClientFramework fw;
    fw.Init(5,0);
	fw.Run();

    linenoiseSetCompletionCallback(Completion_Callback);	
    linenoiseHistoryLoad(history_path);		
	
	char* line;
    while((line = linenoise("EchoCli> ")) != NULL) 
	{
        if (line[0] != '\0') 
		{
			std::vector<std::string> argVec;
			Split_Args(line,argVec);
            linenoiseHistoryAdd(line);
            linenoiseHistorySave(history_path);
            argc = argVec.size();
            if (argVec.empty())
            {
                printf("Invalid argument(s)\n");
                free(line);
                continue;
            } else if (argc > 0) {
                if (strcasecmp(argVec[0].c_str(),"quit") == 0 ||
                    strcasecmp(argVec[0].c_str(),"exit") == 0)
                {
                    exit(0);
                } else if (argc == 3 && !strcasecmp(argVec[0].c_str(),"connect")){
                    ConnectToServer(argVec);
				} else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"disconnect")){
					DisConnectServer(argVec);
                } else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"clear")){
                    linenoiseClearScreen();
				} else if (argc == 2){
					SendMsgToServer(argVec);
                } else {
                    printf("Invalid Command,args should equal 2\n"
					       "    arg1:string,eg:hello \n"
						   "    arg2:Send Times,eg:100 \n"
						   "Or connect ip port \n"
						   "Or disconnect \n"
						   "Or clear \n");
				}
		    }
		}
        
        free(line);
    }
	return 0;
}

void ConnectToServer(const std::vector<std::string>& vec)
{
	if(vec.size() != 3)
	{
		printf("Invalid Connect Command,eg:connect 192.168.12.159 9932");
		return;
	}
	int port = atoi(vec[2].c_str());
		
	gEchoCliFw::instance()->NoticeTcpConnect(SvrID
	                                        ,vec[1].c_str()
					                        ,port
					                        ,10
					                        ,5
					                        ,5);
}

void SendMsgToServer(const std::vector<std::string>& vec)
{
	if(vec.size() != 2)
		return;
	UInt32 connID = 0;
	int times = atoi(vec[1].c_str());
	if(gCliConnMgr::instance()->GetCliConnID(SvrID,connID))
	{
		MsgHeader header;
        header.SetVersion(1);
        header.SetCrypt(1);
        header.SetMode(1);
        header.SetContentType(2);
        header.SetCommand(1983);
        header.SetApplicationId(123);
	    header.SetError(200);
        header.SetSequence(1); 
		for(int i = 0; i < times; i++)
		{
			App::EchoRequest echo_resp;
	        echo_resp.set_name("echo_client");
	        echo_resp.set_msg(vec[0]);
	        string strseri;
	        if(echo_resp.SerializeToString(&strseri))
	        {			
	        	MsgBlock* pMsgBlock = MsgBlock::PackMsg(header,strseri.c_str(),strseri.length(),SvrID,connID);
	        	if(pMsgBlock)
	        	{
	        		gEchoCliFw::instance()->SendMsg(pMsgBlock);
	        	}
	        }
		}
	}
}

void DisConnectServer(const std::vector<std::string>& vec)
{
	if(vec.size() != 2)
		printf("Invalid Connect Command,eg:disconnect ums");		
	gCliConnMgr::instance()->Close(SvrID);
}

static void Completion_Callback(const char *buf, linenoiseCompletions *lc)
{
    
}

void Split_Args(const char* cmdStr,std::vector<std::string>& argVec) 
{
	size_t len = strlen(cmdStr);
    for (size_t i = 0; i < len;) 
	{
        bool quote(false);
        std::stringstream stream;

        while (i < len)
		{
            char c = cmdStr[i++];
            if (!quote && c == ' ')
                break;
            else if (c == '"') 
			{
                quote = !quote;
                continue;
            }
            stream.put(c);
        }

        argVec.push_back(stream.str());
    }
}

void InitPb()
{
	PBReflect::RegisterPbMsg(1983,App::EchoRequest::descriptor()->full_name());
}