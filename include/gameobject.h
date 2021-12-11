#ifndef _gameobject
#define _gameobject

#include "region.h"

class GameObject
{
public:
	using Time_t = float;
	virtual void update( const Time_t delta_time, Region& region ) = 0;
	virtual void render() const = 0;
};

#endif