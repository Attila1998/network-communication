#include "ws2tcpip.h"
#include "Thread.h"
#include "SysThread.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
	CRITICAL_SECTION critical_section;
	InitializeCriticalSection(&critical_section);

	// Initialize Winsock.
	//op. rendszer altal megadott socket inicializalasa
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {

		printf("Error at WSAStartup()\n");
		return 1;
	}
	//Create a SOCKET 
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET) {

		printf("ERROR:%d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.

	sockaddr_in service;
	int servicesize = sizeof(service);
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(13000);
	inet_pton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);


	//osszekapcsolja a socketet a porttal
	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//----------------------

	//elobb nezzuk, hogy van e hiba
	//ha nincs, akkor varjuk az uzenetet

	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	// Create a SOCKET for accepting incoming requests.


	//letrehozunk egy socketet, amivel fogadjuk az uzenetet
	SOCKET AcceptSocket;

	//----------------------
	//Accept the connection
	list<Thread*> listThreads;//ez a lista(Kliens)

	while (1) {
		printf("WAITING FOR CLIENT TO CONNECT...\n");
		//letrehozunk egy uj szalat
		Thread* t = new Thread(&listThreads);
		listThreads.push_back(t);//bele teszi a listaba

		//fogadja az uzenetet
		AcceptSocket = accept(ListenSocket, NULL, NULL);
	
		//elobb megvizsgaljuk, ha hiba van,  lezarja a socketet
		if (AcceptSocket == INVALID_SOCKET) {
			printf("ACCEPT FAILED: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		else {
			//ha helyes, fogadjuk  a clienst
			EnterCriticalSection(&critical_section);
			t->ClientSocket = AcceptSocket;
			printf("CLIENT CONNECTED\n");

			if (t->ClientSocket > 0) {
				t->start();
			}
		}
		LeaveCriticalSection(&critical_section);

		t->stop();
	}
	//Amikor befejeztuk a kuldest , a socket lezaruk

	cout<< "FINISH/EXIT\n";
	WSACleanup();
	int c = getchar();
	return 1;

}

