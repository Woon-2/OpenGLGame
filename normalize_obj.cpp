#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>

template < typename ... Elems, size_t ... Idx >
std::istream& istream_get_impl( std::istream& is, std::tuple< Elems... >& my, std::index_sequence< Idx... > )
{
	int dummy[ sizeof...( Elems ) ] = { ( is >> std::get< Idx >( my ), 0 )... };
	return is;
}

template < typename ... Elems >
std::istream& operator>>( std::istream& is, std::tuple< Elems... >& my )
{
	istream_get_impl( is, my, std::make_index_sequence < sizeof...( Elems ) >{} );
	return is;
}

template < typename ... Elems, size_t ... Idx >
std::ostream& ostream_put_impl( std::ostream& os, std::tuple< Elems... >& my, std::index_sequence< Idx... > )
{
	int dummy[ sizeof...( Elems ) ] = { ( os << std::get< Idx >( my ) << ' ', 0 )...};
	return os;
}

template < typename ... Elems >
std::ostream& operator<<( std::ostream& os, std::tuple< Elems... >& my )
{
	ostream_put_impl( os, my, std::make_index_sequence< sizeof...( Elems ) >{} );
	return os;
}

bool go( const std::string& ifile_path, const std::string& ofile_path, const size_t file_size, const float x_move, const float y_move, const float z_move )
{
	std::ifstream in{ ifile_path };
	std::ofstream out{ ofile_path };

	if ( in.fail() )
	{
		return false;
	}

	while ( in.good() )
	{
		char c = in.get();

		if ( c == 'v' )
		{
			if ( in.peek() == ' ' )
			{
				in.get();
				out.put( 'v' );
				out.put( ' ' );

				std::tuple< float, float, float > vdata;
				in >> vdata;
				
				std::get< 0 >( vdata ) += x_move;
				std::get< 1 >( vdata ) += y_move;
				std::get< 2 >( vdata ) += z_move;

				out << vdata;
			}
			else
			{
				out.put( c );
			}
		}
		else
		{
			out.put( c );
		}
	}

	return true;
}

int main()
{
	std::string ifile_path;
	std::cout << "입력 파일 경로를 입력하세요. : ";
	std::cin >> ifile_path;

	std::string ofile_path;
	std::cout << "출력 파일 경로를 입력하세요. : ";
	std::cin >> ofile_path;

	size_t file_size;
	std::cout << "파일 크기를 입력하세요. : ";
	std::cin >> file_size;

	float x_move;
	std::cout << "x 좌표 이동량을 입력하세요. : ";
	std::cin >> x_move;

	float y_move;
	std::cout << "y 좌표 이동량을 입력하세요. : ";
	std::cin >> y_move;

	float z_move;
	std::cout << "z 좌표 이동량을 입력하세요. : ";
	std::cin >> z_move;

	if ( go( ifile_path, ofile_path, file_size, x_move, y_move, z_move ) )
	{
		std::cout << ofile_path << " 파일을 작성하는 데 성공했습니다.\n";
	}
	else
	{
		std::cout << ifile_path << " 파일이 존재하지 않습니다.\n";
	}
}