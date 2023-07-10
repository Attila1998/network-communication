#pragma once
#include "SysThread.h"
#include <list>

using namespace std;

class Thread : public SysThread {

public:
	SOCKET ClientSocket;
	list<Thread*>* listThreads;
	void run();

	Thread(list<Thread*>* listThread) {
		listThreads = listThread;
	}
};

