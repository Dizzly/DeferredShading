#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include <string>
#include <fstream>
#include "Vec2.h"
#include "Vec3.h"
class File
{
public:
	bool OpenRead(const std::string& filename, bool isBinary=false);
	bool OpenWrite(const std::string& filename);
	int GetBinary(unsigned int bytes, unsigned char* pBuffer);
	void BinarySeek(unsigned int seekPos);
	bool GetInteger(int* result);
	bool GetFloat(float* result);
	bool GetBool(bool* result);
	bool GetString(std::string* result);
	bool GetCSVec2i(Vec2i* result);
	bool GetCSVec2f(Vec2f* result);
	bool GetCSVec3f(Vec3f* result);
	bool GetCSVec3i(Vec3i* result);
	bool WriteString(std::string);
	bool WriteInt(int);
	bool WriteFloat(float);
	int GetLineNum()const;
	std::string GetFileName();
	void Close();
	
	~File();
	File();
private:
	std::string filename_;
	std::fstream m_file;
	char comment_;
	int line_;
};


#endif