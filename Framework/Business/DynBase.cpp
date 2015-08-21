#include "DynBase.h"
#include "ProtoCmdBase.h"
 
bool Register(ClassInfo* ci)
{
	return ProtoCmdBase::Register(ci);
}
