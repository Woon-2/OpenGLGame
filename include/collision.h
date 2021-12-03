#ifndef _collision
#define _collision

#include "AABB.h"
#include "OBB.h"
#include <vector>

template < size_t Dimension, typename Elem_t >
class ComponentCollide
{
public:
	template < typename T >
	using Cont = std::vector< T >;

	Cont< AABB< Dimension, Elem_t > > aabbs;
	Cont< OBB< Dimension, Elem_t > > obbs;

	template < typename OElem_t >
	const bool collide( const ComponentCollide< Dimension, OElem_t >& other ) const
	{
		for ( const auto& aabb : aabbs )
		{
			for ( const auto& oaabb : other.aabbs )
			{
				if ( aabb.collide( oaabb ) ) return true;
			}
		}

		for ( const auto& obb : obbs )
		{
			for ( const auto& oobb : other.obbs )
			{
				if ( obb.collide( oobb ) ) return true;
			}
		}

		return false;
	}

};

#endif