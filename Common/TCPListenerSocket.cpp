/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2008 Apple Inc.  All Rights Reserved.
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 *
 */
/*
    File:       TCPListenerSocket.cpp
    Contains:   implements TCPListenerSocket class
*/

#ifndef __Win32__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

#include <errno.h>
#include "TCPListenerSocket.h"
#include "OSThread.h"
#include "SafeStdLib.h"

OS_Error TCPListenerSocket::Listen(UInt32 queueLength)
{
    if (fFileDesc == kInvalidFileDesc)
        return EBADF;
        
    int err = ::listen(fFileDesc, queueLength);
    if (err != 0)
        return (OS_Error)OSThread::GetErrno();
    return OS_NoErr;
}

OS_Error TCPListenerSocket::Initialize(UInt32 addr, UInt16 port)
{
    OS_Error err = this->TCPSocket::Open();
    if (0 == err) do
    {   
        // set SO_REUSEADDR socket option before calling bind.
#ifndef __Win32__
        // this causes problems on NT (multiple processes can bind simultaneously),
        // so don't do it on NT.
        this->ReuseAddr();
#endif
        err = this->Bind(addr, port);
        if (err != 0) break; // don't assert this is just a port already in use.

        //
        // Unfortunately we need to advertise a big buffer because our TCP sockets
        // can be used for incoming broadcast data. This could force the server
        // to run out of memory faster if it gets bogged down, but it is unavoidable.
        this->SetSocketRcvBufSize(96 * 1024);       
        err = this->Listen(kListenQueueLength);
        AssertV(err == 0, OSThread::GetErrno()); 
        if (err != 0) break;
        
    } while (false);
    
    return err;
}

int TCPListenerSocket::Accept(int& fd,struct sockaddr_in& addr)
{
#if __Win32__ || __osf__ || __sgi__ || __hpux__	
    int size = sizeof(addr);
#else
    socklen_t size = sizeof(addr);
#endif

	int osSocket = accept(fFileDesc, (struct sockaddr*)&addr, &size);
    int acceptError = -1;
    //test osSocket = -1;
	if (osSocket == -1)
	{
        //take a look at what this error is.
        acceptError = OSThread::GetErrno();
        if (acceptError == EAGAIN)
        { 
            //If it's EAGAIN, there's nothing on the listen queue right now,
            LOG_DEBUG("Accept EAgain!");
            return acceptError;
        }
		
        //test acceptError = ENFILE;
        //test acceptError = EINTR;
        //test acceptError = ENOENT;
		 
        //if these error gets returned, we're out of file desciptors, 
        //the server is going to be failing on sockets, logs, qtgroups and qtuser auth file accesses and movie files. The server is not functional.
		if (acceptError == EMFILE || acceptError == ENFILE)
        {           
            #ifndef __Win32__
			LOG_ERROR("Out of File Descriptors. Set max connections lower and check for competing usage from other processes. Exiting.");
            #endif
			exit (EXIT_FAILURE);	
        }
        else
        {   
            char errStr[256];
            errStr[sizeof(errStr) -1] = 0;
            os_snprintf(errStr, sizeof(errStr) -1, "accept error = %d '%s' on socket. Clean up and continue.", acceptError, strerror(acceptError)); 
            if(acceptError != 0)
                LOG_DEBUG("%s",errStr);                      
            return acceptError;
        }
	}	
  
    Assert(osSocket != kInvalidFileDesc);
    
    //set options on the socket
    //we are a server, always disable nagle algorithm
    int one = 1;
    int err = ::setsockopt(osSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int));
    AssertV(err == 0, OSThread::GetErrno());
    
    err = ::setsockopt(osSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));
    AssertV(err == 0, OSThread::GetErrno());
    
    int sndBufSize = 96L * 1024L;
    err = ::setsockopt(osSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBufSize, sizeof(int));
    AssertV(err == 0, OSThread::GetErrno());
    fd = osSocket;
    return acceptError;      
}

