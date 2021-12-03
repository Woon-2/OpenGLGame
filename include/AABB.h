#ifndef _AABB
#define _AABB

#include "glincludes.h"

template < size_t Dimension, typename Elem_t >
struct AABB
{
	using Pos_t = glm::vec< Dimension, Elem_t >;

	Pos_t mins;
	Pos_t maxs;

	template < typename OElem_t >
	const bool collide( const AABB< Dimension, OElem_t >& other )
	{
		for ( decltype( Dimension ) i = 0; i < Dimension; ++i )
		{
			if ( mins[ i ] > static_cast< Elem_t >( other.maxs[ i ] ) ) return false;
			if ( maxs[ i ] < static_cast< Elem_t >( other.mins[ i ] ) ) return false;
		}

		return true;
	}

	AABB( const Pos_t& mins = Pos_t{ static_cast< Elem_t >( 0 ) }, const Pos_t& maxs = Pos_t{ static_cast< Elem_t >( 0 ) } )
		: mins{ mins }, maxs{ maxs } {}
};

#endif