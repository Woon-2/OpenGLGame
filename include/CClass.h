#ifndef _CClass
#define _CClass

#include "pointer.h"
#include "glincludes.h"

template < typename Class, typename Pointer >
class CClass {};

class Texture;
class ComponentVertex;
class ComponentRender;
class ComponentCoord;
class ComponentID;
class ComponentCollide;
class ComponentPhysic;
class GameObject;

template < typename Pointer >
class CClass< Texture, Pointer >
{
public:
	void init( const char* file_name, GLuint location_tex ) { ptr = heap_alloc< Texture, Pointer >( file_name, location_tex ); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	Texture& operator*() noexcept { return *ptr; }
	const Texture& operator*() const noexcept { return *ptr; }
	Texture* operator->() noexcept { return get_raw_pointer( ptr ); }
	const Texture* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentVertex, Pointer >
{
public:
	void init( GLuint shader_program_id ) { ptr = heap_alloc< ComponentVertex, Pointer >( shader_program_id ); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentVertex& operator*() noexcept { return *ptr; }
	const ComponentVertex& operator*() const noexcept { return *ptr; }
	ComponentVertex* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentVertex* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentRender, Pointer >
{
public:
	void init() { ptr = heap_alloc< ComponentRender, Pointer >(); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentRender& operator*() noexcept { return *ptr; }
	const ComponentRender& operator*() const noexcept { return *ptr; }
	ComponentRender* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentRender* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentCoord, Pointer >
{
public:
	void init( const glm::vec3& initial_pos = glm::vec3{ 0.f, 0.f, 0.f } ) { ptr = heap_alloc< ComponentCoord, Pointer >( initial_pos ); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentCoord& operator*() noexcept { return *ptr; }
	const ComponentCoord& operator*() const noexcept { return *ptr; }
	ComponentCoord* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentCoord* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentID, Pointer >
{
public:
	void init() { ptr = heap_alloc< ComponentID, Pointer >(); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentID& operator*() noexcept { return *ptr; }
	const ComponentID& operator*() const noexcept { return *ptr; }
	ComponentID* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentID* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentCollide, Pointer >
{
public:
	void init() { ptr = heap_alloc< ComponentCollide, Pointer >(); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentCollide& operator*() noexcept { return *ptr; }
	const ComponentCollide& operator*() const noexcept { return *ptr; }
	ComponentCollide* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentCollide* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< ComponentPhysic, Pointer >
{
public:
	void init() { ptr = heap_alloc< ComponentPhysic, Pointer >(); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	ComponentPhysic& operator*() noexcept { return *ptr; }
	const ComponentPhysic& operator*() const noexcept { return *ptr; }
	ComponentPhysic* operator->() noexcept { return get_raw_pointer( ptr ); }
	const ComponentPhysic* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};

template < typename Pointer >
class CClass< GameObject, Pointer >
{
public:
	void init() { ptr = heap_alloc< GameObject, Pointer >(); }
	void release() { ptr = nullptr; }
	~CClass() { release(); }
	GameObject& operator*() noexcept { return *ptr; }
	const GameObject& operator*() const noexcept { return *ptr; }
	GameObject* operator->() noexcept { return get_raw_pointer( ptr ); }
	const GameObject* operator->() const noexcept { return get_raw_pointer( ptr ); }
	operator bool() const noexcept { return static_cast<bool>( get_raw_pointer( ptr ) ); }

private:
	Pointer ptr;
};


using CTexture = CClass< Texture, Texture* >;
using CRender = CClass< ComponentRender, ComponentRender* >;
using CVertex = CClass< ComponentVertex, ComponentVertex* >;
using CCoord = CClass< ComponentCoord, ComponentCoord* >;
using CID = CClass< ComponentID, ComponentID* >;
using CCollide = CClass< ComponentCollide, ComponentCollide* >;
using CPhysic = CClass< ComponentPhysic, ComponentPhysic* >;
using CGameObject = CClass< GameObject, GameObject* >;

#endif