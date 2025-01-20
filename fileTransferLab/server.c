#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFERSIZE 4096

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

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
	perror("Error: Invalid number of arguments. Expected 1 argument.\n");
	printf("Correct usage: server <UDP listen port>\n");
	return 1;
    }
    
    unsigned short port = 0;

    if ((port = is_valid_port(argv[1])) == 0)
    {
	perror("Error: Invalid port number.");
	return 1;
    }
    
    int sockfd;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
	perror("Error: Socket failed");
	return 1;
    }
    
    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
	perror("Error: Bind failed");
	close(sockfd);
	return 1;
    }

    printf("UDP server is listening on port %d...\n", port);

    addr_len = sizeof(client_addr);
    
    // Recieve data
    int n = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (n < 0)
    {
	perror("Receive failed");
	close(sockfd);
	return 1;
    }

    // Send response
    if (strcmp(buffer, "ftp") == 0)
    {
	sendto(sockfd, "yes", 3, 0, (struct sockaddr *)&client_addr, addr_len);
    }
    else 
    {
	sendto(sockfd, "no", 2, 0, (struct sockaddr *)&client_addr, addr_len);
    }
    
    // Close socket
    close(sockfd);
    return 0;
}

