/*  MCQ Mysql Module
Author:
-------
 *Satish Singh
Date:
----
 *Jan-2017 to Feb-2017
 */

#include "Msql_conn_api.h"
#include<string.h>
#define MAX_SIZE 512
Connect_sql::Connect_sql(DB_config *DB_data)
{
	try{
		driver = get_driver_instance();
		con = driver->connect(DB_data->DB_ip, DB_data->DB_usr, DB_data->DB_psw);
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	}}
/* Connect to the MySQL test database */
void Connect_sql :: create_table(char *DB_name)
{
	try{
		con->setSchema(DB_name);

		stmt = con->createStatement();
		stmt->execute("DROP TABLE IF EXISTS test");
		stmt->execute("CREATE TABLE test(QuesNo INT,Question TEXT,Choice1 TEXT,Choice2 Text,Choice3 Text,Choice4 Text,Answer Text)");

	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	}
	delete stmt;
}
/* '?' is the supported placeholder syntax */
void Connect_sql :: insert_table(ques obj)
{
	int index=0;
	try
	{
		answer = obj.get_ans();
		pstmt = con->prepareStatement("INSERT INTO test(QuesNo,Question,Choice1,Choice2,Choice3,Choice4,Answer) VALUES (?,?,?,?,?,?,?)");

		pstmt->setInt(1, obj.q_no);
		pstmt->setString(2,obj.question);
		//		cout << obj.question;
		for(int ch=3;ch<7;ch++)
			pstmt->setString(ch,obj.choice[index++]);	
		pstmt->setString(7,answer);
		//	pstmt->setInt(7,(int)obj.get_ans());
		pstmt->executeUpdate();
		//	}
}

catch (sql::SQLException &e) {
	cout << "# ERR: SQLException in " << __FILE__;
	cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	cout << "# ERR: " << e.what();
	cout << " (MySQL error code: " << e.getErrorCode();
	cout << ", SQLState: " << e.getSQLState() << " )" << endl;

}
delete pstmt;
}
/* Select in ascending order */
int Connect_sql :: display_table(struct read_db *rdb,int *data,int size)
{
	int index=0;
//	printf("size= %d\n",size);
	char buff[40];
	int i=0;
	while(index<size)
	{
		sprintf(buff,"SELECT * FROM test WHERE QuesNo = %d",data[index]);
	//	printf("||%s||\n",buff);
		try{
			pstmt = con->prepareStatement(buff);
			res = pstmt->executeQuery();
			//	string str;
			while (res->next()){
				rdb[i].questno = res->getInt("QuesNo");
				strcpy(rdb[i].question,res->getString("Question").c_str());
				strcpy(rdb[i].choice1,res->getString("Choice1").c_str());
				strcpy(rdb[i].choice2,res->getString("Choice2").c_str());
				strcpy(rdb[i].choice3,res->getString("Choice3").c_str());
				strcpy(rdb[i].choice4,res->getString("Choice4").c_str());
				strcpy(rdb[i].answer,res->getString("Answer").c_str());
				i++;
						}
				/*	while (res->next()){
				cout << "\tQId : " <<"Choice  " << endl;
				cout <<"\t" << res->getInt("QuesNo") << "\t"<<res->getString("Question")<<"\t"<< res->getString("Choice1") <<"\t" << res->getString("Choice2") <<"\t" << res->getString("Choice3")<<"\t" << res->getString("Choice4")<<"\t" << res->getString("Answer") << endl;
		}*/

		}

		catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		}
		index++;
		memset(buff,0,strlen(buff));
	}
	//	delete res;
	//
	//	delete pstmt;

	//	delete con;
	return i;
}

/*
   for(int j=0;j<3;j++)
   {
   cout << "\tQId : " <<"Choice  " << endl;
   cout <<"\t" << rdb[i].questno<< "\t"<<rdb[i].question<<"\t"<<rdb[i].choice1 <<"\t" <<rdb[i].choice2 <<"\t" <<rdb[i].choice3 <<"\t" <<rdb[i].choice4 << endl;
   }
 */	//return EXIT_SUCCESS;	

