#include <sys/types.h>	//for msgget
#include <sys/ipc.h>	//for msgget
#include <sys/msg.h>	//for msgget
#include <sys/types.h>	//for getpid()
#include <unistd.h>	//for getpid()
#include <string.h>
#include <iostream>

int main ()
{

	int qid = msgget(ftok(".", 'u'), 0);	//find queue, if doesn't exist, create it

	//declare my message buffer
	//needs to be identical to other struct so that message sent & received is identical
	struct buf {
		long mtype;	//required
		char greeting[50]; //msg conent
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long); //type cast to msgbuf pointer from buf
	
	return 0;
}
