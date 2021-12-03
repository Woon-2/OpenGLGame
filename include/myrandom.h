#ifndef _myrandom
#define _myrandom

#include <random>

namespace _rv
{
	std::random_device rd;
	std::default_random_engine dre( rd() );
}

const auto random_value( const std::uniform_int_distribution<>& uid )
{
	return uid( _rv::dre );
}

const auto random_value( const int lower_bound, const int upper_bound )
{
	return random_value( std::uniform_int_distribution<>{ lower_bound, upper_bound } );
}

const auto random_value( const std::uniform_real_distribution<>& urd )
{
	return urd( _rv::dre );
}

const auto random_value( const double lower_bound, const double upper_bound )
{
	return random_value( std::uniform_real_distribution<>{lower_bound, upper_bound} );
}

const float random_valuef( const double lower_bound, const double upper_bound )
{
	return static_cast< float >( random_value( lower_bound, upper_bound ) );
}

#endif