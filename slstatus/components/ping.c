// C program to Implement Ping 
// https://www.geeksforgeeks.org/ping-in-c/
// https://echorand.me/posts/my-own-ping/
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
#include <signal.h>
#include <netinet/ip_icmp.h> 
#include <time.h> 
#include <fcntl.h> 
#include <time.h> 
#include <errno.h>

#include "../util.h"

// Define the Packet Constants 
// ping packet size 
#define PING_PKT_S 64 

// Automatic port number 
#define PORT_NO 0 

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

int ping_sockfd; 
char *ping_ip_addr; 
struct sockaddr_in ping_addr_con; 

int ping_ttl_val=64, ping_msg_count=0, ping_addr_len; 
long ping_count=0, ping_errors_count=0;

int get_ping_sockfd() {
	return ping_sockfd;
}

const char* ping_errors() {
	if (ping_count == 0)
		return "";
	return bprintf("%.2f%%", (double)ping_errors_count/(double)ping_count*100.0);
}

const char* ping(char* hostname) {

	ping_count++;
	if (ping_ip_addr == NULL){
		ping_ip_addr = dns_lookup(hostname, &ping_addr_con); 
	}
	
	if (ping_sockfd <= 0) {
		ping_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP); 
		if(ping_sockfd<0) {
			ping_errors_count++;
			return strerror( errno );
		}
	}
	
	struct timespec time_start, time_end; 
	struct sockaddr_in r_addr;
	struct ping_pkt pckt;
	struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;
	int i=0;
	double timeElapsed;

	// set socket options at ip to TTL and value to 64, 
	// change to what you want by setting ttl_val 
	int a = setsockopt(ping_sockfd, SOL_IP, IP_TTL,	&ping_ttl_val, sizeof(ping_ttl_val));
	if (a != 0) {
		ping_errors_count++;
		return strerror(errno);
	}
	//
	// setting timeout of recv setting 
	setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out); 

	//filling packet 
	bzero(&pckt, sizeof(pckt)); 
	
	pckt.hdr.type = ICMP_ECHO; 
	pckt.hdr.un.echo.id = getpid(); 
	
	for ( i = 0; i < sizeof(pckt.msg)-1; i++ ) 
		pckt.msg[i] = i+'0'; 
	
	pckt.msg[i] = 0; 
	pckt.hdr.un.echo.sequence = ping_msg_count++; 
	pckt.hdr.checksum = checksum(&pckt, sizeof(pckt)); 


	clock_gettime(CLOCK_MONOTONIC, &time_start); 
	if ( sendto(ping_sockfd, &pckt, sizeof(pckt), 0, &ping_addr_con, sizeof(ping_addr_con)) <= 0) { 
		ping_errors_count++;
		return strerror( errno );
	} 

	//receive packet 
	ping_addr_len=sizeof(r_addr); 

	if ( recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &ping_addr_len) <= 0 && ping_msg_count>1) { 
		ping_errors_count++;
		return strerror( errno );
	} else {
	
		clock_gettime(CLOCK_MONOTONIC, &time_end); 
			
		timeElapsed = ((double)(time_end.tv_nsec - 
								time_start.tv_nsec))/1000000.0; 

		/* printf("\r\n%d, %d, %d, %d, %.2f\r\n", pckt.hdr.type, pckt.hdr.code, count, errors, timeElapsed); */
		if (timeElapsed < 0) {
			ping_errors_count++;
			timeElapsed = 0;
		}
		
		/* if(!(pckt.hdr.type ==69 && pckt.hdr.code==0)) */
		/*  	return "ERR5"; */
	}

	return bprintf("%.1f", timeElapsed);
}
