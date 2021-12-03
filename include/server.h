#ifndef _server
#define _server

#include <unordered_map>
#include "MWEngineLow.h"

template < size_t vecDimension, typename vecElem_t, size_t group = 0u, template< typename > class Inst_t = std::void_t >
class Server
{
public:
	using ID_t = ComponentID<>::ID_t;

	template < typename Ty >
	using Hash = std::unordered_map< ID_t, Ty >;

	template < typename Ty >
	using Dispatched_t = std::conditional_t< std::is_same_v< std::void_t<>, Inst_t >,
		Ty, Inst_t< Ty > >;

	using VC = Dispatched_t< ComponentVertex >;
	using RC = Dispatched_t< ComponentRender >;
	using CC = Dispatched_t< ComponentCoord< vecDimension, vecElem_t > >;
	using PC = Dispatched_t< ComponentPhysic< vecDimension, vecElem_t > >;

	Hash< Hash< VC > > vertex_components;
	Hash< Hash< RC > > render_components;
	Hash< Hash< CC > > coord_components;
	Hash< Hash< PC > > physic_components;
};

#endif