#include "Thread.h"
#include <stdio.h>


using namespace std;

void Thread::run() {

	char SendBuf[1024];
	int BufLen = 1024;
	sockaddr_in service;
	int servicesize = sizeof(service);
	while (1)
	{
		//ha van kliens, elindul ez a szal

		printf("Receiving datagrams\n");
		int iResult = recvfrom(ClientSocket, SendBuf, BufLen, 0, (SOCKADDR*)&service, &servicesize);//kape uzenetet? erkezik e?

		//HIBA A FOGADASBAN
		if (iResult == SOCKET_ERROR) {

			printf("HIBA A FOGADASNAL AZ ALABBI HIBAKODDAL:\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();//WSACleanup befejezi a Socket mukodeset mifedn szalban
			return;
		}

		SendBuf[iResult] = '\0';//amit kuld
		printf("%s \n", SendBuf);

		//UZENETKULDES
		//UZENET KULDESE MINDENKINEK
		printf("SENDING DATAGRAM\n");
		list<Thread*>::iterator T;//listaban levo klienseknek kuldi egyesevel
		for (T = listThreads->begin(); T != listThreads->end(); ++T) {
			send((*T)->ClientSocket, SendBuf, strlen(SendBuf), 0);
		}

		/*list<Thread*>::iterator T;//listaban levo klienseknek kuldi egyesevel
		for (T = listThreads->begin(); T != listThreads->end(); ++T) {
			recv((*T)->ClientSocket, SendBuf, strlen(SendBuf), 0);
		}*/


	}

}
