#ifndef __DYNBASE_H__
#define __DYNBASE_H__
#include <stddef.h>
class ProtoCmdBase;
class ClassInfo;

typedef ProtoCmdBase* (*ObjConstructorFn)(void); 
bool Register(ClassInfo* ci);
using namespace std;

class ClassInfo
{
public:
    ClassInfo(int cmdID,ObjConstructorFn ctor)
    :m_cmdID(cmdID),m_objConstructor(ctor)
    {
    	Register(this);
    }
    virtual ~ClassInfo(){}
    ProtoCmdBase* CreateObject() const { return m_objConstructor ? (*m_objConstructor)() : 0;}
    bool IsDynamic()const { return NULL != m_objConstructor;}
    int GetCmdID()const { return m_cmdID;}
    ObjConstructorFn GetConstructor()const{ return m_objConstructor;}
public:
    int m_cmdID;
    ObjConstructorFn m_objConstructor;
};

#define DECLARE_CLASS()                                   \
    protected:                                            \
        static ClassInfo m_classinfo;                     \
    public:                                               \
        virtual ClassInfo* GetClassInfo() const;          \
        static ProtoCmdBase* CreateObject();              
                                                          
#define IMPLEMENT_CLASS_COMMON(cmdID,name,func)           \
    ClassInfo name::m_classinfo((cmdID),                  \
             (ObjConstructorFn) func);                    \
						                                  \
    ClassInfo *name::GetClassInfo() const                 \
    {return &name::m_classinfo;}                     
                                                          
#define IMPLEMENT_CLASS(cmdID,name)			              \
    IMPLEMENT_CLASS_COMMON(cmdID,name,name::CreateObject) \
    ProtoCmdBase* name::CreateObject()                    \
    { return new name;}

#endif