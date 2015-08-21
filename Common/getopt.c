/*
 * Copyright (c) 2003 Apple Computer, Inc. All rights reserved.
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
 */

#ifdef __Win32__

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "log.h"
#define OPTERRCOLON (1)
#define OPTERRNF (2)
#define OPTERRARG (3)

char *optarg;
int optreset = 0;
int optind = 1;
int opterr = 1;
int optopt;

static int
optiserr(int argc, char * const *argv, int oint, const char *optstr,
         int optchr, int err)
{
    if(opterr)
    {
        LOG_ERROR(stderr, "Error in argument %d, char %d: ", oint, optchr+1);
        switch(err)
        {
        case OPTERRCOLON:
            LOG_ERROR(stderr, ": in flags\n");
            break;
        case OPTERRNF:
            LOG_ERROR(stderr, "option not found %c\n", argv[oint][optchr]);
            break;
        case OPTERRARG:
            LOG_ERROR(stderr, "no argument for option %c\n", argv[oint][optchr]);
            break;
        default:
            LOG_ERROR(stderr, "unknown\n");
            break;
        }
    }
    optopt = argv[oint][optchr];
    return('?');
}
    
   

int
getopt(int argc, char* const *argv, const char *optstr)
{
    static int optchr = 0;
    static int dash = 0; /* have already seen the - */

    char *cp;

    if (optreset)
        optreset = optchr = dash = 0;
    if(optind >= argc)
        return(EOF);
    if(!dash && (argv[optind][0] !=  '-'))
        return(EOF);
    if(!dash && (argv[optind][0] ==  '-') && !argv[optind][1])
    {
        /*
         * use to specify stdin. Need to let pgm process this and
         * the following args
         */
        return(EOF);
    }
    if((argv[optind][0] == '-') && (argv[optind][1] == '-'))
    {
        /* -- indicates end of args */
        optind++;
        return(EOF);
    }
    if(!dash)
    {
        assert((argv[optind][0] == '-') && argv[optind][1]);
        dash = 1;
        optchr = 1;
    }

    /* Check if the guy tries to do a -: kind of flag */
    assert(dash);
    if(argv[optind][optchr] == ':')
    {
        dash = 0;
        optind++;
        return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRCOLON));
    }
    if(!(cp = strchr(optstr, argv[optind][optchr])))
    {
        int errind = optind;
        int errchr = optchr;

        if(!argv[optind][optchr+1])
        {
            dash = 0;
            optind++;
        }
        else
            optchr++;
        return(optiserr(argc, argv, errind, optstr, errchr, OPTERRNF));
    }
    if(cp[1] == ':')
    {
        dash = 0;
        optind++;
        if(optind == argc)
            return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRARG));
        optarg = argv[optind++];
        return(*cp);
    }
    else
    {
        if(!argv[optind][optchr+1])
        {
            dash = 0;
            optind++;
        }
        else
            optchr++;
        return(*cp);
    }
    assert(0);
    return(0);
}

#ifdef TESTGETOPT
int
 main (int argc, char **argv)
 {
      int c;
      extern char *optarg;
      extern int optind;
      int aflg = 0;
      int bflg = 0;
      int errflg = 0;
      char *ofile = NULL;

      while ((c = getopt(argc, argv, "abo:")) != EOF)
           switch (c) {
           case 'a':
                if (bflg)
                     errflg++;
                else
                     aflg++;
                break;
           case 'b':
                if (aflg)
                     errflg++;
                else
                     bflg++;
                break;
           case 'o':
                ofile = optarg;
                (void)qtss_printf("ofile = %s\n", ofile);
                break;
           case '?':
                errflg++;
           }
      if (errflg) {
           (void)LOG_ERROR(stderr,
                "usage: cmd [-a|-b] [-o <filename>] files...\n");
           exit (2);
      }
      for ( ; optind < argc; optind++)
           (void)qtss_printf("%s\n", argv[optind]);
      return 0;
 }

#endif /* TESTGETOPT */

#endif /* WIN32 */
