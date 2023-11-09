#define WIN32_MEAN_AND_LEAN //prevents winsock.h to be included. we only want winsock 2.
#define _WIN32_WINNT 0x0501

#include<stdio.h>
#include<windows.h>
#include<winsock2.h>//socket header.
#include<ws2tcpip.h>//functions and structures
#include<string.h>
//pragma is used for linking libraries
#pragma comment(lib,"Ws2_32.lib") //Winsock Library
#pragma comment(lib,"Mswsock.h")//functions for networking
#pragma comment(lib,"AdvApi32.lib")//registry access

#define Buffer 512 //buffer size for RX/TX
#define def_port "25787" // port for communication

void red () {
  printf("\033[1;31m");
}

void green (){
  printf("\033[1;32m");
}

void yellow () {
  printf("\033[1;33m");
}

void blue () {
  printf("\033[1;34m");
}

void magenta (){
  printf("\033[1;35m");
}

void cyan (){
  printf("\033[1;36m");
}

void reset () {
  printf("\033[0m");
}

int strlower(char str[],int len){
    for (int i = 0; i < len; i++)
    {
        if (str[i]>=65 && str[i]<=90)
        {
            str[i]+=32;
        }
    }
}

int search(char str[], char f, int len, int x[]) {
    int p = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == f) {
            x[p++] = i;
        }
    }
    return p;
}

void change(int x[], char g[], int c, char str[]) {
    for (int i = 0; i < c; i++) {
        int z = x[i];
        g[z] = str[z];                        
    }    
}

void printBody(int mistakes) {  
    printf("\tMistakes :%d\n", 6-mistakes);        
        
    switch(mistakes) {  

        case 6: magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|           \n"
           "\t|         \n"
           "\t|            \n"
           "\t|             \n"  
           "\t|             \n"); 
           reset();
           break;  

        case 5: magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )  \n"
           "\t|             \n"
           "\t|            \n"
           "\t|             \n"  
           "\t|             \n"); 
           reset();
           break;                         
        case 4: magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )  \n"
           "\t|         |    \n"
           "\t|            \n"
           "\t|             \n"  
           "\t|             \n"); 
           reset();
            break;
        case 3: magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )   \n"
           "\t|        \\|    \n"
           "\t|            \n"
           "\t|             \n"  
           "\t|             \n"); 
           reset();
           break;
        case 2:magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )   \n"
           "\t|        \\|/   \n"
           "\t|         |   \n"
           "\t|             \n"  
           "\t|             \n"); 
           reset();
           break;
        case 1: magenta();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )   \n"
           "\t|        \\|/   \n"
           "\t|         |   \n"
           "\t|        /    \n"  
           "\t|             \n"); 
           reset();
            break;
        case 0: red();
        printf("\t _________\n"  
           "\t|         |\n"  
           "\t|        ( )   \n"
           "\t|        \\|/   \n"
           "\t|         |   \n"
           "\t|        / \\    \n"  
           "\t|             \n"); 
           reset();
           break;
        
        default: break;  
  
    }   
}  

unsigned int conn(int arg,char **argv,char *sendm,char str[])//__cdecl is function calling keyword
{//arg is an int which has number of arguments passed and argv ins array of pointers that store arguments(file name ,host etc.. )
    WSADATA wsaData; //initiates use of winsock dll. contains socket information calling functions and values.
    int res; // variable
    char rxbuf[Buffer]; // creating a buffer to RX.          
    int rxbuflen=Buffer; // defining buffer size
    SOCKET ConnectSocket = INVALID_SOCKET; //initializing connectsocket variable with initial value invalid
    struct addrinfo *result=NULL, // addrinfo structure having hints (aka structure with connection info)
    *p = NULL,
    hints;

    ZeroMemory(&hints,sizeof(hints));//initialises memory buffer which sets all bytes in hint(aka structure with connection info) to 0 // it is a forced cleanup of all values in hint.
    hints.ai_family=AF_INET;//address family ipv4 // AF_inet 6 for ipv6 and _unspec for unspecified.
    hints.ai_socktype=SOCK_STREAM;//socket connection info (constant)
    hints.ai_protocol=IPPROTO_TCP;// tcp /ip protocol

    res = WSAStartup(MAKEWORD(2,2),&wsaData);//makeword macro converts 2 8bit values to 16bit. wsa startup is a inbuilt fn in winsock which returns 0 for success other wise error msg.
    if (res != 0){
        printf("WSAStartup failed with error: %d\n", res);
        return -1;
    }

    res=getaddrinfo(argv[1],def_port,&hints,&result);    //argv[1] contains host(ex localhost)
    if(res != 0){
        printf("addr failed ..  .. ");
        WSACleanup();//terminating winsock dll
        return 1;
    }
    blue();
    printf("\nConnecting .. .. ..\n");
    reset();
    for(p=result;p != NULL ; p->ai_next){
        ConnectSocket=socket(p->ai_family,p->ai_socktype,p->ai_protocol);//attempt to connect to all sockets untill one is correct
        if (ConnectSocket == INVALID_SOCKET){
            printf("Error at socket(): %ld\n", WSAGetLastError());    //wsagetlast error returns error number        
            WSACleanup();//terminating winsock dll
            return -1;
        }
        res = connect( ConnectSocket, p->ai_addr, (int)p->ai_addrlen);// attempting to connect to selected socket
        if (res == SOCKET_ERROR) {
            closesocket(ConnectSocket);//cant use res since redifined res as the output of connect(). .close used for closing socket(aka disconnecting)
            ConnectSocket = INVALID_SOCKET;
            continue;
        }       
        break;
    }
    cyan();
    printf("\nConnection Has Been Established\n");
    reset();
    freeaddrinfo(result);//freeing the addrinfo memoryr̥

    if (ConnectSocket==INVALID_SOCKET){//no socket found
        printf("Conncetion failed \n");
        WSACleanup();//terminating winsock dll
        return -1;
    }

    send(ConnectSocket,sendm,(int)strlen(sendm),0);//sends the msg sendm to server   
    res=recv(ConnectSocket,rxbuf,rxbuflen,0); 
    rxbuf[rxbuflen]= '\0';                    
    strcpy(str,rxbuf);
    // shutdown(ConnectSocket, SD_SEND); 
    return ConnectSocket;
}

int main(int arg,char **argv){

    yellow();
    printf("\n\t Be aware you can be hanged!!.");  
  
    printf("\n\n\t Rules : ");  
    printf("\n\t - Maximum 6 mistakes are allowed.");  
    printf("\n\t - All alphabet are in lower case.");      
    printf("\n\t - If you enjoy continue, otherwise close it.");  
  
    printf("\n\t Syntax : Alphabet");  
    printf("\n\t Example : a \n\n");  
    reset();

    int len, i;
    char str[100],wrong[7];
    char *p=wrong;
    char f[]="this worked\n";
    unsigned int ConnectSocket = conn( arg, argv,f,str);      
    len = strlen(str);    
    strlower(str,len);
    char guess[len];
    for (i = 0; i < len; i++) {
        guess[i] = '_';
    }    
    guess[len] = '\0'; 
    char char_g;
    int tries = 6; 
    blue();
    printf("\n\nYou Have To Guess : %s\n",guess) ;
    printf("\nThe Numbers of Letters in the Word : %d\n",len); 
    printf("\nYOU HAVE 6 TRIES !! TREAD CAREFULLY.\n"); 
    reset();
    while (tries--) {
        if(!strcmp(guess,str)){
            green();
            printf("\n\t***CONGRATULATIONS*** !!!\n\tYOU HAVE SURVIVED THE GALLOWS !!! ");
            reset();
            closesocket(ConnectSocket);
            WSACleanup();
            return 0;
        }
        yellow();
        printf("\nGuess a letter: ");
        scanf(" %c", &char_g);
        reset();

        if (char_g>=65 && char_g<=90)
        {
            char_g+=32;
        }

        int x[len];
        int c = search(str, char_g, len, x);
        change(x, guess, c, str);
        if(c){
            tries++;
        } 
        else{*p=char_g;p++;} 
        *p ='\0';      
        printBody(tries);         
        red();
        printf("Wrong Letters: %s\n",wrong);       
        reset();
        green();
        printf("\n%s\n", guess);
        reset();
        send(ConnectSocket,guess,100,0);
        send(ConnectSocket,wrong,7,0);               
    }
    red();
    printf("The word was : %s\n",str);    
    printf("\n\tYOU HAVE BEEN HANGED !!!\n\tBETTER LUCK NEXT LIFE\n"); 
    reset();
    closesocket(ConnectSocket);
    WSACleanup();   
    return 0;
}

