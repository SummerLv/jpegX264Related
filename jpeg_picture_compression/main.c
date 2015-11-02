#include <string.h>
#include <stdlib.h>
#include "v4l2_use.h"
#include "lcd.h"
#include <string.h>


#ifndef TCP_SEND
#define TCP_SEND 1
#endif

#define BUFFER_SIZE		1024
unsigned char sendBuffer[BUFFER_SIZE] = {0};

//   SEND FUNCTION ---- UDP
int  sendFileName(const char * filename, int filenameLen ,int sockfd, struct sockaddr_in serverAddr);
int  sendPicture(const char * filename ,int sockfd, struct sockaddr_in serverAddr);
//   SEND FUNCTION --- TCP
int  sendFileName_TCP(const char * filename, int filenameLen ,int sockfd);
int  sendPicture_TCP(const char * filename ,int sockfd);


int main(int argc,char *argv[])
{
	// printf("jpeg compress like this :"
	// 		"use  ./jpegAPP quality[0--100] \n");
	

	/*****************************************************
	*  init the filename to store the compressed picture
	******************************************************
	*/
	//char *filename = malloc(50);
	char filename[50] = "compress_";
	if(2 == argc){
		strcat(filename,argv[1]);
		strcat(filename,".jpeg");
		puts(filename);
	}
	else{
		printf("the argv[1] is not the quality parameter");
	}

	int quality = atoi(argv[1]);

	/*******************************
	*  init the udpSend
	********************************
	*/
	unsigned short port = 6666;//udp 初始化
	char *ipstr = "192.168.5.166";
	
	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > sockfd){
		perror("socket()");
		return -1;
	}

	int sockfdTCP = socket(PF_INET , SOCK_STREAM, 0);
	if (sockfdTCP < 0){
		perror("TCP socket()");
		return -1;
	}
	
	struct sockaddr_in serverAddr = {
		.sin_family = PF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = inet_addr(ipstr),
	};	
	memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));	
	printf("----------- udp parameter inited --------------\n");
	
	

	/*===================================================
	*  start to fecth the destined picturn & comprass
	*=====================================================*/

	v4l2_use.open();

	v4l2_use.read( filename, quality);
	printf("compressed jpeg.picture -------- waiting for  sending \n");
	
	// *******  send fileName & picture data to the UDP Server ******
	// sendFileName( filename,  strlen(filename), sockfd, serverAddr);
	// sendPicture( filename, sockfd, serverAddr);

	// send fileName & picture data to the TCP Server
	if( connect(sockfdTCP, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		perror("connect ()");
	}
	printf("----------- TCP connect --------------\n");

	sendFileName_TCP( filename,  strlen(filename), sockfdTCP);
	sendPicture_TCP( filename, sockfdTCP);


	v4l2_use.close(lcd_dev_parmet);	
	

  	
}



/*
*	@para  filename    : send file name
*   @para  filenameLen : file name length
*   @return  		   : succecced send bytes
*/
int  sendFileName(const char * filename, int filenameLen,
				   int sockfd, struct sockaddr_in serverAddr
				){
	printf("in the function sendFileName\n");
	socklen_t addrLen = sizeof(struct sockaddr_in);
	int sendSize = sendto(sockfd, filename, filenameLen, 0, 
						(struct sockaddr *) &serverAddr, addrLen);
	if ( sendSize <= 0 ){
		perror("sendFileName");
	}

	return sendSize;
}
/*
*	@para  filename    : send file name
*   @para  ...........
*   @return  		   : succecced send bytes
*/
int  sendPicture(const char * filename, int sockfd, struct sockaddr_in serverAddr){
	
	printf("in the function sendPicture\n");

	int sendSize = 0;
	int readSize = 0;

	socklen_t addrLen = sizeof(struct sockaddr_in);
	memset(sendBuffer, 0, BUFFER_SIZE);
	FILE *fp = fopen(filename, "r");
	printf("file transfer in UDP is going");
	while((readSize = fread(sendBuffer, 1, BUFFER_SIZE,fp)) > 0){
		printf("fread: readSize     %d      Byte\n",readSize);
		sendSize = sendto(sockfd, sendBuffer, readSize, 0, 
						(struct sockaddr *) &serverAddr, addrLen);
		if( sendSize < 0){
			perror("send size < 0");
			exit(0);
		}
		// 是否需要加
		 memset(sendBuffer, 0, BUFFER_SIZE);
	}
	printf("out of the while  in  sendPicture()\n");
	fclose(fp);

	sendto(sockfd, "SENDFINISHED", strlen("SENDFINISHED"), 0, 
		     (struct sockaddr *) &serverAddr, addrLen);

	printf("file transfer in UDP is closing");

}


//================================================================================
/*
*	@para  filename    : send file name
*   @para  filenameLen : file name length
*   @return  		   : succecced send bytes
*/
int  sendFileName_TCP(const char * filename, int filenameLen,int sockfd){
	printf("in the function sendFileName\n");
	socklen_t addrLen = sizeof(struct sockaddr_in);
	int sendSize = send(sockfd, filename, filenameLen, 0);
	if ( sendSize <= 0 ){
		perror("sendFileName");
	}

	return sendSize;
}
/*
*	@para  filename    : send file name
*   @para  ...........
*   @return  		   : succecced send bytes
*/
int  sendPicture_TCP(const char * filename, int sockfd){
	
	printf("in the function sendPicture\n");

	int sendSize = 0;
	int readSize = 0;

	socklen_t addrLen = sizeof(struct sockaddr_in);
	memset(sendBuffer, 0, BUFFER_SIZE);
	FILE *fp = fopen(filename, "r");
	printf("file transfer in UDP is going");
	while((readSize = fread(sendBuffer, 1, BUFFER_SIZE,fp)) > 0){
		printf("fread: readSize     %d      Byte\n",readSize);
		sendSize = send(sockfd, sendBuffer, readSize, 0);
		if( sendSize < 0){
			perror("send size < 0");
			exit(0);
		}
		// 是否需要加
		 memset(sendBuffer, 0, BUFFER_SIZE);
	}
	printf("out of the while  in  sendPicture()\n");
	fclose(fp);

	send(sockfd, "SENDFINISHED", strlen("SENDFINISHED"), 0);

	printf("file transfer in UDP is closing");

}


