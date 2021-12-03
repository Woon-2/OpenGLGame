#ifndef _ordinal
#define _ordinal

#include <string>

const std::string Ord( const int n )
{
	switch ( n % 10 )
	{
	default:
		return "th";

	case 1:
		return "st";

	case 2:
		return "nd";

	case 3:
		return "rd";
	}
}

#endif