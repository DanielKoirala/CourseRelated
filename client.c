#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
#include <unistd.h>
#include<arpa/inet.h>
#include <time.h>

//modify buffer
int buffer_size = 750;
char buff[750];

void func(int sockfd)
{
	bzero(buff, sizeof(buff));
	//read from file.txt
	FILE *fp = fopen("file.txt", "r");
	if(fp == NULL) {
		printf("Cannot open file\n");
		exit(0);
	}
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	printf("File size: %d bytes\n", file_size);

	int n;

	clock_t t;
	//start clock
	t = clock();
	while ((n = fread(buff, sizeof(char), buffer_size, fp))>0) {
		write(sockfd, buff, n);
	}
	//end clock
	t = clock() -t;

	double time_taken = ((double)t)/ CLOCKS_PER_SEC ;

	printf("Buffer size %d: %f seconds\n", buffer_size, time_taken);
	fclose(fp);
	printf("file sent\n");
	
}


int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}