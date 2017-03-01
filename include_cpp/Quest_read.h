
#ifndef XL_API_H
#define XL_API_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libxl.h"

using namespace libxl;
using namespace std;
class ques {
		char ans;
		int mrk;
	public:
		int q_no;
		string question;
		char *choice[4];
	
	void copy_fn(int no)
	{
		q_no = no; 
		//cout << "in header :  ques no " << q_no << endl;
	}

	void ques_fn(string strr)
	{
		question = strr;
		//cout << "in head ques" << question <<endl;
	}

	void choice_fn(char *c_choice,int var)
	{ 

		choice[var] = c_choice;
		//cout << "in header choice  " << choice[var] <<endl;
	}

	void ans_fn(char *c_ans)
	{ 

		ans = *c_ans;
		//cout << "in head ans " << *c_ans <<endl;
	}
	char get_ans()
	{
		return ans;	
	}
};
int Read_quess(ques *obj,char *xl_path);
#endif
