#ifndef _fileread
#define _fileread

#include <sstream>
#include <fstream>
#include <iostream>

const std::string read_file( const char* file_path )
{
	std::stringstream ss;
	std::ifstream in{ file_path };
	if ( in.fail() ) std::cerr << "파일이 존재하지 않습니다.\n";
	ss.set_rdbuf( in.rdbuf() );
	return ss.str();
}

#endif