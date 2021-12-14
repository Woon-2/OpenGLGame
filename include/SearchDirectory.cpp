#include <iostream> //I/O스트림 헤더
#include <io.h> //파일 구조체 헤더
#include <string>//스트링 객체 사용 헤더
#include <list>//리스트 자료형 헤더

using namespace std;

void searchingDir( string path );
int isFileOrDir( _finddata_t fd );

int main() {
	string path = "resources/object";
	searchingDir( path );

}

void searchingDir( string path )
{
	int checkDirFile = 0;
	string dirPath = path + "\\*.*";
	struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
	intptr_t handle;
	list<_finddata_t> fdlist;//디렉토리, 파일 정보 객체 리스트

	if ( ( handle = _findfirst( dirPath.c_str(), &fd ) ) == -1L ) //fd 구조체 초기화.
	{
		//파일이나 디렉토리가 없을 경우.
		cout << "No file in directory!" << endl;
		return;
	}

	do //폴더 탐색 반복 시작
	{
		checkDirFile = isFileOrDir( fd );//현재 객체 종류 확인(파일 or 디렉토리)
		if ( checkDirFile == 0 && fd.name[ 0 ] != '.' ) {
			//디렉토리일 때의 로직
			cout << "Dir  : " << path << "\\" << fd.name << endl;
			searchingDir( path + "\\" + fd.name );//재귀적 호출
		}
		else if ( checkDirFile == 1 && fd.name[ 0 ] != '.' ) {
			//파일일 때의 로직
			cout << '"' << path << "/" << fd.name << "\",\n";
			fdlist.push_back( fd );
		}

	} while ( _findnext( handle, &fd ) == 0 );
	_findclose( handle );
}

int isFileOrDir( _finddata_t fd )
//파일인지 디렉토리인지 판별
{

	if ( fd.attrib & _A_SUBDIR )
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

}