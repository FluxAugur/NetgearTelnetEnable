/*
gcc -W -Wall -O2 -fno-strict-aliasing blowfish.c md5.c telnetenable.c -o telnetenable2
gcc -W -Wall -O2 -fno-strict-aliasing blowfish.c md5.c telnetenable.c -o telnetenable2 -lws2_32
*/

/*******************************************************************/

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "md5.h"
#include "blowfish.h"

#ifdef _WIN32
#define LASTERRNO WSAGetLastError()
WSADATA wsaData;
#else
#define LASTERRNO errno
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
#endif

struct payload
{
	char signature[0x10];
	char mac[0x10];
	char username[0x10];
	char password[0x10];
	char reserved[0x40];
};

SOCKET sock = INVALID_SOCKET;;
static BLOWFISH_CTX ctx;

char pkt[0x640];
int pktLen;

/*******************************************************************/

static void usage(void)
{
	fprintf(stderr,""
"telnetenable2 (0.0.4) - enable telnet in the new Netgear firmwares.\n"
"Written by alfie, with some code from\n"
"https://github.com/davejagoda/NetgearTelnetEnable\n"
"Released under the terms of the GPL2.\n"
"\n"
#ifdef _WIN32
"Usage: telnetenable2.exe <IP> <MAC> <user> <pass>\n"
#else
"Usage: telnetenable2 <IP> <MAC> <user> <pass>\n"
#endif
"\n"
"\t      IP - the IP of the LAN of the router\n"
"\t     MAC - the MAC address of the LAN of the router\n"
"\t    user - usually admin\n"
"\tpassword - the admin password (the one used to login into the WEB configuration pages)\n"
"\n"
"Example:\n"
#ifdef _WIN32
"telnetenable2.exe 192.168.0.1 010203040506 admin password\n"
#else
"./telnetenable2 192.168.0.1 010203040506 admin password\n"
#endif
"\n");

	exit(1);
}

/*******************************************************************/

static int startAll(void)
{
#ifdef _WIN32
	int res;
	
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res!=NO_ERROR) {
        fprintf(stderr,"WSAStartup failed with error: %d\n", res);
		return 1;
	}
#endif

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock==INVALID_SOCKET) {
        fprintf(stderr,"socket failed with error: %d\n", LASTERRNO);
#ifdef _WIN32
        WSACleanup();
#endif
		return 1;
	}

	return 0;
}

/*******************************************************************/

static void closeAll(void)
{
	closesocket(sock);
#ifdef _WIN32
	WSACleanup();
#endif
}

/*******************************************************************/

static int getOutputLength(unsigned long lInputLong)
{
	unsigned long lVal = lInputLong % 8;

	if (lVal!=0) return lInputLong+8-lVal;
	else return lInputLong;
}

/*******************************************************************/

static int encodeString(BLOWFISH_CTX *ctx,char *pInput,char *pOutput, int lSize)
{
	int lCount, lOutSize, i = 0;

	lOutSize = getOutputLength(lSize);
	lCount = 0;
	while (lCount<lOutSize) {
		char *pi=pInput;
		char *po=pOutput;
		for (i = 0; i<8; i++) *po++=*pi++;
		Blowfish_Encrypt(ctx,(uint32_t *)pOutput,(uint32_t *)(pOutput+4));
		pInput += 8;
		pOutput += 8;
		lCount += 8;
    }

	return lCount;
}

/*******************************************************************/

static int createPkt(char *mac,char *user,char *pass)
{
	MD5_CTX MD;
	char MD5_key[0x10], secret_key[0x400]="AMBIT_TELNET_ENABLE+";
	struct payload pl;

	memset(&pl,0,sizeof(pl));
	strcpy(pl.mac,mac);
	strcpy(pl.username,user);
	strcpy(pl.password,pass);

	MD5Init(&MD);
	MD5Update(&MD,(const unsigned char *)pl.mac,0x70);
	MD5Final((unsigned char *)MD5_key,&MD);

	strncpy(pl.signature,MD5_key,sizeof(pl.signature));
    strncat(secret_key,pass,sizeof(secret_key)-strlen(secret_key) - 1);
	Blowfish_Init(&ctx,(unsigned char *)secret_key,strlen(secret_key));
	pktLen = encodeString(&ctx,(char *)&pl,(char *)&pkt,0x80);

	return 0;
}

/*******************************************************************/

#define PORT 23

static int sendPkt(char *host)
{
    struct sockaddr_in sin;
	int res;

	sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(host);

	if (sin.sin_addr.s_addr==INADDR_NONE) {
        fprintf(stderr,"invalid IP address %s\n",host);
		return 1;
	}
			
    res = sendto(sock,(char *)pkt,pktLen,0,(SOCKADDR *)&sin,sizeof(sin));
    if (res==SOCKET_ERROR) {
        fprintf(stderr,"sendto failed with error: %d\n", LASTERRNO);
		return 1;
    }

	return 0;
}

/*******************************************************************/

int main(int argc, char *argv[])
{
	int res = 1;
	
	if (argc!=5) {
		usage();
		return 1;
	}
	
	if (strlen(argv[2])>0x10-1) {
		fprintf(stderr,"MAC address too long\n");
		return 1;
	}
	
	if (strchr(argv[2],':')!=NULL) {
		fprintf(stderr,"MAC address must have no ':'\n");
		return 1;
	}
	
	if (strlen(argv[3])>0x10-1) {
		fprintf(stderr,"username too long\n");
		return 1;
	}
	
	if (strlen(argv[4])>0x10-1) {
		fprintf(stderr,"password too long\n");
		return 1;
	}

	printf("starting...\n");
	if (startAll()==0) {

		printf("creating pkt...\n");
		if (createPkt(argv[2],argv[3],argv[4])==0) {

			printf("sending pkt...\n");
			if (sendPkt(argv[1])==0) {

				printf("done.\n");
				res = 0;
			}
		}
		
		closeAll();
	}
	
	return res;
}

/*******************************************************************/
