#ifndef _gameobject
#define _gameobject

#include "MWEngineLow.h"
#include "event_handler.h"
#include <vector>

template <
	size_t vecDimension, typename vecElem_t, size_t group = 0u,
	template< typename > class Cont = std::vector,
	template< typename > class EventHandler_Ptr_t = std::unique_ptr,
	template< typename > class Inst_t = std::void_t
>
class GameObject
{
public:
	ComponentID< group > component_id;
	using EventHandlerPtr = EventHandler_Ptr_t< EventHandler< group > >;

	virtual void update() = 0;
	virtual void render() = 0;

	GameObject( EventHandlerPtr&& event_handler )
		: component_id{}, component_vertices{}, component_renders{}, component_coords{}, component_physics{},
		event_handler{ std::move( event_handler ) } {}

	virtual ~GameObject() {}

protected:
	template < typename Ty >
	using Dispatched_t = std::conditional_t< std::is_same_v< std::void_t<>, Inst_t >,
		Ty, Inst_t< Ty > >;

	using VC = ID_Inst_Pair< Dispatched_t< ComponentVertex >, group >;
	using RC = ID_Inst_Pair< Dispatched_t< ComponentRender >, group >;
	using CC = ID_Inst_Pair< Dispatched_t< ComponentCoord< vecDimension, vecElem_t > >, group >;
	using PC = ID_Inst_Pair< Dispatched_t< ComponentPhysic< vecDimension, vecElem_t > >, group >;

	Cont< VC > component_vertices;
	Cont< RC > component_renders;
	Cont< CC > component_coords;
	Cont< PC > component_physics;
	EventHandlerPtr event_handler;
};

template <
	size_t group = 0u,
	template< typename > class Cont = std::vector,
	template< typename > class EventHandler_Ptr_t = std::unique_ptr
>
class GameIDObject
{
public:
	ComponentID< group > component_id;
	using EventHandlerPtr = EventHandler_Ptr_t< EventHandler< group > >;
	using ID_t = typename ComponentID< group >::ID_t;

	virtual void update() = 0;
	virtual void render() = 0;

	GameIDObject( EventHandlerPtr&& event_handler )
		: component_id{}, component_vertices{}, component_renders{}, component_coords{}, component_physics{},
		event_handler{ std::move( event_handler ) } {}

	virtual ~GameIDObject() {}

protected:
	using VC = ComponentID< group >;
	using RC = ComponentID< group >;
	using CC = ComponentID< group >;
	using PC = ComponentID< group >;

	Cont< VC > component_vertices;
	Cont< RC > component_renders;
	Cont< CC > component_coords;
	Cont< PC > component_physics;
	EventHandlerPtr event_handler;
};

#endif