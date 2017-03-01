INC = -I./include_cpp/ -I. 
LIB = -L./libxl/
all:MCQ Candidate
MCQ:MCQ_Main.cpp XML_Parser.o Quest_read.o msql_conn_api.o
	g++ $(INC) $(LIB) MCQ_Main.cpp XML_Parser.o Quest_read.o msql_conn_api.o -o MCQ `pkg-config --cflags --libs libxml++-2.6` -lxl -lmysqlcppconn
XML_Parser.o:XML_Parser.cpp
	g++ -c $(INC) XML_Parser.cpp `pkg-config --cflags --libs libxml++-2.6`
Quest_read.o:Quest_read.cpp
	g++ -c  $(INC) $(LIB) Quest_read.cpp -lxl
msql_conn_api.o:msql_conn_api.cpp
	g++  -c -I./include_cpp/ -I. -I/usr/include/cppconn msql_conn_api.cpp -L/usr/lib -lmysqlcppconn
Candidate:./CLIENT/client.c
	gcc ./CLIENT/client.c -o Candidate
clean: 
	rm *.o MCQ Candidate
