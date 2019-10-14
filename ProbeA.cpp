#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <stdio.h>      //printf, scanf, puts, NULL
#include <stdlib.h>     //srand, rand
#include <time.h>       //time
#include <iostream>

using namespace std;

int main() {

	int alpha = 997;
	

	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it
	
	//declare my message buffer
	struct buf{
		long mtype; //required
		char greeting[50]; //mesg content
		//no strings etc. because it's not fixed size
	};
	
	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//initialize random seed from time
  	srand (time(NULL));

	do {
		//generate a random integer
  		int randomNum = rand();
		
		//check if ProbeA should terminate
		if(ramdomNum < 100){
			cout << getpid() << " will be terminated." << endl;
		}
		else {
			//check for valid reading
			if(randomNum % alpha == 0){
				//send message
				strncat(msg.greeting, getpid() + " sent a message", size); //creating message
				cout << getpid() << ": ProbeA sends message" << endl;
				msg.mtype = 314; //only reading msg with mtype 314
				msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sending message

				msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);	//read mesg. mtype = 117
				cout << getpid() << ": gets message from DataHub" << endl;
			}
			
		}
	} while(randomNum >= 100)

	exit(0);	
}
