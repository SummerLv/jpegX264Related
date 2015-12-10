/*
 * udpclient.c
 *
 *  Created on: 2015年10月29日
 *      Author: summer
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>




#define		BUFFER_SIZE  1024

int main(int argc, char *argv[]){
	/*
	 * define  varible here
	 * */

	 printf("the jpeg Server[***UDP**] in going to work\n");
	
	
	FILE 	*fp = NULL;
	int  	receiveSize = 0 ;
	int 	writeSize = 0;
	int  	sockfd = 0;
	struct sockaddr_in 	serverAddr,clientAddr;
	int 	sockaddrLen = sizeof(clientAddr);
	
	unsigned char 		buffer[ BUFFER_SIZE ] ;	

	char 	filename[50];
	char  	command [100] = "feh ";
	memset(filename, 0, 50);

	/*
	* deal with input parameter
	* */

	/*
	* initial the parameter
	* */
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(6666);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("sockfd < 0");
		exit(0);
	}

	if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		perror("bind failed");
		exit(0);
	}

	/*
	 * start to receive data here
	 * */

	// if((fp = fopen("receive.txt","wb")) == NULL){
	// 	perror("fopen failed");
	// }

	memset(buffer, 0, BUFFER_SIZE);
	while((receiveSize = recvfrom(sockfd, buffer, BUFFER_SIZE, 
		0,(struct sockaddr *)&clientAddr, &sockaddrLen) ) >0 ){

		printf("recvfrom  receiveSize :  %d\n", receiveSize);

		if ( NULL != strstr(buffer,"compress_") && receiveSize < 50 ){
			printf("receive the file name \n");
			fp = fopen(buffer,"wb");
			memcpy(filename, buffer, receiveSize);
			printf("FILE NAME :  %s\n", filename);
		}else if(NULL != strstr(buffer,"SENDFINISHED") /* && receiveSize < 50 */ ){
			printf("send file finishe , break the while \n");
			//printf("%s\n", buffer);
			break;
		}else{

			writeSize = fwrite(buffer, 1, receiveSize, fp);
			//fflush(fp);

			if (writeSize < 0){
				perror("fwrite failed");
				exit(0);
			}
			else{
				printf("fwrite  size :  %d\n", writeSize);
			}
		}
	}
	//printf("receive from client : %s   ,  port: %d \n",inet_ntop(AF_INET, serverAddr.sin_addr))

	if(receiveSize < 0) {
		perror("received failed");
	}

	// open the receive file
	strcat(command, filename);
	system(command);

	fclose(fp);
	close(sockfd);


	return 0;



}
