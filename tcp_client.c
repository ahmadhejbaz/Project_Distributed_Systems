#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_IP "18.118.37.41"

int server_port = 49000;
int main() {

int sock_;
struct sockaddr_in addr_;
char buffer[3000];
char *message = "Hello World!";

sock_ = socket(AF_INET, SOCK_STREAM, 0);

memset(&addr_, 0, sizeof(addr_));
addr_.sin_family = AF_INET;
addr_.sin_port = htons(server_port);
inet_pton(AF_INET, SERVER_IP, &addr_.sin_addr);

connect(sock_, (struct sockaddr *)&addr_, sizeof(addr_));

while(1)
{
if (send(sock_, message, strlen(message), 0) < 0)
{
close(sock_);
exit(1);
}

}


close(sock_);
return 0;
}
