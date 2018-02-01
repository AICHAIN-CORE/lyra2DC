// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sched.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "Lyra2DC.h"

int ExitFlag=0;
void termination_handler(int signum)
{
    ExitFlag = 1;
    signal (signum, termination_handler);
}

int HexToBin(const char* psz, unsigned char *bytebin)
{
	unsigned char a,b;
	int i;
	int len=0;
	if(strlen(psz)%2)
		return 0;

	for(i=0;i<strlen(psz)/2;i++)
	{
		if(psz[i*2]>='0' && psz[i*2]<='9')
			a=(psz[i*2]-'0');
		else if(psz[i*2]>='a' && psz[i*2]<='f')
			a=(psz[i*2]-'a')+10;
		else if(psz[i*2]>='A' && psz[i*2]<='F')
			a=(psz[i*2]-'A')+10;
		else return 0;

		if(psz[i*2+1]>='0' && psz[i*2+1]<='9')
			b=(psz[i*2+1]-'0');
		else if(psz[i*2+1]>='a' && psz[i*2+1]<='f')
			b=(psz[i*2+1]-'a')+10;
		else if(psz[i*2+1]>='A' && psz[i*2+1]<='F')
			b=(psz[i*2+1]-'A')+10;
		else return 0;

		bytebin[len++]=(a<<4)+b;
	}
    return len;
}

int BinToHexStr(char *pStr, unsigned char *data, int len)
{
	int i;
	int sigLen=0;
	for(i=0;i<len;i++)
		sigLen+=sprintf(pStr+sigLen,"%02x",data[i]);
	return sigLen;
}

int main(int argc, char* argv[])
{
    char strHash[128];
    unsigned char lyra2DC_hash[32];
	unsigned char test_data[80];	// fixed to 80 bytes test data, will be used to mining and blockchain calculation
	int i;
	
#if 0	/* Become a daemon: */
    switch (fork ())
	{
	case -1:					/* can't fork */
		   perror ("fork()");
		   exit (3);
		 case 0:					 /* child, process becomes a daemon: */
		//	 close (STDIN_FILENO);
		//	 close (STDOUT_FILENO);
		//	 close (STDERR_FILENO);
		   if (setsid () == -1) 	 /* request a new session (job control) */
			 {
			   exit (4);
		 }
	   break;
	default:					/* parent returns to calling process: */
	   return 0;
	}

	/* Establish signal handler to clean up before termination: */
	if (signal (SIGTERM, termination_handler) == SIG_IGN)
		signal (SIGTERM, SIG_IGN);
	signal (SIGINT, SIG_IGN);
	signal (SIGHUP, SIG_IGN);
#endif

    signal(SIGPIPE, SIG_IGN);

    for(i=0;i<8;i++)
    {
        memset(test_data,i,sizeof(test_data));
        lyra2dc_hash((const char *)test_data,(char *)lyra2DC_hash);
		BinToHexStr(strHash,lyra2DC_hash,32);

		printf("test %d lyra2DC_hash=%s\n",i,strHash);
    }
    return 0;
}
