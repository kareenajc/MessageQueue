#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <iostream>

using namespace std;

int main() {
	
	//create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCEL | IPC_CREAT | 0600); //handle to queue
																	//ftok = system wide unique key
	
	//declare my message buffer
	struct buf{
		long mtype; //required
		char getting[50]; //mesg content
		//no strings etc. because it's not fixed size
	};
	
	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf
	msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);	//read mesg. mtype = 117
								//don't read "fake mesg
								//header msgrcv(int, struct msgbug *, int, int, int)
	cout << getpid() << ": gets message" << endl;
	cout <<"message: " << msg.greeting <<endl;
	
	strncat(msg.greeting, " and Adios.");
	cout << getpid() << ": sends reply" <<endl;
	msg.mtype = 314; //only reading msg with mtype 314
	msfsnd(qid, (struct msgbuf *)&msg, size, 0);
	cout << getpid() << ": now exits" << endl;
	
	msgrcv(qid, (struct msgbuf *)&msg, size, -112, 0);	
	msgrcv(qid, (struct msgbuf *)&msg, size, 0, 0);	//0 = take the oldest message
	msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);	//tie breaker FIFO
	
	//now safe to delete message queue
	msgctl(qid, IPC_RMID, NULL);	//remove queue
}
