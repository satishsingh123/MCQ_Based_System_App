#ifndef MSQL_CONN_API_H
#define MSQL_CONN_API_H

#include<stdlib.h>
#include<iostream>
#include<string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "mysql_connection.h"
#include "Quest_read.h"
#include "XML_Parser.h"
#define MAX_SIZE 512
using namespace std;

using namespace sql;
struct read_db
{
        int questno;
        char question[MAX_SIZE];
        char choice1[MAX_SIZE];
        char choice2[MAX_SIZE];
        char choice3[MAX_SIZE];
        char choice4[MAX_SIZE];
        char answer[2];
};
class Connect_sql
{
	public:
		Connect_sql(DB_config *DB_data);
public:		
//struct read_db sdb[15];
	public:
		sql::Driver *driver;
      		Connection *con;
        	Statement *stmt;
        	ResultSet *res;
        	PreparedStatement *pstmt;

	public:
		void create_table(char *);
		void insert_table(ques);
		int display_table(struct read_db *,int *data,int size);

	private:
		int count;
		int quest_no;
		string question;
		char *choice[4];
		string answer;
			
};

#endif
