#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
	int sockfd;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		perror("(socket)");
	
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
	};
	
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		perror("(bind)");

	if (listen(sockfd, 10) == -1) //backlog
		perror("(listen)");

	if (accept(sockfd, NULL, NULL) == -1)
		perror("(accept)");

	close(sockfd);

	return 0;
}
