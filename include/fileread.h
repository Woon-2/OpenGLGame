#ifndef _fileread
#define _fileread

#include <fstream>
#include <iostream>
#include <sstream>

const std::string read_file( const char* file_path )
{
	std::stringstream ss;
	std::ifstream in{ file_path };
	if ( in.fail() ) std::cerr << file_path << "파일이 존재하지 않습니다.\n";
	ss << in.rdbuf();

	return ss.str();
}

#endif