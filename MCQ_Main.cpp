/*  MCQ Main
Author:
-------
*Subhasish Mishra
Date:
----
*Jan-2017 to Feb-2017
*/
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<malloc.h>
#include "XML_Parser.h"
#include "Quest_read.h"
#include "Msql_conn_api.h"
//#include<semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void * get_in_addr(struct sockaddr * sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
int sem_id;
void semaphore_lock(void)
{
	struct sembuf v;

	v.sem_num = 0;
	v.sem_op = -1;
	v.sem_flg = SEM_UNDO;

	if (semop(sem_id, &v, 1) == -1) {
		perror("Thread1:semop failure Reason:");
		exit(-1);
	}

}

void semaphore_unlock()
{
	struct sembuf v;
	v.sem_num = 0;
	v.sem_op = 1;
	v.sem_flg = SEM_UNDO;

	if (semop(sem_id, &v, 1) == -1) {
		perror("Thread1:semop failure Reason:");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	int status,req_ques_no,Quest_Range;
	struct addrinfo hints, * res;
	int listner;
	if(argc != 6)
	{
		fprintf(stderr,"USAGE:%s <Config_xml_file> <IP> <Port> <Question_Count_To_Display> <Question_Range_From_Excel>\n",argv[0]);
		return -1;
	}
	sem_id = semget((key_t)134, 2, 0666 | IPC_CREAT); 

	/*****Socket Details******/
	memset(& hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE;

	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo(argv[2],argv[3] , &hints, &res);
	req_ques_no = atoi(argv[4]);
	Quest_Range = atoi(argv[5]);
	char range_count[20]={0};
	sprintf(range_count,"%d__%d",Quest_Range,req_ques_no);
	/* if(send(sid,&req_ques_no,4,0) < 0){
		perror("send");
		return;
	}*/

	if(status != 0)
	{
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	}

	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
	if(listner < 0 )
	{
		fprintf(stderr,"socket error: %s\n",gai_strerror(status));
	}

	// Bind the socket to the address of my local machine and port number 
	status = bind(listner, res->ai_addr, res->ai_addrlen);
	if(status < 0)
	{
		fprintf(stderr,"bind: %s\n",gai_strerror(status));
	}

	status = listen(listner, 50);
	if(status < 0)
	{
		fprintf(stderr,"listen: %s\n",gai_strerror(status));
}


	// Free the res linked list after we are done with it   
	freeaddrinfo(res);


	// We should wait now for a connection to accept
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char *Qst[req_ques_no],s[INET6_ADDRSTRLEN],Ans[req_ques_no]; // an empty string 
	for(int i=0;i<req_ques_no;i++)
		Qst[i]=(char *)malloc(1024);
	// Calculate the size of the data structure     
	addr_size = sizeof client_addr;

	printf("I am now accepting connections ...\n");
	void *buf=malloc(40);
	char *name_mob=new char[50];
	char *Cli_det=new char[50];
	int *arr;
	/******************************Socket**********************************/

	xml_config *XML_data = new xml_config; // Class declaration

	Parse_XML(argv[1],XML_data);

	DB_config DB_data=XML_data->s_config; //structure declaration
	int Ques_cnt;

	ques *obj;
	//class obj
	obj = new ques[100];
	printf("file path is %s\n",XML_data->file_name);


	Ques_cnt = Read_quess(obj,XML_data->file_name);//read function call
//	printf("ques size%d\n",Ques_cnt);

	Connect_sql Sql_info(&DB_data);
	Sql_info.create_table(DB_data.DB_name);

	//printf("cnt=%d\n",Ques_cnt);

	//	struct read_db rdb[3];
	int ret,object_cnt;
	struct read_db rdb[req_ques_no];
	char *token,flname[20];
	for(object_cnt = 0; object_cnt < Ques_cnt; object_cnt++)
	{	
		Sql_info.insert_table(obj[object_cnt]);
	}
	int size=req_ques_no,pid;//CAN BE CONFIGURED THROUGH XML 
	/*************Multiple time Execution***************/
	while(1)
	{

		new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);
		if(new_conn_fd < 0)
		{
			fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
			continue;
		}
		pid = fork();
		if(pid==0)
		{
		//	while(1){
				  if(send(new_conn_fd,range_count,20,0) < 0){
                			perror("send");
			                return -1;
			        }

				FILE *fp;
				status = recv(new_conn_fd,name_mob,50,0);
				cout<<"Name__Mobile: "<<name_mob<<endl;
				memcpy(Cli_det,name_mob,strlen(name_mob)+1);
				token = strtok(name_mob,"_");
				token[4] = '\0';   
				sprintf( flname,"Client_Log/%s_%s",strtok(NULL, s),token);
				if(!(fp=fopen( flname,"w+")))
				{
					fprintf(stderr,"unable to open file:%s\n",flname);
					return -1;
				}	
				status = recv(new_conn_fd,buf,4*req_ques_no,0);
				if(status == -1)
				{
					close(new_conn_fd);
					_exit(4);
				}
				arr=(int *)buf;
				for(int i=0;i<req_ques_no;i++)
					printf("_%d_\n",arr[i]);

				semctl(sem_id,0,SETVAL,1);
				semaphore_lock();
				ret=Sql_info.display_table(rdb,arr,size);
				//		}
				semctl(sem_id,0,SETVAL,0);
				semaphore_unlock();
				printf("Total q: %d\n",ret);			
				for(int i=0;i<ret;i++)
				{
					//printf("***************************\n");
					//     cout << "\tQId : " <<"Choice  " << endl;
					//		cout <<rdb[i].questno<< "\t"<<rdb[i].question<<"\n \n"<<rdb[i].choice1 <<"\t" <<rdb[i].choice2 <<"\t" <<rdb[i].choice3 <<"\t" <<rdb[i].choice4 << "\n" <<rdb[i].answer[0]<<endl;
					sprintf(Qst[i],"%d:  %s\n%s\n%s\n%s\n%s\n",i+1,rdb[i].question,rdb[i].choice1,rdb[i].choice2,rdb[i].choice3,rdb[i].choice4);	
					//	sleep(10);
				}	
				inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);
				printf("I am now connected to %s \n",s);
				for(int i=0;i<ret;i++)
				{
					status = send(new_conn_fd,Qst[i], strlen(Qst[i])+1,0);
					if(status == -1)
					{
						close(new_conn_fd);
						_exit(4);
					}
					usleep(500);
				}
				status = recv(new_conn_fd,Ans,req_ques_no,0);
				if(status == -1)
				{
					close(new_conn_fd);
					_exit(4);
				}
				printf("Answers written by %s is : %s\n",Cli_det,Ans);
				/**************VALIDATION*****************/
				char Score[30];
				int marks=0,in1=0,in2=0;
				for(int i=0;i<req_ques_no;i++)
				{

					if((rdb[i].answer[0]==Ans[i])||(rdb[i].answer[0]==(Ans[i]-32)))
						marks++;
				}
				sprintf(Score,"Total Marks scored is %d\n",marks);
				status = send(new_conn_fd,Score, strlen(Score)+1,0);
				if(status == -1)
				{
					close(new_conn_fd);
					_exit(4);
				}
				fprintf(fp,"%s\n%s\n",Cli_det,s);
				fprintf(fp,"Q.No  --> Cand.Ans  --> Org.Ans\n");
				for(int i=0;i<req_ques_no;i++)
					fprintf(fp," %02d   -->   %c     -->   %c\n",arr[i],Ans[i],rdb[i].answer[0]);
				fprintf(fp,"%s\n",Score);
				fclose(fp);
	//	}//inner fork while while(1) inside child
		close(new_conn_fd);
	}//fork if(child)
}//outer while
return 0;

}
