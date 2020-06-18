// C program to Implement Ping 
// https://www.geeksforgeeks.org/ping-in-c/
// compile as -o ping 
// run as sudo ./ping <hostname> 

#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netinet/ip_icmp.h> 
#include <time.h> 
#include <fcntl.h> 
#include <time.h> 

#include "../util.h"

// Define the Packet Constants 
// ping packet size 
#define PING_PKT_S 64 

// Automatic port number 
#define PORT_NO 0 

// Automatic port number 
#define PING_SLEEP_RATE 1000000 

// Gives the timeout delay for receiving packets 
// in seconds 
#define RECV_TIMEOUT 1 

// Define the Ping Loop 
int pingloop=1; 


// ping packet structure 
struct ping_pkt 
{ 
	struct icmphdr hdr; 
	char msg[PING_PKT_S-sizeof(struct icmphdr)]; 
}; 

// Calculating the Check Sum 
unsigned short checksum(void *b, int len) 
{ unsigned short *buf = b; 
	unsigned int sum=0; 
	unsigned short result; 

	for ( sum = 0; len > 1; len -= 2 ) 
		sum += *buf++; 
	if ( len == 1 ) 
		sum += *(unsigned char*)buf; 
	sum = (sum >> 16) + (sum & 0xFFFF); 
	sum += (sum >> 16); 
	result = ~sum; 
	return result; 
} 


// Performs a DNS lookup 
char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con) 
{ 
	struct hostent *host_entity; 
	char *ip=(char*)malloc(NI_MAXHOST*sizeof(char)); 

	if ((host_entity = gethostbyname(addr_host)) == NULL) 
	{ 
		// No ip found for hostname 
		return NULL; 
	} 
	
	//filling up address structure 
	strcpy(ip, inet_ntoa(*(struct in_addr *) 
						host_entity->h_addr)); 

	(*addr_con).sin_family = host_entity->h_addrtype; 
	(*addr_con).sin_port = htons (PORT_NO); 
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr; 

	return ip; 
	
} 

int sockfd; 
char *ip_addr; 
struct sockaddr_in addr_con; 
double timeElapsed;

int ttl_val=64, msg_count=0, i, addr_len; 

char* ping(char* hostname) {

	if (ip_addr == NULL)
		ip_addr = dns_lookup(hostname, &addr_con); 
	
	if (sockfd <= 0) {
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP); 
		if(sockfd<0)
			return "ERR1";
	}
	
	struct timespec time_start, time_end; 
	struct sockaddr_in r_addr;
	struct ping_pkt pckt;
	struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;

	// set socket options at ip to TTL and value to 64, 
	// change to what you want by setting ttl_val 
	int a = setsockopt(sockfd, SOL_IP, IP_TTL,	&ttl_val, sizeof(ttl_val));
	if (a != 0) 
		return "ERR2"; 
	//
	// setting timeout of recv setting 
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out); 

	//filling packet 
	bzero(&pckt, sizeof(pckt)); 
	
	pckt.hdr.type = ICMP_ECHO; 
	pckt.hdr.un.echo.id = getpid(); 
	
	for ( i = 0; i < sizeof(pckt.msg)-1; i++ ) 
		pckt.msg[i] = i+'0'; 
	
	pckt.msg[i] = 0; 
	pckt.hdr.un.echo.sequence = msg_count++; 
	pckt.hdr.checksum = checksum(&pckt, sizeof(pckt)); 


	usleep(PING_SLEEP_RATE); 

	clock_gettime(CLOCK_MONOTONIC, &time_start); 
	if ( sendto(sockfd, &pckt, sizeof(pckt), 0, &addr_con, sizeof(addr_con)) <= 0) { 
		return "ERR3";
	} 

	//receive packet 
	addr_len=sizeof(r_addr); 

	if ( recvfrom(sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len) <= 0 && msg_count>1) { 
		return "ERR4";
	} else {
	
		clock_gettime(CLOCK_MONOTONIC, &time_end); 
			
		timeElapsed = ((double)(time_end.tv_nsec - 
								time_start.tv_nsec))/1000000.0; 
		if (timeElapsed < 0) timeElapsed = 0;
		
		/* if(!(pckt.hdr.type ==69 && pckt.hdr.code==0)) */ 
		/* 	return "ERR5"; */
	}

	return bprintf("%.1f", timeElapsed);
}
