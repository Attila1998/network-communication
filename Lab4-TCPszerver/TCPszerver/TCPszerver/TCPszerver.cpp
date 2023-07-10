#include <stdio.h>
#include <string.h>
#include <ws2tcpip.h>
#include "winsock2.h"
#include "iostream"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void main() {
	// Initialize Winsock.
	WSADATA wsaData;
	const int SendBufLen = 1024;
	char SendBuf[SendBufLen];
	const int RecvBufLen = 1024;
	char RecvBuf[RecvBufLen];
	SOCKET SendSocket;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return;
	}
	SendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	int Port = 13;
	char IP[10] = "127.0.0.1";
	sockaddr_in service;
	//int SenderAddrSize = sizeof(service);
	service.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &service.sin_addr.s_addr);
	service.sin_port = htons(Port);

	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		printf("Error listening on socket.\n");
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	//----------------------
	// Create a SOCKET for accepting incoming requests.
	//SOCKET Accept(__in SOCKET s,__out struct sockaddr *addr,__inout int *addrlen);
	SOCKET AcceptSocket;
	printf("Waiting for client to connect...\n");
	//----------------------
	// Accept the connection.
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	else
		printf("Client connected.\n");
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	printf("Receiving datagrams...\n");
	recv(AcceptSocket, RecvBuf, RecvBufLen, 0);
	// ido lekerese es elkuldese
	//---------------------------------------------
   // Send a datagram
	printf("Sending a datagram...\n");
	send(AcceptSocket, SendBuf, SendBufLen, 0);
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	closesocket(SendSocket);
	//---------------------------------------------
	// Clean up and quit.
	printf("Exiting.\n");
	WSACleanup();
	cin.get();
	return;
}