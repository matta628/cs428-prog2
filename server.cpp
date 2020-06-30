// TCP server client simulation

// Must have this server running before you can run the TCP Client code

#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


int main(int argc, char ** argv) { 
	int port;
	int sockfd, n;
	socklen_t len;
	char acknowledge[1024];
	char message1[1024];
	char message2[1024];
	struct sockaddr_in servaddr, cliaddr1, cliaddr2; 

	// Check correct command line format
	if (argc != 2){
		printf("must be in format: ./server port_number\n");
		return -1;
	}
	
	// Set port number
	port = atoi(argv[1]);
	std::cout << port << std::endl;
	
	// Create a TCP socket
	// Notice the use of SOCK_STREAM for TCP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Initialize servaddr, cliaddr1, and cliaddr2 to 0
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr1, 0, sizeof(cliaddr1));
	memset(&cliaddr2, 0, sizeof(cliaddr2));
	
	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(port); // port number
	
	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("bind error\n");
		return -1;
	}

	// Listen for any potential connections
	if (listen(sockfd, 2) < 0){
		printf("listen error\n");
		return -1;
	}

	// Accept first connection
	int newsockfd1 = accept(sockfd, (struct sockaddr *) &cliaddr1, &len);
	if (newsockfd1 < 0){
		printf("first accept error\n");
		return -1;
	}
	
	// Receive the first client message along with the address it is coming from
	n = recv(newsockfd1, (char *)message1, sizeof(message1), 0);
	if (n < 0){
		printf("first recv error\t\t");
		return -1;
	}
	message1[n] = '\0';

	// Print first message
	std::cout << message1 << std::endl;

	// Accept second connection
	int newsockfd2 = accept(sockfd, (struct sockaddr *) &cliaddr2, &len);
	if (newsockfd2 < 0){
		printf("second accept error\n");
		return -1;
	}
	
	// Receive the second client message along with the address it is coming from
	n = recv(newsockfd2, (char *)message2, sizeof(message2), 0);
	if (n < 0){
		printf("second recv error\n");
		return -1;
	}
	message2[n] ='\0';

	// Print second message
	std::cout << message2 << std::endl;

	// Build acknowledgement message & parse client IDs
	std::string msg1(message1);
	std::string id1 = msg1.substr(7, msg1.find(':')-7);
	std::string msg2(message2);
	std::string id2 = msg2.substr(7, msg2.find(':')-7);
	msg1 = msg1.substr(7) + " received before " + msg2.substr(7);
	strcpy(acknowledge, msg1.c_str());
	
	// Send acknowledgment to first client
	if (send(newsockfd1, (const char *)acknowledge, strlen(acknowledge), 0) < 0){
		printf("first send error\n");
		return -1;
	}

	// Send acknowledgment to second client
	if (send(newsockfd2, (const char *)acknowledge, strlen(acknowledge), 0) < 0){
		printf("second send error\n");
		return -1;
	}
	std::cout << "Sent acknowledgements to both " << id1 << " and " << id2  << std::endl;

	// Shutdown socket
	if (shutdown(sockfd, SHUT_RDWR) < 0){
		printf("shutdown error\n");
		return -1;
	}
	
	return 0; 
}
