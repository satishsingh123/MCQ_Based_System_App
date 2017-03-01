#ifndef XML_API_H
#define XML_API_H
//#include <libxml++/libxml++.h>
//#include <libxml++/parsers/textreader.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct DB_config
{
        char DB_usr[30];
        char DB_psw[30];
        char DB_ip[30];
        char DB_name[30];
};

class xml_config
{

        public:
                char file_name[30];
                char file_path[30];
                struct DB_config s_config;
                void set_exl_config();
                void set_DB_config();
                void display_config();

};
int Parse_XML(char *xml_file,xml_config *c_config);
#endif

