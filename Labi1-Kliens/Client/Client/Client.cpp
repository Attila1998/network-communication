#include <stdio.h>
#include "winsock2.h"
#include "iostream"
#include "ws2tcpip.h"

#pragma comment(lib, "Ws2_32.lib")
// Link with ws2_32.lib
using namespace std;

void main() {

	WSADATA wsaData;
	SOCKET ClientSocket;
	sockaddr_in ServAddr;

	int Port = 1300;
	char SendBuf[1024]="Valami Barmi";
	int BufLen = 1024;



	//---------------------------------------------
	// Initialize Winsock
	// Call WSAStartup and return its value as an integer and check for errors.
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//---------------------------------------------
	// Create a socket for sending data
	ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//---------------------------------------------
	// Set up the ServAddr structure with the IP address of
	// the receiver (server) (in this example case "192.168.113.85")
	// and the specified port number.

//AF_INET is an address family that is used to designate the type of addresses that your socket can communicate 
//with(in this case, Internet Protocol v4 addresses).When you create a socket, you have to specify its address family, 
//and then you can only use addresses of that type with the socket.The Linux kernel, for example, supports 29 other 
//address families such as UNIX(AF_UNIX) sockets and IPX(AF_IPX), and also communications with 
//IRDAand Bluetooth(AF_IRDA and AF_BLUETOOTH, but it is doubtful you'll use these at such a low level).

//For the most part, sticking with AF_INET for socket programming over a network is the safest option.
//There is also AF_INET6 for Internet Protocol v6 addresses.

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(Port);
	inet_pton(AF_INET, "127.0.0.1", &ServAddr.sin_addr);
	//---------------------------------------------
	// Send a datagram to the receiver
	printf("Sending a datagram to the receiver...\n");
	int lengthDatInSendBuf = strlen(SendBuf) + 1;
	sendto(ClientSocket, SendBuf, BufLen, 0, (SOCKADDR*)&ServAddr,sizeof(ServAddr));
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	printf("Receiving datagrams...\n");
	int lengthAddr = sizeof(ServAddr);
	int res = 0;
	int errorcode = 0;
	res =	recvfrom(ClientSocket, SendBuf, BufLen, 0, (SOCKADDR*)&ServAddr, &lengthAddr);
	if (res < 0)
	{
		errorcode = WSAGetLastError();
	}
	printf("A szerverrol lekert ertek: %s" ,SendBuf );
	//printf("%s", SendBuf);
	// kiiratas
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	closesocket(ClientSocket);
	//---------------------------------------------
	// Clean up and quit.
	printf("Exiting.\n");
	WSACleanup();
	return;
}