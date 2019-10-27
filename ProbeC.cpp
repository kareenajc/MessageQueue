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
	int rho = 7027;

	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf {
		long mtype;	//required
		char greeting[50]; //mesg content
	};

	/* initialize random seed: */
  	srand (time(NULL));

	/* allows an infinite loop */
	bool infin = true;

	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//find pid for probe C
	pid_t pidC = getpid();

	//convert pid to string
	char mypid[50];

	sprintf(mypid,"%d",pidC);

	string pid = mypid;

	//creating message
	string message = "Probe C - " + pid;
	char * convert = new char [message.length() + 1];
	strcpy(convert, message.c_str());

	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it
	/* apply the kill command */
	msg.mtype = 314; //sending msg with mtype 314
	strncpy(msg.greeting, "exit", size); //creating message
	kill_patch(qid, (struct msgbuf *)&msg, size, 314);

	/* initialize the random integer */
	int randomNum = 0;

	while(infin){
		/* generate a random integer */
  		randomNum = rand();
		//check for valid reading
		if(randomNum % rho == 0){
			//send message
			cout << "ProbeC sends message" << endl;
			msg.mtype = 314; //sending msg with mtype 314
			strncpy(msg.greeting, convert, size); //creating message
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sending message
		}
	}

	exit(0);
}
