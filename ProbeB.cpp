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
	int beta = 7351;
	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it

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

  	/* initialize the random integer */
  	int randomNum = 0;

	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//find pid for probe B
	pid_t pidB = getpid();

	//convert pid to string
	char mypid[50];

	sprintf(mypid,"%d",pidB);

	string pid = mypid;

	//creating message
	string message = "Probe B - " + pid;
	char * convert = new char [message.length() + 1];
	strcpy(convert, message.c_str());

	while(infin){
		/* generate a random integer */
 	 	randomNum = rand();
		//check for valid reading
		if(randomNum % beta == 0){
			//send message
			cout << "ProbeB sends message" << endl;
			msg.mtype = 314; //sending msg with mtype 314
			strncpy(msg.greeting, convert, size); //creating message
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sending message
		}
	}

	exit(0);
}
