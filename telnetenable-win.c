/*
  This program is a re-implementation of the telnet console enabler utility
  for use with Netgear wireless routers.

  The original Netgear Windows binary version of this tool is available here:
  http://www.netgear.co.kr/Support/Product/FileInfo.asp?IDXNo=155

  Per DMCA 17 U.S.C. §1201(f)(1)-(2), the original Netgear executable was
  reverse engineered to enable interoperability with other operating systems
  not supported by the original windows-only tool (MacOS, Linux, etc).

        Netgear Router - Console Telnet Enable Utility
        Release 0.1 : 25th June 2006
        Copyright (C) 2006, yoshac @ member.fsf.org
        Release 0.2 : 20th August 2012
        dj bug fix on OS X
        Release 0.3 : 8th October 2012
        keithr-git bug fix to send entire packet in one write() call,
          strcpy->strncpy, clean up some whitespace

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License along
        with this program; if not, write to the Free Software Foundation, Inc.,
        51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


  The RSA MD5 and Blowfish implementations are provided under LGPL from
  http://www.opentom.org/Mkttimage
*/

//#include <netinet/tcp.h>

// #include <netinet/udp.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
#include <Winsock2.h>

#include <stdlib.h>
#include <stdio.h>
//#include <process.h>
#include <string.h>

#include "md5.h"
#include "blowfish.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

static char output_buf[0x640];

static BLOWFISH_CTX ctx;

struct PAYLOAD
{
  char signature[0x10];
  char mac[0x10];
  char username[0x10];
  char password[0x21];
  char reserved[0x2F];
} payload;

void usage(char * progname)
{
  printf("\nVersion: 0.5, 2015/03/01\n");
  printf("Modified to work with newer Negear routers R7000 R7500 by insanid\n");
  printf("\nUsage:\n%s <host ip> <host mac> <user name> <password>\n\n",progname);
  exit(-1);
}

// int socket_connect(char *host, in_port_t 
