#include <stdio.h>
#include <stdlib.h>


#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>
#include <fcntl.h>


#include <linux/ip.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/udp.h>


#include <assert.h>
#include <net/if.h>
#include <netdb.h>
#include <stdint.h>
#include <poll.h>
#include <arpa/inet.h>


#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>





void get_walltime_(double* wcTime) 
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}



// global variables needed so that signal handler can access them
int sock_;
struct tpacket_stats_v3 sum_;
long packets_in  = 0, bytes_in = 0;
double start_,end_;
//struct pollfd *pfd;


// signal handler for cntrl c signal
static void sighandler(int num_)
{
   get_walltime_(&end_);
   int size_ = sizeof(struct tpacket_stats_v3);

   getsockopt(sock_, SOL_PACKET, PACKET_STATISTICS, &sum_, &size_);
   printf("%d packets in, %d bytes in during time %f\n", sum_.tp_packets, bytes_in,end_-start_);
   exit(num_);
}



//r_ contains the configs of the buffer (number of blocks, frames, and sizes
// memory_ holds the virtual memory address of the mapping
struct buf
{
struct iovec *io;
int *memory_;
struct tpacket_req3 r_;
};


struct block_ {
int val_1;
int val_2;
struct tpacket_hdr_v1 h1;
};


int main(int argc, char **argv)

{

get_walltime_(&start_);

socklen_t len;
struct buf buf1;


if (argc > 2 ||  argc < 2)
{
printf("Error... \n");
return 1;
}


signal(SIGINT, sighandler);

memset(&buf1, 0, sizeof(buf1));




struct sockaddr_ll addr_;
const int packet_version = TPACKET_V3;
//read network interface device string
char *net_inter = argv[argc - 1];

// raw socket create
sock_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

setsockopt(sock_, SOL_PACKET, PACKET_VERSION, &packet_version, sizeof(packet_version));


memset(&buf1.r_, 0, sizeof(buf1.r_));


// configuration parameters of the buffer
buf1.r_.tp_block_nr = 64;
buf1.r_.tp_block_size = 4194304;

buf1.r_.tp_frame_size = 2048;
buf1.r_.tp_frame_nr = 131072;

buf1.r_.tp_feature_req_word = TP_FT_REQ_FILL_RXHASH;

buf1.r_.tp_retire_blk_tov = 60;
setsockopt(sock_, SOL_PACKET, PACKET_RX_RING, &buf1.r_, sizeof(buf1.r_));


// memory mapping shared buffer
buf1.memory_ = mmap(NULL, buf1.r_.tp_block_size * buf1.r_.tp_block_nr, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, sock_, 0);


// scatter gather io vectors for each block
buf1.io = malloc(buf1.r_.tp_block_nr * sizeof(*(buf1.io)));

for (int i=0;i<buf1.r_.tp_block_nr;i++)
{
buf1.io[i].iov_base = buf1.memory_ + (i * buf1.r_.tp_block_size);
buf1.io[i].iov_len = buf1.r_.tp_block_size;
}


memset(&addr_, 0, sizeof(addr_));
addr_.sll_family = PF_PACKET;
addr_.sll_protocol = htons(ETH_P_ALL);
addr_.sll_ifindex = if_nametoindex(net_inter);

bind(sock_, (struct sockaddr *) &addr_, sizeof(addr_));



unsigned int blockn_ = 0;


struct pollfd *poll_ = ( void *)malloc(sizeof(struct pollfd));

// to do:    
//polling to check if blocks can be read


memset(poll_, 0, sizeof(*poll_));

poll_->revents = 0;
poll_->fd = sock_;

poll_->events = POLLIN | POLLERR;



struct block_ *p1 = (struct block_ *) buf1.io[blockn_].iov_base;

while (1)
{


// check if we can read the block
if (( ((struct block_ *) buf1.io[blockn_].iov_base)->h1.block_status & TP_STATUS_USER) == 0)
{

// timeout specified to be -1 ; poll blocks until there is something to be read
poll(poll_, 1, -1);

// run loop again ; something to read
continue;
}



p1 = (struct block_ *) buf1.io[blockn_].iov_base;

int num_pkts = p1->h1.num_pkts;
long bytes = 0;
struct tpacket3_hdr *p2;

p2 = (struct tpacket3_hdr *) ((uint8_t *) p1 + p1->h1.offset_to_first_pkt);
for (int i = 0; i < num_pkts; ++i) {

bytes += p2->tp_snaplen;

// move to next packet
p2 = (struct tpacket3_hdr *) ((uint8_t *) p2 + p2->tp_next_offset);
}

packets_in += num_pkts;
bytes_in += bytes;


((struct block_ *) (buf1.io[blockn_].iov_base))->h1.block_status = TP_STATUS_KERNEL;

blockn_++;
blockn_ = blockn_ % 64;
}


return 0;
  
}
