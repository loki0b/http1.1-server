#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "parser.h"

#define HOST "127.0.0.1"
#define PORT 8081

int main() {
	int sockfd, clientfd;
	pid_t pid;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		perror("(socket)");
	
	// Address information
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
	};
	inet_pton(AF_INET, HOST, &addr.sin_addr);
	
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		perror("(bind)");

	if (listen(sockfd, 10) == -1) //backlog
		perror("(listen)");
	
	struct sockaddr_in client_addr;
	socklen_t sock_client_size = sizeof(client_addr);
	if ((clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &sock_client_size)) == -1)
			perror("(accept)");

	char req[16];
	read(clientfd, req, 16);
	
	struct request_line* rl = parse_request_line(req, 16);
	write(clientfd, rl->method, 3);

	close(clientfd);
	close(sockfd);

	return 0;
}
