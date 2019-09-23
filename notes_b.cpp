int main() {
	//pause program A
	sleep(3); 	//BAD programming - unreliable & portential bug
	
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
	
	//sending
	msg.mtype = 111;
	strcpy(msg.greeting, "Fake message");	//use strncpy instead!!!
	msgsnd(qid, *struct msfbuf *)&msg, size, 0);
	
	msg.mtype = 113;
	strcpy(msg.greeting, "Another Fake");
	msgsnd(qid, *struct msfbuf *)&msg, size, 0);
	
	//prepare my message to send
	msg.mtype = 117;
	strcpy(msg.greeting, "Hello there");
	cout << getpid() <<": sends greeting" <<endl;
	msgsnd(qid, *struct msfbuf *) &msg, size, 0);	//sending
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