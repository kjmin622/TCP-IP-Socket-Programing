#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message);
int calculate(int cnt, int* arr, char operator);

int main(int argc, char* argv[]){
	int sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int oparr[BUF_SIZE];
	int i, j, clnt_adr_sz, cnt, result;
	char operator;
	
	if(argc!=2){
		printf("Usage : %s <port>",argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	if(listen(sock,5)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz = sizeof(clnt_adr);
	for(i=0; i<5; i++){
		clnt_sock=accept(sock,(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		
		read(clnt_sock,&cnt,1);
		for(j=0; j<cnt; j++){
			read(clnt_sock,&oparr[j],OPSZ);
		}
		read(clnt_sock,&operator,1);
		result=calculate(cnt,oparr,operator);
		write(clnt_sock,(char*)&result,sizeof(result));
		close(clnt_sock);
	}
	close(sock);
}

int calculate(int cnt, int* arr, char operator){
	int result = arr[0];
	for(int i=1; i<cnt; i++){
		if(operator=='+') result+=arr[i];
		else if(operator=='-') result-=arr[i];
		else if(operator=='*') result*=arr[i];
	}
	return result;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}