#include "BuildCommand.h"

const char* BuildCommand::m_CRLF = "\r\n";

string BuildCommand::getCommand()
{
    return ( *m_param.begin() );
}
