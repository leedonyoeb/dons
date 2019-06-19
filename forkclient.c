// 소켓 이용해서 계산   자식<->(부모) 서버<->클라이언트
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>

#define SOCK_PATH "echo_socket"
typedef struct MS 
{ 
	int type;
	// 1,2 해서 1은 클라이언트 요청메시지 2는 서버 받는메시지
	int operate;// 덧셈뺼셈곱셈나눗셈
	int a[2];
}MS;


int main(void){
	struct MS m;
	int s, t, len;
	struct sockaddr_un remote;
	char str[100];
	

	if((s = socket (AF_UNIX, SOCK_STREAM, 0 )) == -1){
		perror("socket");
		exit(1);
	}
	
	printf("Trying to connect...\n");

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	
	if( connect( s, (struct sockaddr *)&remote, len) == -1){
		perror("connect");
		exit(1);
	}

	printf("Connected.\n");
    char cont;
	for(;;){
		
		printf("\n'c' close , else continue ");
		scanf("%c",&cont);
		if(cont=='c') break;
		

		printf("\na,b = ");
		scanf("%d%d",&m.a[0],&m.a[1]);
		printf("(1=+, 2=-, 3=*, 4=/)\nopreate  = ");
		scanf("%d",&m.operate);
		
		m.type = 1;
		
		if(send(s,&m,sizeof(MS),0)==-1 ){
			perror("send");
			exit(1);
		}

		sleep(2);
		if((t=recv(s,&m,sizeof(MS),0))>0){
			printf("=> %d\n",m.a[0]);
		}
		else{
			if(t<0) perror("recv");
			else printf("Server closed connetion\n");
			exit(1);
		
		}
		
		
			
	}
	close(s);

	return 0;
}
