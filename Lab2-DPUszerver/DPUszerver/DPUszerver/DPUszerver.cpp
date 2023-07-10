#define _CRT_SECURE_NO_WARNINGS 1 
#include <stdio.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include <ctime>
#include <iostream>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

void main() {

	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	int Port = 13000;
	char RecvBuf[1024];
	char SendBuf[1024];
	int BufLen = 1024;
	int SenderAddrSize = sizeof(RecvAddr);
	int iResult = 0;
	//---------------------------------------------
	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "123.456.789.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	inet_pton(AF_INET, "127.0.0.1", &RecvAddr.sin_addr.s_addr);
	//----------------------
	// Bind the socket.
	if (bind(SendSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(SendSocket);
		return;
	}
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	printf("Receiving datagrams...\n");

	recvfrom(SendSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&RecvAddr, &SenderAddrSize);
	// ido lekerese es elkuldese
	time_t my_time = time(NULL);
	char* Buffer;
	// ctime() used to give the present time 
	Buffer = (char*)ctime(&my_time);


	//---------------------------------------------
	// Send a datagram to the receiver
	printf("Sending a datagram to the receiver...\n");
	sendto(SendSocket, Buffer, sizeof(SendBuf), 0, (SOCKADDR*)&RecvAddr, SenderAddrSize);
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	closesocket(SendSocket);
	//---------------------------------------------
	// Clean up and quit.
	printf("Exiting.\n");
	WSACleanup();
	return;
}
