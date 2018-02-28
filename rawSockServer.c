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
	while(1) {
		//printf("listener: Waiting to recvfrom...\n");
		numbytes = recv(sockfd, buf, BUFFER_SIZE,0);
		//printf("listener: got packet %lu bytes\n", numbytes);
		printf("DMAC:");
		for (i=0; i<6; i++) printf("%02x:", eh->ether_dhost[i]);
		printf("\nSMAC:");
		for (i=0; i<6; i++) printf("%02x:", eh->ether_shost[i]);
		printf("\ntype:");
		printf("%02x:", eh->ether_type);
        	/* Print packet */
	        printf("\nPayload:");
        	for (i=0; i<numbytes; i++) printf("%02x ", (buf+sizeof(struct ether_header))[i]);
	        printf("\n");
		sleep(1);
	}
	close(sockfd);
	return 0;
}
