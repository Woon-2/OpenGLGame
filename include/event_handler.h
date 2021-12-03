#ifndef _event_handler
#define _event_handler

#include "MWEngineLow.h"
#include <queue>
#include <deque>
#include <unordered_map>

template < size_t group >
class EventHandler
{
public:
	ComponentID< group > id_component;

	using ID_t = ComponentID<>::ID_t;
	using Event_t = int;
	using EventQueue = std::queue< Event_t >;
	using Hash = std::unordered_map < ID_t, EventQueue >;

	void add_event( const Event_t ev ) { event_queue.push( ev ); }
	static void add_event( const ID_t id, const Event_t ev ) { insts[ id ].push( ev ); }
	virtual void run() = 0;

protected:
	EventQueue event_queue;
	
	static Hash insts;
};

template < size_t group >
typename EventHandler< group >::Hash EventHandler< group >::insts;


template < size_t group >
class DefaultEventHandler : public EventHandler< group >
{
public:
	void run() override { this->event_queue.clear(); }
};
#endif