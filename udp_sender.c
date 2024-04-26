#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

int bytes_out = 1;
double start_;
double end_;

void get_time(double *Time)
{
struct timeval t_;
gettimeofday(&t_,NULL);
*Time = (double)(t_.tv_sec + t_.tv_usec/1000000.0);
}

//handling sigint (cntrl c) when terminating process
void sig_handler(int num_)
{
printf("bytes out %d \n", bytes_out);
get_time(&end_);
printf("time elapsed is %f \n",end_-start_);


exit(num_);
}


int main(int argc, char *argv[])
{

get_time(&start_);

signal(SIGINT, sig_handler);

int sock_ = socket(AF_INET, SOCK_DGRAM,0);

if(sock_<0)
{
printf("Error creating socket \n");
exit(1);
}


struct sockaddr_in *serv_addr, *clnt_addr;
serv_addr = (void *)malloc(sizeof(struct sockaddr_in));
clnt_addr = (void *)malloc(sizeof(struct sockaddr_in));

//setting sockaddr_in struct to all zeros by passing
//its address which serv_addr holds.



memset(serv_addr,0,sizeof(*serv_addr));
memset(clnt_addr,0,sizeof(*clnt_addr));

//our socket's information
serv_addr->sin_family = AF_INET;
serv_addr->sin_addr.s_addr = INADDR_ANY;
serv_addr->sin_port = htons(60000);


clnt_addr->sin_family = AF_INET;
clnt_addr->sin_addr.s_addr = inet_addr("18.118.37.41");
clnt_addr->sin_port = htons(60000);


int r_ = bind(sock_, (struct sockaddr *)serv_addr, sizeof(*serv_addr));
if(r_<0){printf("binding failed \n"); exit(1);}

int l_ = sizeof(*clnt_addr);

//mtu is 9000 jumbo
// try size_ = 100 to size_ = 9000 (in increments of 500)
int size_ = 100;

char *message_ = (void *)malloc(size_ *(sizeof(char)));
*message_ = 'a';

char *m_ = message_;
for(int i =0;i<size_;i++,m_++)
{
*m_ = 'a';
}

while(1)
{

r_ = sendto(sock_,message_,size_,0,(struct sockaddr *)clnt_addr,l_);

//r_ = recvfrom(sock_,message_,size_, 0, (struct sockaddr *)clnt_addr,&l_);
bytes_out += r_;

}

int c_ = close(sock_);
free(serv_addr);
free(clnt_addr);

return 0;

}
