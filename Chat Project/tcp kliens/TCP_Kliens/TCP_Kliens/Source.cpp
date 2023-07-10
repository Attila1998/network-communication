#include <stdio.h>
#include "winsock2.h"
#include <iostream>
#include <ws2tcpip.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
// Link with ws2_32.lib
using namespace std;

void main() {

	int iResult;//vissza ertek tarolasahoz

	WSADATA wsaData;									
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);// Initialize Winsock
	if (iResult != NO_ERROR)
		printf("Hiba a WSAStartup()\n");

	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ClientSocket == INVALID_SOCKET) {
		printf("Hiba a socket inicializalasanal a kovetkezo hibakoddal: %ld\n", WSAGetLastError());//wsaCleanup befejezi a windows sockets mukodeset minden szalban
		WSACleanup();
		return;
	}

	sockaddr_in ServAddr;
	int Port = 13000;
	int AddrLen = sizeof(ServAddr);
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(Port);
	ServAddr.sin_addr.S_un.S_addr;
	inet_pton(AF_INET, "127.0.0.1", &ServAddr.sin_addr.s_addr);

	

	//Kapcsolat letrehozasa
	if (connect(ClientSocket, (SOCKADDR*)&ServAddr, AddrLen) == SOCKET_ERROR) {
		
		printf("Hiba a kapcsolódásnál a következõ hibakóddal: %ld\n",
			WSAGetLastError());
		// WSACleanup befejezi a Windows Sockets mûködését minden szálban!!!!
		WSACleanup();
		return;

	}

	const int SendBufLen = 1024;
	char SendBuf[SendBufLen];
	int DataLen;
	while (1) {
		printf("Type a message");
		fgets(SendBuf, 1024, stdin);
		DataLen = strlen(SendBuf) + 1; //a legutolso karakter

		if (strcmp(SendBuf, "close\n") == 0 || strcmp(SendBuf, "colse") == 0) {
			printf("Close");
			break;
		}

		//KULDES
		//CIM, hogy hova kuldom,  socket , adat hossz

		printf("Uzenetk kuldese\n");
		iResult = send(ClientSocket, SendBuf, DataLen, 0);

		if (strcmp(SendBuf, "end\n") == 0) {
			printf("Kapcsolat bontasa \n");
			break;
		}

		if (iResult == SOCKET_ERROR) {
			printf("Hiba a kuldesnel:%d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}


		//FOGADAS
		const int RecBufLen = 1024;
		char RecBuf[RecBufLen];
		printf("Uzenetre varok:\n");
		iResult = recv(ClientSocket, RecBuf, RecBufLen - 1, 0);
		printf("Az uzenet megerkezett\n");

		//Ha hiba van a fogadasnal

		if (iResult == SOCKET_ERROR) {
			printf("Hiba a fogadasnal: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		
		}

		//ha lezartuk a socketet

		if (iResult == 0) {
			return;
		}

		//eredmeny

		RecBuf[iResult] = '\0';
		printf("\"%s\" \n", RecBuf);
		printf("\n");
	
	}
	//kapcsolatbontas
	iResult = shutdown(ClientSocket, SD_SEND);

	// WSACleanup befejezi a Windows Sockets mûködését minden szálban!!!!
	WSACleanup();
	system("pause");
	return;


}