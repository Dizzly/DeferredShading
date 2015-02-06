#include "DirectoryFunctions.h"


std::string GetCWD()
{
	char pwd[MAX_PATH]={};
	_getcwd(pwd,MAX_PATH);
	std::stringstream ss;
	ss<<pwd;
	ss<<"\\";
	return ss.str();
}

std::string ExtractDirectory(const std::string& str)
{
	int place =str.find_last_of('/');
	if(place==-1)
	{
	return std::string();
	}
	return str.substr(0,place+1);
}