#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <iostream>
#include "force_patch.h" //the header file that allows ProbeB to terminate when the hub reaches 10,000 messages

using namespace std;

int main() {

	//create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL | IPC_CREAT | 0600);
											
	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf {
		long mtype;	//required
		char greeting[50]; //msg conent
	};
	
	buf msg;
	int size = sizeof(msg) - sizeof(long);

	//now safe to delete message queue
	msgctl(qid, IPC_RMID, NULL);	//remove queue
	
	exit(0);
}
