#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main() {
	//pause program A
//	sleep(3); 	//BAD programming - unreliable & portential bug
	
	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it
											// 0 = find
											
	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf {
		long mtype;	//required
		char greeting[50]; //msg conent
	};
	
	buf msg;
	int size = sizeof(msg) - sizeof(long);
	
	//sending fake messages
	msg.mtype = 111;
	strncpy(msg.greeting, "Fake message", size);	//use strncpy instead!!!
	msgsnd(qid, (struct msfbuf *)&msg, size, 0);
	
	msg.mtype = 113;
	strncpy(msg.greeting, "Another Fake", size);
	msgsnd(qid, (struct msfbuf *)&msg, size, 0);
	
	//sending real messages
	msg.mtype = 117;
	strncpy(msg.greeting, "Hello there", size);
	cout << getpid() <<": sends greeting" <<endl;
	msgsnd(qid, (struct msfbuf *)&msg, size, 0);	//sending
							//0 = halt here until message is copied into queue successfully
							//non-zero = causes busy wait (avoid!!!!)
													
	msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0);	//reading
								//0 = halt here until a message is extracted from the queue
	cout <<getpid() <<": gets reply" << endl;
	cout << "reply: " << msg.greeting <<endl;
	cout << getpid() << ": now exits" << endl;
	
	msg.mtype = 117;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	exit(0);
}
