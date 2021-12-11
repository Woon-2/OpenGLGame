#ifndef _collision
#define _collision

#include "AABB.h"
#include "OBB.h"
#include <vector>

class ComponentCollide
{
public:
	template < typename T >
	using Cont = std::vector< T >;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "AABB count: " << aabbs.size() << '\n';
			ss << fore_indent << indent << "OBB count: " << obbs.size() << '\n';
			
			ss << fore_indent << indent << "AABBs: " << '\n';
			for ( const auto& aabb : aabbs )
			{
				ss << aabb.get_desc( fore_indent + indent ).rdbuf();
			}

			ss << fore_indent << indent << "OBBs: " << '\n';
			for ( const auto& obb : obbs )
			{
				ss << obb.get_desc( fore_indent + indent ).rdbuf();
			}
		}

		return ss;
	}

	Cont< AABB > aabbs;
	Cont< OBB > obbs;

	const bool collide( const ComponentCollide& other ) const
	{
		for ( const auto& aabb : aabbs )
		{
			for ( const auto& oaabb : other.aabbs )
			{
				if ( aabb.collide( oaabb ) ) return true;
			}

			for ( const auto& oobb : other.obbs )
			{
				if ( aabb.collide( make_AABB_from_OBB( oobb ) ) ) return true;
			}
		}

		for ( const auto& obb : obbs )
		{
			for ( const auto& oobb : other.obbs )
			{
				if ( obb.collide( oobb ) ) return true;
			}

			for ( const auto& oaabb : other.aabbs )
			{
				if ( obb.collide( make_OBB_from_AABB( oaabb ) ) ) return true;
			}
		}

		return false;
	}

private:
	static OBB make_OBB_from_AABB( const AABB& aabb )
	{
		OBB ret;
		ret.init( aabb.coord->get_base(), aabb.half_len, aabb.movement );
		return ret;
	}

	static AABB make_AABB_from_OBB( const OBB& obb )
	{
		AABB ret;
		ret.init( obb.coord->get_base(), obb.half_len, obb.movement );
		return ret;
	}

};

bool ComponentCollide::will_get_desc = true;

#endif