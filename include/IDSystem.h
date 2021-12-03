#ifndef _IDSystem
#define _IDSystem

#include <unordered_set>
#include <stack>

template < size_t Group = 0u >
class ComponentID
{
public:
	using ID_t = int;
	const ID_t getid() const { return id; }

	ComponentID() : id{ allocid() } {}
	ComponentID( const ComponentID& other ) : id{ allocid() } {}
	ComponentID& operator=( const ComponentID& other ) { return *this; }
	~ComponentID() { deallocid(); }

private:
	ID_t id;
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

template < typename Ty, size_t Group = 0u >
struct ID_Inst_Pair
{
	ComponentID< Group > component_id;
	Ty inst;

	ID_Inst_Pair( const Ty& inst ) : component_id{}, inst{ inst } {}
	ID_Inst_Pair( Ty&& inst ) : component_id{}, inst{ std::move( inst ) } {}
};

#endif