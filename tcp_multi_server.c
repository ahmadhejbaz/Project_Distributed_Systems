#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int port =49000;

int sock_;
int clnt_socks[400];
double start_,end_;




int count_ = 0;
size_t total_received = 0;



void get_walltime_(double* wcTime) {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}




void signal_handler(int sig) {
get_walltime_(&end_);
printf("elapsed time is: %f seconds\n", (end_ - start_));
printf("Number of bytes is: : %ld bytes\n", total_received);
exit(0);
}


void set_up()
{
struct sockaddr_in addr_;
sock_ = socket(AF_INET, SOCK_STREAM, 0);
memset(&addr_, 0, sizeof(addr_));
addr_.sin_family = AF_INET;
addr_.sin_addr.s_addr = htonl(INADDR_ANY);
addr_.sin_port = htons(port);

bind(sock_, (struct sockaddr *)&addr_, sizeof(addr_));
listen(sock_, 100);

 //strangely this has to be more than 50 if we are forking 400 clients
                            //at the same time and sending connection requests. it seems like the 
                            //
                            //connection requests of this server overflow when 400 clients are forked 
                            //at the same time and send connection requests.
}
int accept_()
{
    struct sockaddr_in clnt_addr;
    socklen_t clnt_len = sizeof(clnt_addr);
    int clnt_sock = accept(sock_, (struct sockaddr *)&clnt_addr, &clnt_len);
    clnt_socks[count_++] = clnt_sock;
    return clnt_sock;
}

void receive_(int clnt_sock)
{
char buffer[3000];
while (1)
{
ssize_t bytes_received = recv(clnt_sock, buffer, 3000, 0);
if (bytes_received <= 0) break;
total_received += bytes_received;
buffer[bytes_received] = '\0';
printf("Received: %s\n", buffer);
}
}









int main() {

struct sigaction sa;
memset(&sa, 0, sizeof(sa));
sa.sa_handler = signal_handler;
sigaction(SIGINT, &sa, NULL);

set_up();
get_walltime_(&start_);

int clnt_sock;

pid_t pid;
while ((clnt_sock = accept_()) != -1)
{
pid = fork();

if (pid == 0)
   {
   // child process
   receive_(clnt_sock);
   close(clnt_sock);
   exit(0);
   }
}

close(sock_);
return 0;
}
                                                                                                                                                       126,1         Bot
