////////////////////////////////////////////////////////////////////////
// file_server.c -- socket文件传输服务器端示例代码
// /////////////////////////////////////////////////////////////////////
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define HELLO_WORLD_SERVER_PORT    6666
#define LENGTH_OF_LISTEN_QUEUE     20
#define BUFFER_SIZE                1024
unsigned char 		 buffer[BUFFER_SIZE];


int main(int argc, char **argv)
{
	

	// set socket's address information
	// 设置一个socket地址结构[sockaddr_in]server_addr,代表服务器internet的地址和端口
	struct sockaddr_in   server_addr,client_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//bind to the free interface (NIC)
	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

	// create a stream socket
	// 创建用于internet的流协议(TCP)socket，用server_socket代表服务器向客户端提供服务的接口
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd< 0)
	{
		printf("Create Socket Failed!\n");
		exit(1);
	}

	// 把socket和socket地址结构绑定
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		//on fail [-1] will be returned
		//on success [0] will be returned
		printf("Server Bind Port: %d Failed!\n", HELLO_WORLD_SERVER_PORT);
		exit(1);
	}

	// server_socket用于监听
	if (listen(sockfd, LENGTH_OF_LISTEN_QUEUE))
	{
		printf("Server Listen Failed!\n");
		exit(1);
	}

	socklen_t length = sizeof(client_addr);
	int connectSockfd = accept(sockfd, (struct sockaddr*)&client_addr, &length);
	if (connectSockfd < 0)
	{
		perror("cccept() Failed!\n");
		exit(1);
	}

	///**************************************************************
	// 服务器端一直运行用以持续为客户端提供服务
	//*****************************************************************
	int receiveSize = 0;
	int writeSize = 0;
	FILE *fp =  NULL;

	memset(buffer, 0, BUFFER_SIZE);
	while((receiveSize = recv(connectSockfd, buffer, BUFFER_SIZE, 0) ) >0 ){

		printf("recvfrom  receiveSize :  %d\n", receiveSize);

		if ( NULL != strstr(buffer,"compress_") && receiveSize < 50 ){
			printf("receive the file name \n");
			fp = fopen(buffer,"wb");
		}else if(NULL != strstr(buffer,"SENDFINISHED") /* && receiveSize < 50 */ ){
			printf("send file finished , break the while \n");
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

	fclose(fp);
	close(connectSockfd);
	close(sockfd);


	return 0;
}
