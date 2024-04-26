# Project_Distributed_Systems
Project Distributed Systems


2.) libPcap:

-> Compiling: 
gcc libpcap_packet_capture.c -lpcap -o pcap_capture

-> Running:
sudo ./pcap_capture

-> Can be tested by running for example a udp sender that sends packets to the machine that's running pcap_capture. Dividing number of bytes by the time returns the achieved throughput of the application. A signal handler for SIGINT is used to report number of bytes and time upon receiving the signal (cntrl-z).


3.) Packet MMAP:

-> Compiling: 
gcc packet_mmap.c -o packet_mmap

-> Running:
sudo ./packet_mmap ens5

->  A signal handler for SIGINT is used to report number of bytes and time upon receiving the signal (cntrl-z). The network interface name should be passed as input (in our case it's ens5). 


4.) UDP receiver and sender:

-> Compiling:
gcc udp_receiver.c -o udp_receiver
gcc udp_sender.c -o udp_sender

-> Change source port for receiver and (destination IPv4-destination port) for sender in the source code accordingly. 
-> can run multiple senders in parallel utilizing different source ports and sending to the same destination port for traffic generation.

5.) TCP client and TCP multiprocessing server:

-> Compiling:

gcc udp_receiver.c -o udp_receiver
gcc udp_sender.c -o udp_sender

-> Change ports and IPv4 address accordingly similar to UDP receiver and sender. 

-> Experiments with the TCP protocol didn't use hole punching unlike experiments with the UDP protocol and packet capturing approaches. 


6.) 

-> NIC_throughput.sh (script to compute NIC's achieved throughput).

-> fine_tuning_parameters.sh (script to fine-tune linux network stack's configurable runtime parameters).

-> interrupts_distribution.sh (script to use smp_affinity to evenly distribute CPU cores that can execute the ISRs of the RX/TX queues' IRQs).




