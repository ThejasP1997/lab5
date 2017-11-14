#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<errno.h>

#include<string.h>

#include<netdb.h>

#include<sys/types.h>

#include<netinet/in.h>

#include<sys/socket.h>

#include<fcntl.h>

#define PORT 6490

int main()

{

int i=0,sockfd,len;

char buf1[40],buf2[20000];

FILE* fp;

struct sockaddr_in their_addr;if((sockfd=socket(AF_INET, SOCK_STREAM,0))==-1)

{

perror("socket");

exit(1);

}

their_addr.sin_family=AF_INET;

their_addr.sin_port=htons(PORT);

their_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

memset(&(their_addr.sin_zero), '\0', 8);

if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))==-1)

{

perror("connect");

exit(1);

}

printf("CLIENT is online!\n");

printf("CLIENT:Enter the filename to be displayed: "); scanf("%s",buf1);

send(sockfd,buf1,sizeof(buf1),0);

if(recv(sockfd,buf2,sizeof(buf2),0)==1)

{

perror("recv");

exit(1);

}

else

{

printf("Displyaing the contents of %s",buf1);

printf("\n%s\n",buf2);

}

close(sockfd);

return 0;}



****Server program****

#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<errno.h>

#include<string.h>

#include<sys/types.h>

#include<sys/stat.h>

#include<fcntl.h>

#include<netinet/in.h>

#include<sys/socket.h>

#include<arpa/inet.h>

#include<sys/wait.h>

#include<signal.h>

#define MYPORT 6490

#define BACKLOG 10

int main(void)

{

int sockfd,fp,new_fd;

struct sockaddr_in my_addr,their_addr; int sin_size,i=0;
int yes=1;

char buf1[20],buf2[20000];

if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)

{

perror("socket");

exit(1);

}
if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)

{

perror("setsockopt");

exit(1);

}
my_addr.sin_family=AF_INET;
converting an integer address to network format htons
my_addr.sin_port=htons(MYPORT);

my_addr.sin_addr.s_addr=INADDR_ANY; this PC's address memset(&(my_addr.sin_zero), '\0', 8);

if(bind(sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr)) ==-1)

{

perror("Bind");

exit(1);

}

binding the socket and server address

if(listen(sockfd, BACKLOG) == -1)

{
perror("listen"); listening to 'backlog =10 'number of
exit(1);	people at once

}

printf("\n SERVER is online! \n SERVER: Waiting for the client........\n");

sin_size=sizeof(struct sockaddr_in);

if((new_fd=accept(sockfd,(struct sockaddr *)&their_addr, &sin_size))==-1)

{

perror("Accept");

exit(0);

}

printf("\n SERVER: Got connection from %s \n", inet_ntoa(their_addr.sin_addr));
receiving file name and storing in buffer 1
recv(new_fd,&buf1,sizeof(buf1),0);

printf("File requested is %s\n", buf1);

if((fp=open(buf1,O_RDONLY))<0)

{
trying to open file in read mode






Department of CSE, Dr.AIT
Page 41
Network Lab Manual


printf("File not found\n");

strcpy(buf2,"File not found");

}


else

{  if open in read more is possible then read

printf("SERVER: %s found and ready to transfer.\n",buf1);

read(fp,&buf2,20000);

close(fp);

}

send(new_fd,&buf2,sizeof(buf2),0);

close(new_fd);

close(sockfd);

printf("Transfer success \n");

printf("\n");

return 0;

}
