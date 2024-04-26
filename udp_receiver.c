#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

int bytes_in=1;
double start_;
double end_;

void get_walltime_(double* wcTime) {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}



//handling sigint (cntrl c) when terminating process
void sig_handle(int num_) {

    printf("bytes in %d \n", bytes_in);
    get_walltime_(&end_);
    printf("time %f \n", end_-start_);
    exit(num_);
}



int main(int argc, char *argv[])
{

get_walltime_(&start_);

signal(SIGINT,sig_handle);
//if(sig_ ==SIG_ERR)
//{ printf("erorr \n"); exit(1);}

int sock_ = socket(AF_INET, SOCK_DGRAM,0);
if(sock_<0)
{ printf("Error creating socket \n"); exit(1);}

//serv_addr and clnt_addr only pointers ; hold addressses
//need to malloc to allocate memory on heap to hold the
//structures
struct sockaddr_in *serv_addr;
struct sockaddr_in *clnt_addr;

serv_addr = (void *)malloc(sizeof(struct sockaddr_in));
clnt_addr = (void *)malloc(sizeof(struct sockaddr_in));

//sanity check 
printf("%d \n", sizeof(struct sockaddr_in) ==sizeof(*serv_addr));


memset(serv_addr,0,sizeof(*serv_addr));
memset(clnt_addr,0,sizeof(*clnt_addr));

//inserting information ; interface and port of server
serv_addr->sin_family = AF_INET;
serv_addr->sin_addr.s_addr = INADDR_ANY;
serv_addr->sin_port = htons(60000);


// need to cast serv_addr (holds address of struct sockaddr_in) to
// sockaddr
int r_ = bind(sock_, (struct sockaddr *)serv_addr, sizeof(*serv_addr));
if(r_<0)
{printf("binding failed \n"); exit(1);}


int l_ = sizeof(*clnt_addr);

int size_ = 3000;
char *message_ = (void *)malloc(size_*(sizeof(char)));

while(1)
{
r_ = recvfrom(sock_,message_,size_, 0, (struct sockaddr *)clnt_addr,&l_);
bytes_in += r_;
}
//don't worry about endianess ; since sending char datatypes (each 1 byte) so doesn’t matter
//also little endian machines on both sides


printf("%s \n",message_);

int c_ = close(sock_);

free(serv_addr);
free(clnt_addr);

return 0;

}
