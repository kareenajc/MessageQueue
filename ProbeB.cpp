#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

using namespace std;

int main ()
{
	int beta = 257;
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

	/* initialize random seed: */
  	srand (time(NULL));


  	/* initialize the random integer */
  	int randomNum = 0;

	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//find pid for probe B
	msg.pidB = getpid();

	while(randomNum >= 0){
		/* generate a random integer */
  		randomNum = rand();
		//check for valid reading
		if(randomNum % beta == 0){
			//send message
			cout << msg.pidB << ": ProbeB sends message" << endl;
			msg.mtype = 318; //sending msg with mtype 318
			strncpy(msg.greeting, "Probe B sent a message", size); //creating message
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sending message
		}
	}

	exit(0);
}
