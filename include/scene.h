#ifndef _scene
#define _scene

#include "server.h"
#include "event_handler.h"

template < template< typename > class EventHandler_Ptr_t = std::unique_ptr >
class Scene
{
public:
	using EventHandlerPtr = EventHandler_Ptr_t< EventHandler >;

	virtual void update() = 0;
	virtual void render() = 0;

	virtual ~Scene() {}

	Scene( EventHandlerPtr&& event_handler ) : event_handler{ std::move( event_handler ) } {}

protected:
	EventHandlerPtr event_handler;
};

#endif