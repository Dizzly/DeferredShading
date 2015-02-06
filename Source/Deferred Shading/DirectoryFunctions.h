#ifndef DIRECTORYFUNCTIONS_H_INCLUDED
#define DIRECTORYFUNCTIONS_H_INCLUDED
#ifdef WIN32
#include <string>
#include <direct.h>
#include <sstream>
#include <Windows.h>

std::string GetCWD();


std::string ExtractDirectory(const std::string& str);

#endif
#endif