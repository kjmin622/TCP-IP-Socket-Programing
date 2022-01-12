#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
void error_handling(char *message);

int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len=0;
	int idx=0, read_len=0;
	
	if(argc!=3){
		printf("Usage : %s <IP> <PORT>\n",argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0); 
	//PF_INET: IPv4 인터넷 프로토콜 체계
	//SOCK_STREAM: 연결지향형 소켓(TCP)
	//위 두 인자로써 세번째 인자인 IPPROTO_TCP 생략 가능
	
	if(sock==-1){
		error_handling("socket() error");
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}
	
	while(read_len=read(sock, &message[idx++],1)){ // read함수가 1바이트씩 데이터를 읽어온다.
		if(read_len==-1){
			error_handling("read_error");
		}
		str_len+=read_len;
	}
	printf("Message from server: %s \n",message);
	printf("Function read call count: %d \n",str_len);
	close(sock);
	return 0;	
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}