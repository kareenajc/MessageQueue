#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <iostream>
#include <stdlib.h>	//for exit(0), atoi
#include "force_patch.h" //the header file that allows ProbeB to terminate when the hub reaches 10,000 messages

using namespace std;

int main() {		
	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf{
		long mtype; //required
		char greeting[50]; //mesg content
				//no strings etc. because it's not fixed size
	};

	//create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL | IPC_CREAT | 0600);
									
	buf msg;
	int size = sizeof(msg) - sizeof(long);

	bool activeA = true;
	bool activeB = false;
	bool activeC = false;

	pid_t pidB;

	// string for comparison
	string cmpr;
	

	int counter = 0; //counts number of messages ProbeB has sent, so we can terminate when it hits 10,000

	do {
		msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0);	//read mesg. mtype = 314
		char probe[7];	//identify which probe sent the message
		strncpy(probe, msg.greeting, 7);
		cout <<"Message Incoming From: "<< msg.greeting << endl;
		
		if(strcmp(msg.greeting, "Probe A - terminate") == 0){ //Check if ProbeA should terminate
			activeA = false;
			cout << "Probe A will terminate" <<endl;
		}		
		else if(strcmp(probe, "Probe A") == 0){
			// reinitialize comparison string
			cmpr = "";
			for(int i = 0; i < size; i++){
				cmpr = cmpr + msg.greeting[i];
			}
			if(cmpr.compare(0,7,"Probe A") == 0) {
				strncpy(msg.greeting, "return acknowledgement", size);
				cout << getpid() << ": DataHub sent a message" << endl;
				msg.mtype = 117;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		}
		else if(strcmp(probe, "Probe B") == 0){	//Check if ProbeB sent the message
			activeB = true;
			char pid[50];
			strncpy(pid, msg.greeting+10, sizeof(msg.greeting)-10);
			pid[sizeof(msg.greeting)-7] = '\0';
			pidB = atoi(pid); //pid char[] converted to int
			cout<<pidB;
		}
		else if(strcmp(probe, "Probe B") == 0){ //Check if ProbeC sent the message
			activeC = true;
		}

		counter++; //increments after every message received

		//Check if ProbeB should terminate
		if(counter >= 10000 && activeB == true){
			cout << "Probe B will terminate" << endl;
			cout <<"PID: "<<pidB<<endl;
			activeB = false;
			force_patch(pidB);
		}
		//Check if ProbeC has terminated
		if(strcmp(msg.greeting, "exit") == 0){
			activeC = false;
			cout << "Probe C was terminated" << endl;
		}
		cout << counter << endl;
	}while(activeB == true);
	//while(activeA == true || activeB == true || activeC == true);

	cout << "DataHub terminating."<<endl;
	
	//now safe to delete message queue
	msgctl(qid, IPC_RMID, NULL);	//remove queue
	
	exit(0);
}
