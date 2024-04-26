#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <unistd.h>

int bytes_in=1;
double start_;
double end_;

void get_walltime_(double* wcTime) 
{
struct timeval tp;
gettimeofday(&tp, NULL);
*wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}


//handling sigint (cntrl c) when terminating process
void sig_handle(int num_) 
{
printf("bytes in %d \n", bytes_in);
get_walltime_(&end_);
printf("time %f \n", end_-start_);
exit(num_);
}



void my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

int cnt_ = 0;

int main(int argc, char **argv)
{

get_walltime_(&start_);

signal(SIGINT,sig_handle);

char *device;
char *error_buffer;

error_buffer = (char *)malloc(sizeof(char)*PCAP_ERRBUF_SIZE);

device = pcap_lookupdev(error_buffer);

if(device==NULL){printf("error finding device \n"); return 1;}

printf("Network device found %s \n",device);





struct pcap_pkthdr packet_header;
pcap_t *handle;
const u_char *packet;
int packet_count_limit = 100;
int timeout_limit = 100;


//open interface ens5 ; for capturing
handle = pcap_open_live(device,BUFSIZ,packet_count_limit, timeout_limit, error_buffer);
if(handle==NULL){printf("problem \n");}


// applying berkely filter to get udp packets that have destination port 60000
char filter_exp[] = "udp dst port 60000";
struct bpf_program fp;
if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return(2);
}

if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return(2);
}



pcap_loop(handle,0,my_packet_handler,NULL);



print_packet_info(packet, packet_header);


return 0;

}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header)
{
  //printf("Packet %d capture length %d \n",cnt_, packet_header.caplen);
  //printf("Packet total length %d \n", packet_header.len);
  bytes_in += packet_header.caplen;
  //printf("%d \n",bytes_in);
  /*
  if(cnt_)
  { printf("%s \n",&packet[42]);      printf("%s \n",packet+42);}
  cnt_++;
  printf("Packet %d \n", cnt_);
  //if(cnt_==60000) { 
  
  if(cnt_<3){ int c = 55;}
  */
}



// for testing
void my_packet_handler(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body)
{
 print_packet_info(packet_body, *packet_header);




 return;

}










