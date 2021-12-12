#ifndef _AABB
#define _AABB

#include "BBBase.h"

class AABB : public BBBase
{
public:
	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << BBBase::get_desc( fore_indent + indent ).rdbuf();
		}

		return ss;
	}

	const bool collide( const AABB& other ) const
	{
		const auto pi = coord->getpivot();
		const auto opi = coord->getpivot();

		auto half_len = get_half_len();

		for ( int i = 0; i < half_len.length(); ++i )
		{
			auto my_min = pi[ i ] - half_len[ i ];
			auto my_max = pi[ i ] + half_len[ i ];
			auto other_min = opi[ i ] - half_len[ i ];
			auto other_max = opi[ i ] + half_len[ i ];

			if ( my_min > other_max ) return false;
			if ( other_min > my_max ) return false;
		}

		return true;
	}

	friend class ComponentCollide;
};

bool AABB::will_get_desc = true;

#endif