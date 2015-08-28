#include <sstream>
#include <sys/resource.h>
#include "OS.h"
#include <stdio.h>
#include "SocketUtils.h"
#include "PBReflect.h"
#include "protocol/presence.pb.h"
#include "protocol/user_management_service.pb.h"
#include "protocol/terminal_protocol.pb.h"
#include "TestCliFramework.h"
#include "linenoise.h"
#include "CommonDef.h"
#include "config.h"
#include "SendCmdThread.h"
#include "TestCaseCtx.h"
#include "UserAuthCommand.h"
#include "ClientConnectionMgr.h"
#include "FileReader.h"

const char* history_path = "history.txt";
void        Reg_PBReflect();
static void Parse_Options(int argc, char **argv);
static void Usage(void) ;
static void Trace_ARGC_ARGV(int argc, char *argv[]);
static void Completion_Callback(const char *buf, linenoiseCompletions *lc);
void        Split_Args(const char* cmdStr,std::vector<std::string>& argVec);
void        Sig_Catcher(int sig);
static void Setup_Signal();
static int  Setup_Limit();
void SetSendThreadContinue(bool bContinue);
void ConnectToServer(const std::vector<std::string>& vec);
void DisConnectServer(const std::vector<std::string>& vec);
void PrintTestInfo();
std::vector<SharedPtr<SendCmdThread> > gThreadVec;
int main(int argc, char * argv[]) 
{
    log_init(LL_DEBUG, "log", "./");
    Reg_PBReflect();
    gTestCliFw::instance()->Init(5,5);
    gTestCliFw::instance()->Run();
    Parse_Options(argc,argv);
    Setup_Signal();
    Setup_Limit();
    Trace_ARGC_ARGV(argc,argv);
    TestCaseCtx::regCmd();
	
	FileReader reader("./example.dat");
    reader.ReadValues();
    gConfigPtr::instance()->m_rows = reader.getData();
	
    int threadNum = gConfigPtr::instance()->m_cmdThreadNum;
    for(int i = 0; i < threadNum; i++)
    {
        SendCmdThread *pThread = new SendCmdThread();
        gThreadVec.push_back(pThread);
        pThread->Start();
    }
	
    linenoiseSetCompletionCallback(Completion_Callback);	
    linenoiseHistoryLoad(history_path);		
	
    char* line;
    while((line = linenoise("TestCli> ")) != NULL) 
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
                    break;
                } else if (argc == 5 && !strcasecmp(argVec[0].c_str(),"connect")){
                    ConnectToServer(argVec);
                } else if (argc == 2 && !strcasecmp(argVec[0].c_str(),"disconnect")){
                    DisConnectServer(argVec);
                } else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"clear")){
                                linenoiseClearScreen();
                } else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"stop")){
                	SetSendThreadContinue(false);
                } else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"start")){
                	SetSendThreadContinue(true);
                } else if (argc == 1 && !strcasecmp(argVec[0].c_str(),"info")){
                	PrintTestInfo();
                } else if (argc >= 3){
				    gTestCaseCtxPtr::instance()->SetCurArg(argVec);
                } else {
                    printf("Invalid Command,args should equal or greater than 3\n"
                           "    arg1:Command Name,eg:UserAuth \n"
                           "    arg2:Run Times,eg:100 \n"
                           "    arg3:Run All The Time? ,eg:Yes or No \n"
                           "Support Commands: \n"
                           "    1:UserAuth \n"
                           "    2:GetUserInfo\n"
                           "    3:GetFriendInfo\n"
                           "    4:GetFriendList\n"
                           "    5:QuitUser\n"
                           "    6:UpdateUserInfo\n"
                           "    7:ChgUserInfo\n"
                           "    8:GetUserState \n");
               }
            }
        }
        
        free(line);
    }
    return 0;
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

void Reg_PBReflect()
{
    PBReflect::RegisterPbMsg(0x4301,tt_presence::ChgUserInfoRequset::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4302,tt_presence::ChgUserInfoNotify::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4303,tt_presence::GetUserStateRequset::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4304,tt_presence::GetUserStateResponse::descriptor()->full_name());

    PBReflect::RegisterPbMsg(0x4001,umsProtocol::AuthUserRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4002,umsProtocol::QuitUserRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4003,umsProtocol::GetFriendListRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4004,umsProtocol::GetUserInfoRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4005,umsProtocol::GetFriendInfoRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x401e,umsProtocol::UpdateUserInfoRequest::descriptor()->full_name());	
    PBReflect::RegisterPbMsg(0x4101,TerminalProtocol::LoginRequest::descriptor()->full_name());
    PBReflect::RegisterPbMsg(0x4103,TerminalProtocol::LogoutRequest::descriptor()->full_name());    
}

static void Completion_Callback(const char *buf, linenoiseCompletions *lc)
{

}

static void Parse_Options(int argc, char **argv) 
{
    for (int i = 1; i < argc; i++) 
    {
        int lastarg = i == argc - 1;
        if (!strcmp(argv[i],"-h") && !lastarg) 
        {
            gConfigPtr::instance()->m_hostip = std::string(argv[++i]);
        } 
        else if (!strcmp(argv[i],"--help")) 
        {
            Usage();
        } 
        else if (!strcmp(argv[i],"-p") && !lastarg) 
        {
            gConfigPtr::instance()->m_hostport = atoi(argv[++i]);       
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

static void Trace_ARGC_ARGV(int argc, char *argv[])
{
    int i = 0;
    printf("main(%d,", argc);
    if(argc > 0)
    {
        for(i = 0; i < argc; ++i)
        {
            printf(" \"%s\"", argv[i]);
        }
    }
    else
    {
        printf(" NULL");
    }
    printf(");\r\n");
}

static void Usage(void) 
{
    printf("Usage: TestClient    [OPTIONS] [cmd [arg [arg ...]]]\n"
        "  -h <hostname>      Server hostname (default: 127.0.0.1).\n"
        "  -p <port>          Server port (default: 6379).\n"
        "\n"
        "Examples:\n"
        "  TestClient -h 192.168.15.120 -p 1000 \n"
        "\n");
    exit(1);
}

static int Setup_Limit()
{
    struct rlimit rlim_old;
    struct rlimit rlim_new;
    if (0 != getrlimit(RLIMIT_NOFILE, &rlim_old)) 
    {
        LOG_ERROR("can't get max filedescriptors,error:%s",strerror(errno));
    }

    if(gConfigPtr::instance()->m_max_fds <= 0)
        gConfigPtr::instance()->m_max_fds = 1024;
    rlim_new.rlim_cur = gConfigPtr::instance()->m_max_fds;
    rlim_new.rlim_max = gConfigPtr::instance()->m_max_fds;

    if (0 != setrlimit(RLIMIT_NOFILE, &rlim_new))
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = rlim_old.rlim_max;
        setrlimit(RLIMIT_CORE, &rlim_new);
        LOG_ERROR("can't set max filedescriptors,error:%s",strerror(errno));
    }

    if (0 != getrlimit(RLIMIT_CORE, &rlim_old))
    {
        LOG_ERROR("can't get max limit_core,error:%s",strerror(errno));
    }

    if(gConfigPtr::instance()->m_core_size <= 0)
        rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
    else
        rlim_new.rlim_cur = rlim_new.rlim_max = gConfigPtr::instance()->m_core_size;
    if(0 != setrlimit(RLIMIT_CORE, &rlim_new)) 
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = rlim_old.rlim_max;
        setrlimit(RLIMIT_CORE, &rlim_new);
        LOG_ERROR("can't set max limit_core,error:%s",strerror(errno));
    }
    return 0;
}

static void Setup_Signal()
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = Sig_Catcher;
    (void)::sigaction(SIGPIPE, &act, NULL);
    (void)::sigaction(SIGHUP, &act, NULL);
    //(void)::sigaction(SIGINT, &act, NULL);
    //(void)::sigaction(SIGTERM, &act, NULL);
    //(void)::sigaction(SIGQUIT, &act, NULL);
    //(void)::sigaction(SIGALRM, &act, NULL);
}

void Sig_Catcher(int sig)
{
    LOG_DEBUG("Receive Sig:%d",sig);
    switch (sig) {
    case SIGTERM:
        break;
    case SIGINT:
        break;
    case SIGALRM:
        break;
    case SIGHUP:
        break;
    case SIGCHLD:  
        break;
    default:
        break;
    }
    //pid_t myPID = getpid();
}

void SetSendThreadContinue(bool bContinue)
{
    std::vector<SharedPtr<SendCmdThread> >::iterator iter = gThreadVec.begin();
    for(;iter != gThreadVec.end(); iter++)
    {
        (*iter)->SetContinue(bContinue);
    }
    gTestCaseCtxPtr::instance()->m_bContinue = bContinue;

    if(bContinue)
    {
        gTestCaseCtxPtr::instance()->Reset();
    }
    printf("Set Continue:%d\n",bContinue);
}

void ConnectToServer(const std::vector<std::string>& vec)
{
    if(vec.size() != 5)
        printf("Invalid Connect Command,eg:connect ums 192.168.12.159 9932 10");
    int port = atoi(vec[3].c_str());
    int connNum = atoi(vec[4].c_str());
    UInt32 serverID = UMSServerID;
    if(vec[1] == "ums")
        serverID = UMSServerID;
    else if(vec[1] == "presence")
        serverID = PresenceServerID;
    else
        printf("The Server Type Error,%s",vec[1].c_str());

    gTestCliFw::instance()->NoticeTcpConnect(serverID
        ,vec[2].c_str()
        ,port
        ,connNum
        ,5
        ,5);
}

void DisConnectServer(const std::vector<std::string>& vec)
{
    if(vec.size() != 2)
        printf("Invalid Connect Command,eg:disconnect ums");
    UInt32 serverID = UMSServerID;
    if(vec[1] == "ums")
        serverID = UMSServerID;
    else if(vec[1] == "presence")
        serverID = PresenceServerID;
    else
        printf("The Server Type Error,%s",vec[1].c_str());

    gCliConnMgr::instance()->Close(serverID);
}

void PrintTestInfo()
{
    UInt32 reqNum = ProtoCmdBase::GetSeqID() - gTestCaseCtxPtr::instance()->m_StartReqNum.value();
    printf("Test Information\n"
          "Send Request     :%u\n"
          "receive Response :%u\n"
          "Timeout          :%u\n"
          "Cost             :%ld\n"
          "Min              :%ld\n"
          "Max              :%ld\n"
          "Avg              :%ld\n"
          "Create Cmd Objs  :%u\n"
          "Delete Cmd Objs  :%u\n",
          reqNum,
          gTestCaseCtxPtr::instance()->m_ResponseNum.value(),
          gTestCaseCtxPtr::instance()->m_timeoutNum.value(),
          gTestCaseCtxPtr::instance()->GetEndTime() - gTestCaseCtxPtr::instance()->GetStartTime(),
          gTestCaseCtxPtr::instance()->GetMinTime(),
          gTestCaseCtxPtr::instance()->GetMaxTime(),
          gTestCaseCtxPtr::instance()->GetTotle() / reqNum,
          gTestCaseCtxPtr::instance()->m_createCmdObjs.value(),
          gTestCaseCtxPtr::instance()->m_deleteCmdObjs.value());		   
}
