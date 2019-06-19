
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
	int operate;
	int a[2];
}MS;

int main(void){
	struct MS m;
	pid_t pid;

	int s, s2, len;
	unsigned t;
	struct sockaddr_un local, remote;
	char str[100];

	if((s = socket (AF_UNIX, SOCK_STREAM, 0 )) == -1){
		perror("socker");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);

	if( bind( s, (struct sockaddr *)&local, len) == -1){
		perror("bind");
		exit(1);
	}
		if(listen(s, 5) == -1){
			perror("listen");
			exit(1);
		}
		for(;;){
			int done , n;
			printf("waitiong for a connection...\n");
			t = sizeof(remote);

			if(( s2 = accept(s, (struct sockaddr *)&remote, &t))==-1){
				perror("accept");
				exit(1);
			}

			printf("Connected.\n");

			done = 0;
	
			if((pid = fork())<0){
				printf("fork error");
				exit(0);
			}
			
			else if( pid == 0){ // child 
						
					do{
						n = recv(s2, &m , sizeof(m), 0);
						if( n<=0) {
							if(n<0) perror("recv");
							done =1;
						}	
				
						m.type =2;
					

						if(m.operate ==1){
							m.a[0] = m.a[0]+m.a[1];
						}	
						if(m.operate ==2){
							m.a[0] = m.a[0]-m.a[1];
						}
						if(m.operate ==3){
							m.a[0] = m.a[0]*m.a[1];
						}
						if(m.operate ==4){
							m.a[0] =( m.a[0])/( m.a[1]);
						}

						if(!done){
							
							if(send(s2,&m,sizeof(m),0)<0){
								perror("send");
								done=1;
							}
						}
					}while(!done);
			}
			else {
					sleep(2);

					n = recv(s2, &m , sizeof(m), 0);
					if( n<=0) {
						if(n<0) perror("recv");
						done =1;
					}	
				
					m.type =2;
					if(send(s2,&m,sizeof(m),0)<0){
							perror("send");
							done=1;
					}
					
			}
			
			close(s2);
					
			return 0;
		}
}

