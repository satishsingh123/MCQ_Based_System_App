/*  MCQ Client
Author:
-------
 *Satish Singh
 *Kathiravan
 *Subhasish Mishra
Date:
----
 *Jan-2017 to Feb-2017
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<errno.h>
#include<time.h>
#define MAX_CLIENT 50
int main(int argc , char **argv)
{
	int i,sid;
	/*	if ( argc != 4){
		fprintf(stderr , "%s <PORT>  <IP> <RANGE>",argv[0]);
		return;
		} 
	 */
	sid = socket( AF_INET, SOCK_STREAM, 0);
	if (sid < 0){
		perror("socket");
		return ;
	}
	//	puts("\nsock_id created sucess\n");

	struct sockaddr_in Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons (atoi("6666"));
	Addr.sin_addr.s_addr = inet_addr("192.168.1.247");

	if ( connect (sid,(const struct sockaddr *)&Addr, sizeof (Addr)) < 0){
		perror("connect");
		return;
	}
	//	puts("\nconnect sucess\n");
	int range,Q_Cnt;
	char range_qnum[20]={0};
	if(recv(sid,range_qnum,20,0 ) < 0){
		perror("recv");
		return;
	} 
	range = atoi(strtok(range_qnum,"__"));
	Q_Cnt = atoi(strtok(NULL,"__"));
	printf("||%d||%d|\n",range,Q_Cnt);

	char fl_name[30],*Ans=(char *)malloc(Q_Cnt),Score[30],*buf[Q_Cnt],*buf1;int status;

	for(i=0;i<Q_Cnt;i++)
		buf[i]=(char *)malloc(1024);

	time_t seconds;
	int rand_val;
	int arr[Q_Cnt];
	int index1 , index2,flag=0;
	char name[100];
	char mob_num[10];
	char name_mob[50],rt;
	//	printf("Msg is: %s\n",fl_name);	
	//	puts("\nwaiting for server msg\n");
	printf("*********************WELCOME TO VOTARY SOFTWARE SOLUTIONS***************\n");
	while(1)
	{
		printf("Enter Your name : ");
		scanf("%[^\n]s",name);
		getchar();
		printf("Enter your Mobile no : ");
		scanf("%s",mob_num);
		sprintf(name_mob,"%s_%s",name,mob_num);
		// THIS WILL HAVE  STRUCTURES OF QUESTIONS
		status = send(sid,name_mob,50,0);
		getchar();
		srand((unsigned)time(0));
		for ( index1 = 0 ; index1 < Q_Cnt; index1++){ // Loop to create Random Numbers
			rand_val = (rand()%range)+1;
			arr[index1]=rand_val;
			for ( index2 = 0; index2 <index1 ; index2++){ 
				if (arr[index2] == arr[index1])
				{index1--;
					flag=1;
					break;}

			}
			//	printf("||%d||\n",rand_val);
			if(flag)
				continue;
			flag=0;
		}
		for(index2=0;index2<Q_Cnt;index2++)
			printf("_%d_\n",arr[index2]);


		status = send(sid,arr,4*Q_Cnt,0);
		if(status == -1)
		{
			perror("send");
			close(sid);
			break;
		}
		for(i=0;i<Q_Cnt;i++)
		{
			status = recv(sid,buf[i],1024,0); // THIS WILL HAVE as requested STRUCTURES OF QUESTIONS
			if (status < 0){
				perror("recv");
				break;
			}
		}
		for(i=0;i<Q_Cnt;i++)
		{
			printf("\n%s\n",buf[i]);
			while(1)
			{
				printf("Answer: ");
				scanf("%c",&Ans[i]);
				while ( getchar() != '\n' );
				if(((Ans[i]>=65)&&(Ans[i]<=68))||((Ans[i]>=97)&&(Ans[i]<=100)))
					break;
				else
					printf("Enter a valid input:[A,B,C,D]\n");
			}
		}
		status = send(sid,Ans,Q_Cnt,0);
		if(status == -1)
		{
			perror("send");
			close(sid);
			break;
		}
		status = recv(sid,Score,30,0); // THIS WILL HAVE as requested STRUCTURES OF QUESTIONS
		if (status < 0){
			perror("recv");
			break;
		}
		printf("%s\n",Score);


		break;
	}
	puts("\nserver completed\n");
	close(sid);
	return 0;
}

