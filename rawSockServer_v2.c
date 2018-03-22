/*  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.*/

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#define BUFFER_SIZE 	1024
int main(int argc, char *argv[])
{
	int sockfd, i;
	ssize_t numbytes;
	uint8_t buf[BUFFER_SIZE];
	struct ifreq ifreq_c;
	uint8_t hwMAC[6];
	/* Header structures */
	//defined in net/ethernet.h
	struct ether_header *eh = (struct ether_header *) buf;
	/* Open PF_PACKET socket, in listening mode */
	/*AF_PACKET:low level packet interface
	SOCK_RAW:raw nw prorocol access
	ETHERTYPE_IP:IP protocol
	*/
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETHERTYPE_IP))) == -1) {
		perror("socket");	
		return -1;
	}
	memset(&ifreq_c,0,sizeof(ifreq_c));
	strncpy(ifreq_c.ifr_name,"wlan0",IFNAMSIZ-1);//giving name of Interface
	
	if((ioctl(sockfd,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
	printf("error in SIOCGIFHWADDR ioctl reading");
	for (i=0; i<6; i++) hwMAC[i] = ifreq_c.ifr_hwaddr.sa_data[i]&0xFF;
	printf("DestHwMac:");
	for (i=0; i<6; i++) printf("%02x:", hwMAC[i]);
	
	while(1) {
		//printf("listener: Waiting to recvfrom...\n");
		numbytes = recv(sockfd, buf, BUFFER_SIZE,0);
		//printf("listener: got packet %lu bytes\n", numbytes);
		printf("\nRxpacketDMAC:");
		for (i=0; i<6; i++) printf("%02x:", eh->ether_dhost[i]);
		printf("\nRxpacketSMAC:");
		for (i=0; i<6; i++) printf("%02x:", eh->ether_shost[i]);
		printf("\nRxpacketType:");
		printf("%02x:", eh->ether_type);
		if((eh->ether_dhost[0] == hwMAC[0]) && (eh->ether_dhost[1] == hwMAC[1]) && 
			(eh->ether_dhost[2] == hwMAC[2]) && (eh->ether_dhost[3] == hwMAC[3]) && 
			(eh->ether_dhost[4] == hwMAC[4]) &&(eh->ether_dhost[5] == hwMAC[5])) {
        	/* Print packet */
	        printf("\nRxPayload:");
        	for (i=0; i<numbytes; i++) printf("%02x ", (buf+sizeof(struct ether_header))[i]);
	        printf("\n");
		}
		sleep(1);
	}
	close(sockfd);
	return 0;
}
