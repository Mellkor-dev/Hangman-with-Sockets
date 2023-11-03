#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501

#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define Buffer 512
#define def_port "5357"

unsigned int conn(char *str) 
{
    WSADATA wsaData;
    int res;

    SOCKET ListenSocket = INVALID_SOCKET;//listen socket is similar to connectsocket variable in client except in that we try for connection in this we listen for conection attempt.
    SOCKET C_sock = INVALID_SOCKET;//temp variable to store client socket info

    struct addrinfo *result = NULL,
    hints;

    int s_res;
    char rxbuff[Buffer];
    int rxbufflen = Buffer;
    
    
    res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (res != 0) {
        printf("WSAStartup failed with error: %d\n", res);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;//passive means for server

    
    res = getaddrinfo(NULL, def_port, &hints, &result);
    if ( res != 0 ) {
        printf("addr failed with error: %d\n", res);
        WSACleanup();
        return -1;
    }

    
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);//listening for socket connection.
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    
    res = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);// binding the socket(listening for tcp socket)
    if (res == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return -1;
    }

    freeaddrinfo(result);

    res = listen(ListenSocket, SOMAXCONN);//syntax for listen is (socket , backlog); somaxconn refers to maximum no. of connections in queue(unaccepted).
    if (res == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    
    C_sock = accept(ListenSocket, NULL, NULL);//accept socket connection attempt. syntax of accept is (socket,optional ,optional); returns 0 for no error and socket_error in case of error.
    if (C_sock == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return -1;
    }
    
    closesocket(ListenSocket);
    printf("WORD HAS BEEN SENT\n");
    res = recv(C_sock, rxbuff, rxbufflen, 0);
    s_res = send( C_sock, str, res, 0 );
    
    res = shutdown(C_sock, SD_SEND);//RX/TX shutdown
    if (res == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(C_sock);
        WSACleanup();
        return -1;
    }
    
    // closesocket(C_sock);
    // WSACleanup();

    return C_sock;
}

int main(){
    char str[100],wrong[6],guess[50];
    for (int i = 0; i < 6; i++)
    {
        wrong[i]=' ';
    }
    for (int i = 0; i < 50; i++)
    {
        guess[i]=' ';
    }
    
    
    unsigned int C_sock;
    printf("Enter word to be guessed: ");   
    scanf("%s", str);
    char *p = str;
    int i= strlen(str)+6;
    C_sock = conn(p);
    while (i>0)
    {
        recv(C_sock,guess,100,0);
        recv(C_sock,wrong,6,0);                       
        printf("\nguess: %s\n",guess);
        printf("wrong: %s\n",wrong);
        i--;
    }
    closesocket(C_sock);
    WSACleanup();
    return 0;    
}