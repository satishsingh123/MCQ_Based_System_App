/*  MCQ XML Reader
Author:
-------
*Chaitan viradia
Date:
----
*Jan-2017 to Feb-2017
*/

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include "XML_Parser.h"
using namespace Glib;
ustring u_file_name,u_file_path,u_DB_usr,u_DB_psw,u_DB_ip,u_DB_name;
void xml_config:: display_config()
{
	std::cout<<"..........................................."<<endl;
	std::cout<<"file_name:"<<file_name<<endl;
	std::cout<<"file_path:"<<file_path<<endl;
	std::cout<<"DB_usr:"<<s_config.DB_usr<<endl;
	std::cout<<"DB_psw:"<<s_config.DB_psw<<endl;
	std::cout<<"DB_ip:"<<s_config.DB_ip<<endl;
	std::cout<<"DB_name:"<<s_config.DB_name<<endl;
	std::cout<<"..........................................."<<endl;
}
void xml_config:: set_exl_config()
{

	strcpy(file_name,u_file_name.c_str());
	strcpy(file_path,u_file_path.c_str());
}
void xml_config::set_DB_config()
{
	strcpy(s_config.DB_usr,u_DB_usr.c_str());
	strcpy(s_config.DB_psw,u_DB_psw.c_str());
	strcpy(s_config.DB_ip,u_DB_ip.c_str());
	strcpy(s_config.DB_name,u_DB_name.c_str());
}

int Parse_XML(char *xml_file,xml_config *c_config)
{
xmlpp::TextReader reader(xml_file);

while(reader.read())
{
  try
  {
		if(reader.get_name()=="file_name")
		{
		reader.read();
		u_file_name=reader.get_value();
		reader.read();
	continue;	
		}

		if(reader.get_name() == "file_path")
		{
		reader.read();
		u_file_path=reader.get_value();
		reader.read();
	continue;	
		}

		if(reader.get_name() == "DB_usr")
		{
		reader.read();
		u_DB_usr=reader.get_value();
		reader.read();
	continue;	
		}

		if(reader.get_name() == "DB_psw")
		{
		reader.read();
		u_DB_psw=reader.get_value();
		reader.read();
	continue;	
		}

		if(reader.get_name() == "DB_ip")
		{
		reader.read();
		u_DB_ip=reader.get_value();
		reader.read();
	continue;	
		}

		if(reader.get_name() == "DB_name")
		{
		reader.read();
		u_DB_name=reader.get_value();
		reader.read();
	continue;	
		}
	}	
		  
  catch(const std::exception& e)
  {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }
}//while

	c_config->set_exl_config();
	c_config->set_DB_config();
	//c_config->display_config();
	
}//main


