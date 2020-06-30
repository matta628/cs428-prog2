// TCP server client simulation

#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char ** argv) { 
	int sockfd;
	socklen_t len;
	char message[1024];
	char acknowledge[1024];
	struct sockaddr_in servaddr; 

	// Check for correct format
	if (argc != 5 ){
		printf("must be in format: ./client client_ID client_name server_name/ip portnumber\n");
		return -1;
	}

	// Use command line args
	char * clientID = argv[1];
	char * clientName = argv[2];
	char * serverName = argv[3];
	int port = atoi(argv[4]);
	
	// Build message to send
	std::string msg ("Client ");
	msg += clientID;
	msg.append(": ");
	msg += clientName;
	strcpy(message, msg.c_str());

	// Create a TCP socket
	// Notice the use of SOCK_STREAM for TCP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Initialize servaddr to 0
	memset(&servaddr, 0, sizeof(servaddr)); 

	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4
	struct hostent * host = gethostbyname(serverName);
	servaddr.sin_addr = *(struct in_addr*)(host->h_addr);
	servaddr.sin_port = htons(port); // port number
	
	// Connect to the server socket
	if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("connect error\n");
		return -1;
	}

	// Send message to server
	if (send(sockfd, (const char *)message, strlen(message), 0) < 0){
		printf("send error\n");
		return -1;
	}
		
	// Receive acknowledgement from server
	int rec = recv(sockfd, (char *)acknowledge, sizeof(acknowledge), 0);

	// Print Message & Response
	if (rec < 0){
		printf("recv error\n");
		return -1;
	}
	else{
		acknowledge[rec] = '\0';
		std::cout << "Message sent ==> " << message << std::endl;
		std::cout << "Reply received ==> " << acknowledge << std::endl;
	}
	
	return 0; 
}
