#ifndef _OBB
#define _OBB

#include "glincludes.h"
#include "quaternion_rotate.h"

template < size_t Dimension, typename Elem_t >
class OBB
{
public:
	using Pos_t = glm::vec< Dimension, Elem_t >;
	using Diff_t = Pos_t;
	using Rad_t = Pos_t;
	using Mat_t = glm::mat< Dimension, Dimension, Elem_t >;
	using CartesianMat_t = glm::mat< 4, 4, Elem_t >;
	using CartesianPos_t = glm::vec< 4, Elem_t >;

	// 회전 변환에 쓰이는 각도만 따로 모아 OBB의 각도를 변환해주어야 함!
	Rad_t radians;

	// 스케일 변환만 따로 모아 axis_half_len을 변환해주어야 함!
	Pos_t axis_half_len;

public:
	void transform( const CartesianMat_t& transmat )
	{
		pivot = transmat * CartesianPos_t{ pivot, static_cast< Elem_t >( 1 ) };
	}

#define A axis_half_len
#define B other.axis_half_len

	const Mat_t make_axis_dirs() const
	{
		CartesianMat_t cartesian_dirs{ static_cast< Elem_t >( 1 ) };

		if constexpr ( Dimension == 2 )
		{
			cartesian_dirs = glm::rotate( cartesian_dirs, radians.x, glm::vec< 3, Elem_t >{ static_cast< Elem_t >( 1 ), 0, 0 } );
			cartesian_dirs = glm::rotate( cartesian_dirs, radians.y, glm::vec< 3, Elem_t >{ 0, static_cast< Elem_t >( 1 ), 0 } );
		}
		else if constexpr ( Dimension == 3 )
		{
			cartesian_dirs = rotate_quat( radians );
		}

		return cartesian_dirs;
	}

	template< typename OElem_t >
	const bool collide( const OBB< Dimension, OElem_t >& other )
	{
		Diff_t diff = other.pivot - pivot;
		static constexpr Elem_t cutoff = static_cast< Elem_t >( 0.999999 );

		// 만약 이상하다면 C 전치시키기
		auto axis_dirs = make_axis_dirs();
		auto oaxis_dirs = other.make_axis_dirs();
		Mat_t C = glm::transpose( axis_dirs ) * oaxis_dirs;
		Diff_t D;

		bool exists_parallel_pair = false;


		for ( decltype( Dimension ) i = 0; i < Dimension; ++i )
		{
			for ( decltype( Dimension ) j = 0; j < Dimension; ++j )
			{
				C[ i ][ j ] = glm::abs( C[ i ][ j ] );
				if ( C[ i ][ j ] > cutoff ) exists_parallel_pair = true;
			}

			D[ i ] = glm::abs( glm::dot( diff, axis_dirs[ i ] ) );
			Diff_t OD = glm::abs( glm::dot( diff, oaxis_dirs[ i ] ) );

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

	OBB( const Pos_t& pivot = Pos_t{ static_cast< Elem_t >( 0 ) },
		const Diff_t& axis_half_len = Diff_t{ static_cast< Elem_t >( 0 ) },
		const Rad_t& radians = Rad_t{ static_cast< Elem_t >( 0 ) } )
		: pivot{ pivot }, axis_half_len{ axis_half_len }, radians{ radians } {}

private:
	Pos_t pivot;
};

#endif