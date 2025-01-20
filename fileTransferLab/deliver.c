#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFERSIZE 4096


const char* is_valid_ip(const char *arg)
{
    struct sockaddr_in server_addr;
    if (inet_pton(AF_INET, arg, &(server_addr.sin_addr)))
    {
	return arg;
    } 
    else 
    {
	return NULL;
    }
} 


unsigned short is_valid_port(const char *arg)
{
    // Check if the argument is numeric
    for (int i = 0; arg[i] != '\0'; ++i)
    {
	if (!isdigit(arg[i]))
	{
	    return 0; // Return 0 is not a valid number
	}
    }

    int port = atoi(arg);

    // Check if port is in the valid range
    if (port <= 0 || port > 65535)
    {
	return 0; // Invalid port number
    }

    return (unsigned short) port;
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
	perror("Error: Invalid number of arguments. Expected 2 arguments.\n");
	printf("Correct usage: deliver <server address> <server port number>\n");
	return 1;
    }

    const char *server_ip;
    
    if ((server_ip = is_valid_ip(argv[1])) == NULL)
    {
	perror("Error: Invalid IP address.");
	return 1;
    }

    unsigned short port = 0;

    if ((port = is_valid_port(argv[2])) == 0)
    {
	perror("Error: Invalid port number.");
	return 1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFERSIZE];
    char filename[BUFFERSIZE];
    char message[BUFFERSIZE];
    socklen_t addr_len = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	perror("Error: Socket creation failed.");
	return 1;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert server IP to binary
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
	perror("Error: Invalid IP address.");
	close(sockfd);
	return 1;
    }

    printf("Enter command 'ftp <file name>': ");

    // Extract filename from input
    if (scanf("%s %s", message,	filename) == 2)
    {
	// Check if the file exists
	if (access(filename, F_OK) == 0)
	{
	    // Send the message to the server
	    if (sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&server_addr, addr_len) < 0)
	    {
		perror("Error: Failed to send message.");
		close(sockfd);
		return 1;
	    }
	}
	else
	{
	    printf("Error: File does not exist\n");
	    return 1;
	}
    }
    

    int n = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (n < 0)
    {
	perror("Error: Failed to receive a response.");
    }
    else
    {
	if (strcmp(buffer, "yes") == 0)
	{
	    printf("A file transfer can start.\n");
	}
	else
	{
	    perror("Error: Cannot start file transfer");
	    close(sockfd);
	    return 1;
	}
    }

    close(sockfd);
    return 0;
}
