#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

struct pseudo_header
{
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;

    struct tcphdr tcp;
};

unsigned short csum(unsigned short *ptr,int nbytes);
void udp_flood(const char* ip, int port, char* message, int verbose);
void tcp_flood(const char* ip, int port, char* message, int verbose);
void tcpsyn_flood(const char* ip, int port, char* message, int verbose);
