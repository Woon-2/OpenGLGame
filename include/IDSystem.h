#ifndef _IDSystem
#define _IDSystem

#include <unordered_set>
#include <stack>
#include "indent.h"

class ComponentID
{
public:
	using ID_t = int;
	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "GID: " << id << '\n';
			ss << fore_indent << indent << "ID: " << gid << '\n';
		}

		return ss;
	}

	void setgid( const int group_id ) { gid = group_id; }
	const ID_t getid() const { return id; }
	const ID_t getgid() const { return gid; }

	ComponentID() : id{ allocid() } {}
	ComponentID( const ComponentID& other ) : id{ allocid() } {}
	ComponentID& operator=( const ComponentID& other ) { return *this; }
	~ComponentID() { deallocid(); }

private:
	ID_t id;
	ID_t gid;
	static std::unordered_set< ID_t > ids_active;
	static std::stack< ID_t > ids_inactive;

	static const ID_t allocid()
	{
		if ( ids_inactive.empty() )
		{
			ID_t id = ids_active.size();

			ids_active.insert( id );

			return id;
		}
		else
		{
			ID_t id = ids_inactive.top();
			ids_inactive.pop();

			ids_active.insert( id );

			return id;
		}
	}

	void deallocid()
	{
		ids_active.erase( id );
		ids_inactive.push( id );
	}
};

template < typename Ty >
struct ID_Inst_Pair
{
	ComponentID component_id;
	Ty inst;

	ID_Inst_Pair( const Ty& inst ) : component_id{}, inst{ inst } {}
	ID_Inst_Pair( Ty&& inst ) : component_id{}, inst{ std::move( inst ) } {}
};

bool ComponentID::will_get_desc = true;

#endif