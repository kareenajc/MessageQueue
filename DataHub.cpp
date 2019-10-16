#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <iostream>
#include <stdlib.h>	//for exit(0)
#include "force_patch.h" //the header file that allows ProbeB to terminate when the hub reaches 10,000 messages

using namespace std;

int main() {		
	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf{
		long mtype; //required
		pid_t pidA;
		pid_t pidB;
		pid_t pidC;
		char greeting[50]; //mesg content
				//no strings etc. because it's not fixed size
	};

	//create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL | IPC_CREAT | 0600);
									
	buf msg;
	int size = sizeof(msg) - sizeof(long);

	bool activeA = true;
	bool activeB = true;
	bool activeC = true;

	int counter = 0; //counts number of messages ProbeB has sent, so we can terminate when it hits 10,000

	do {
		msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0);	//read mesg. mtype = 314
		cout <<"message from probeA:" << msg.greeting << endl;

		//Check if ProbeA should terminate
		if(strcmp(msg.greeting, "terminate") == 0){
			activeA = false;
			cout << "Probe A will terminate" <<endl;
		}
//		else if(counter >= 10000){
			
//		}		
		else {
			strncpy(msg.greeting, "return acknowledgement", size);
			cout << getpid() << ": DataHub sent a message" << endl;
			msg.mtype = 117;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			counter++;
		}

	}while(activeA == true);
	//while(activeA == true || activeB == true || activeC == true);

	cout << "DataHub terminating."<<endl;
	
	//now safe to delete message queue
	msgctl(qid, IPC_RMID, NULL);	//remove queue
	
	exit(0);
}
