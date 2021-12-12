#ifndef _physics
#define _physics

#include "coord.h"
#include <numeric>

class ComponentPhysic
{
public:
	static constexpr const int Dimension = 3;
	using vec_t = glm::vec3;
	using Elem_t = float;
	using scalar_t = Elem_t;
	using time_t = Elem_t;

	vec_t velocity;
	vec_t accel;
	// radians
	vec_t velocity_angle;
	// radians
	vec_t accel_angle;
	time_t time_unit;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "velocity: " << velocity << '\n';
			ss << fore_indent << indent << "accel: " << accel << '\n';
			ss << fore_indent << indent << "velocity_angle: " << velocity_angle << '\n';
			ss << fore_indent << indent << "accel_angle: " << accel_angle << '\n';
			ss << fore_indent << indent << "time_unit: " << time_unit << '\n';
		}

		return ss;
	}

	template < typename TargetPos, typename TargetRadians >
	void update( TargetPos& pos, TargetRadians& radians, const time_t delta_time )
	{
		auto delta_time_unit = static_cast<Elem_t>( delta_time / time_unit );

		int i = 0;
		for ( ; i < Dimension; ++i )
		{
			if ( glm::abs( accel[ i ] ) > std::numeric_limits< Elem_t >::epsilon() * 10 )
			{
				auto delta_velocity = accel * delta_time_unit;
				pos += ( velocity + delta_velocity / static_cast<Elem_t>( 2 ) ) * delta_time_unit;
				velocity += delta_velocity * delta_time_unit;
				break;
			}
		}
		if ( i == Dimension ) pos += velocity * delta_time_unit;

		for ( i = 0; i < Dimension; ++i )
		{
			if ( glm::abs( accel_angle[ i ] ) > std::numeric_limits< Elem_t >::epsilon() * 10 )
			{
				auto delta_velocity_angle = accel_angle * delta_time_unit;
				radians += ( velocity_angle + delta_velocity_angle / static_cast<Elem_t>( 2 ) ) * delta_time_unit;
				velocity_angle += delta_velocity_angle * delta_time_unit;
				break;
			}
		}
		if ( i == Dimension ) radians += velocity_angle * delta_time_unit;
	}

	ComponentPhysic( const time_t time_unit = static_cast<time_t>( 1 ), const vec_t& velocity = vec_t{ 0 },
		const vec_t& accel = vec_t{ 0 }, const vec_t& velocity_angle = vec_t{ 0 }, const vec_t& accel_angle = vec_t{ 0 } )
		: time_unit{ time_unit }, velocity{ velocity }, accel{ accel }, velocity_angle{ velocity_angle }, accel_angle{ accel_angle } {}
};

bool ComponentPhysic::will_get_desc = true;

#endif