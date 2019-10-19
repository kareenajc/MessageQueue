#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "kill_patch.h" //the header file that allows ProbeC to terminate when the User enters a kill command

using namespace std;

int main ()
{
	int rho = 251;
	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it

	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf {
		long mtype;	//required
		pid_t pidA;	//pid for ProbeA
		pid_t pidB;	//pid for ProbeB
		pid_t pidC;	//pid for ProbeC
		char greeting[50]; //mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//find pid for probe C
	msg.pidC = getpid();

	int randomNum = 0;

	exit(0);
}
