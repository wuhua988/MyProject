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
    File:       TCPListenerSocket.h

    Contains:   A TCP listener socket. When a new connection comes in, the listener
                attempts to assign the new connection to a socket object    
*/


#ifndef __TCPLISTENERSOCKET_H__
#define __TCPLISTENERSOCKET_H__

#include "TCPSocket.h"

class TCPListenerSocket : public TCPSocket
{
public:

    TCPListenerSocket() {}
    virtual ~TCPListenerSocket() {}

    // Send a TCPListenerObject a Kill event to delete it.
            
    //addr = listening address. port = listening port. Automatically
    //starts listening
    OS_Error Initialize(UInt32 addr, UInt16 port);
    int Accept(int& fd,struct sockaddr_in& addr);
private:
    enum
    {
        kTimeBetweenAcceptsInMsec = 1000,   //UInt32
        kListenQueueLength = 128            //UInt32
    };
   
    OS_Error Listen(UInt32 queueLength);
};
#endif // __TCPLISTENERSOCKET_H__

