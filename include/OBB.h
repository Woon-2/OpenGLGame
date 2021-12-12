#ifndef _OBB
#define _OBB

#include "quaternion_rotate.h"
#include "BBBase.h"

class OBB : public BBBase
{
public:
	static constexpr const int Dimension = 3;
	using Pos_t = glm::vec3;
	using Diff_t = Pos_t;
	using Rad_t = Pos_t;
	using Mat_t = glm::mat3;
	using CartesianMat_t = glm::mat4;
	using CartesianPos_t = glm::vec4;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << BBBase::get_desc( fore_indent + indent ).rdbuf();
		}
	}

#define A half_len
#define B ohalf_len

	const Mat_t make_axis_dirs() const { return rotate_quat( glm::mat4( 1.0f ), coord->getradians() ); }

	const bool collide( const OBB& other ) const
	{
		auto half_len = get_half_len();
		auto ohalf_len = other.get_half_len();
		
		Diff_t diff = other.coord->getpivot() - coord->getpivot();
		static constexpr auto cutoff = 0.999999f;

		// 만약 이상하다면 C 전치시키기
		auto axis_dirs = make_axis_dirs();
		auto oaxis_dirs = other.make_axis_dirs();
		Mat_t C = glm::transpose( axis_dirs ) * oaxis_dirs;
		Diff_t D;

		bool exists_parallel_pair = false;


		for ( int i = 0; i < Dimension; ++i )
		{
			for ( int j = 0; j < Dimension; ++j )
			{
				C[ i ][ j ] = glm::abs( C[ i ][ j ] );
				if ( C[ i ][ j ] > cutoff ) exists_parallel_pair = true;
			}

			D[ i ] = glm::abs( glm::dot( diff, axis_dirs[ i ] ) );
			auto OD = glm::abs( glm::dot( diff, oaxis_dirs[ i ] ) );

			if ( D[ i ] > A[ i ] + B[ 0 ] * C[ i ][ 0 ] + B[ 1 ] * C[ i ][ 1 ] + B[ 2 ] * C[ i ][ 2 ] ) return false;
			if ( OD > B[ i ] + A[ 0 ] * C[ 0 ][ i ] + A[ 1 ] * C[ 1 ][ i ] + A[ 2 ] * C[ 2 ][ i ] ) return false;
		}

		if ( exists_parallel_pair ) return true;

		// A0 X B0
		if ( glm::abs( C[ 1 ][ 0 ] * D[ 2 ] - C[ 2 ][ 0 ] * D[ 1 ] ) >
			A[ 1 ] * C[ 2 ][ 0 ] + A[ 2 ] * C[ 1 ][ 0 ] + B[ 1 ] * C[ 0 ][ 2 ] + B[ 2 ] * C[ 0 ][ 1 ] ) return false;

		// A0 X B1
		if ( glm::abs( C[ 1 ][ 1 ] * D[ 2 ] - C[ 2 ][ 1 ] * D[ 1 ] ) >
			A[ 1 ] * C[ 2 ][ 1 ] + A[ 2 ] * C[ 1 ][ 1 ] + B[ 0 ] * C[ 0 ][ 2 ] + B[ 2 ] * C[ 0 ][ 0 ] ) return false;

		// A0 X B2
		if ( glm::abs( C[ 1 ][ 2 ] * D[ 2 ] - C[ 2 ][ 2 ] * D[ 1 ] ) >
			A[ 1 ] * C[ 2 ][ 2 ] + A[ 2 ] * C[ 1 ][ 2 ] + B[ 0 ] * C[ 0 ][ 1 ] + B[ 1 ] * C[ 0 ][ 0 ] ) return false;

		// A1 X B0
		if ( glm::abs( C[ 2 ][ 0 ] * D[ 0 ] - C[ 0 ][ 0 ] * D[ 2 ] ) >
			A[ 0 ] * C[ 2 ][ 0 ] + A[ 2 ] * C[ 0 ][ 0 ] + B[ 1 ] * C[ 1 ][ 2 ] + B[ 2 ] * C[ 1 ][ 1 ] ) return false;

		// A1 X B1
		if ( glm::abs( C[ 2 ][ 1 ] * D[ 0 ] - C[ 0 ][ 1 ] * D[ 2 ] ) >
			A[ 0 ] * C[ 2 ][ 1 ] + A[ 2 ] * C[ 0 ][ 1 ] + B[ 0 ] * C[ 1 ][ 2 ] + B[ 2 ] * C[ 1 ][ 0 ] ) return false;

		// A1 X B2
		if ( glm::abs( C[ 2 ][ 2 ] * D[ 0 ] - C[ 0 ][ 2 ] * D[ 2 ] ) >
			A[ 0 ] * C[ 2 ][ 2 ] + A[ 2 ] * C[ 0 ][ 2 ] + B[ 0 ] * C[ 1 ][ 1 ] + B[ 1 ] * C[ 1 ][ 0 ] ) return false;

		// A2 X B0
		if ( glm::abs( C[ 0 ][ 0 ] * D[ 1 ] - C[ 1 ][ 0 ] * D[ 0 ] ) >
			A[ 0 ] * C[ 1 ][ 0 ] + A[ 1 ] * C[ 0 ][ 0 ] + B[ 1 ] * C[ 2 ][ 2 ] + B[ 2 ] * C[ 2 ][ 1 ] ) return false;

		// A2 X B1
		if ( glm::abs( C[ 0 ][ 1 ] * D[ 1 ] - C[ 1 ][ 1 ] * D[ 0 ] ) >
			A[ 0 ] * C[ 1 ][ 1 ] + A[ 1 ] * C[ 0 ][ 1 ] + B[ 0 ] * C[ 2 ][ 2 ] + B[ 2 ] * C[ 2 ][ 0 ] ) return false;

		// A2 X B2
		if ( glm::abs( C[ 0 ][ 2 ] * D[ 1 ] - C[ 1 ][ 2 ] * D[ 0 ] ) >
			A[ 0 ] * C[ 1 ][ 2 ] + A[ 1 ] * C[ 0 ][ 2 ] + B[ 0 ] * C[ 2 ][ 1 ] + B[ 1 ] * C[ 2 ][ 0 ] ) return false;

		return true;
	}

#undef A
#undef B

	friend class ComponentCollide;
};

bool OBB::will_get_desc = true;

#endif