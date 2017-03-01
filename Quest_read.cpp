/*  MCQ Excel Sheet Module
Author:
-------
*Kathiravan
Date:
----
*Jan-2017 to Feb-2017
*/

#include"Quest_read.h"
using namespace std;

int Read_quess(ques *obj,char *xl_path) 
{

//	obj = new ques[100];
	char  *p_t[6];
	char *ptr = new char[50];
	const char  *p;
	int row = 0,  col = 1;
	int cnt = 0;
	int q_no = 1;
	int obj_cnt = 0;
	char ch = 'A'; 
	int choice_cnt = 0;


	Book* book = xlCreateXMLBook();//for xlsx
	if(book)
	{
		if(book->load(xl_path))
		{
			Sheet* sheet = book->getSheet(0);

			while(1)
			{
				p_t[0]=new char[1024]; //memory for question 
	
				for(int index_1 = 1; index_1 < 6; index_1++) //memory for choices
					p_t[index_1] = new char[500];
				
				obj[obj_cnt].copy_fn(q_no);   // Question No.

				p = sheet->readStr(row ,col); // Question
				if ( p == NULL)
					break;
				memcpy(p_t[0], p, strlen(p)+1);
				//cout << "in Quest " << p_t[0] <<endl;
				obj[obj_cnt].ques_fn((string)p_t[0]);

				col++;

				p = sheet->readStr(row , col); //Choice A
				memcpy(p_t[1],p,strlen(p)+1);
				obj[obj_cnt].choice_fn(p_t[1],0); //0

				col++;
				choice_cnt++; 

				p = sheet->readStr(row , col); //Choice B
				memcpy(p_t[2],p,strlen(p)+1);
				obj[obj_cnt].choice_fn(p_t[2],1);//1

				col++;
				choice_cnt++; 

				p = sheet->readStr(row , col); //Choice C
				memcpy(p_t[3],p,strlen(p)+1);
				obj[obj_cnt].choice_fn(p_t[3],2);//2

				col++;
				choice_cnt++; 

				p  = sheet->readStr(row , col); //Choice D
				memcpy(p_t[4],p,strlen(p)+1);
				obj[obj_cnt].choice_fn(p_t[4],3);//3

				col++;

				p  = sheet->readStr(row , col); //answer
				//cout << p <<endl;
				memcpy(p_t[5],p,strlen(p)+1);
				//cout << p_t[5] << endl;
				obj[obj_cnt].ans_fn(p_t[5]);
				//cout << obj[obj_cnt].get_ans() << endl;


				obj_cnt++ , q_no ++ , cnt ++, row ++, col = 1 , choice_cnt = 0;  //incrementing

				cout<<endl;

				//for(int index_2 = 0;index_2 < 6;index_2++)
				//	memset(p_t[index_2], 0, strlen(p_t[index_2]));
			//	memset(ptr , 0 ,strlen(ptr));
			}
		}
		else
		{
			cout<< "Error: Path not found.\n";
			return -1; 
		}

		book->release();
	}
/*	
	for ( int i = 0; i < 3 ; i++){
		
		cout << "in read2 " << obj[i].choice[0] << endl;
	} */
	
	return obj_cnt;
}
