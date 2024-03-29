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

	int alpha = 24001;  //magic seed - random number must be divisible by this

	//declare my message buffer
	struct buf{
		long mtype; //required
		char greeting[50]; //mesg content
				//no strings etc. because it's not fixed size
	};

	//find or create queue
	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it

	//declare message
	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf

	//find pid for probe A
	pid_t pidA = getpid();

	//convert pid to string
	char mypid[50];

	sprintf(mypid,"%d",pidA);

	string pid = mypid;

	//creating message
	string message = "Probe A - " + pid;
	char * convert = new char [message.length() + 1];
	strcpy(convert, message.c_str());

	//initialize random seed from time
  	srand (time(NULL));
	int randomNum = 120; //ensures the while loop is entered

	while(randomNum >= 100){
		//generate a random integer
  		randomNum = rand();
		//cout << randomNum <<endl;

		//check if ProbeA should terminate
		if(randomNum < 100){
			cout << "Random number: " << randomNum << "\nProbe A will be terminated." << endl;
			msg.mtype = 314; //sending msg with mtype 314
			strncpy(msg.greeting, "Probe A - terminate", size);
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
		else {
			//check for valid reading
			if(randomNum % alpha == 0){
				//send message
				cout << "ProbeA sends message" << endl;
				msg.mtype = 314; //sending msg with mtype 314
				strncpy(msg.greeting, convert, size); //creating message
				msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sending message
				//wait for acknowledgement
				msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);	//read mesg. mtype = 117
				cout << "ProbeA gets message from DataHub" << endl;
			}
			
		}

	}


	exit(0);	
}
