#ifndef _heapalloc
#define _heapalloc

#include <memory>

template < typename Ty >
struct is_smart_pointer
{
	static constexpr bool value = false;
};

template < typename Ty, typename Dx >
struct is_smart_pointer< std::unique_ptr< Ty, Dx > >
{
	static constexpr bool value = false;
};

template < typename Ty >
struct is_smart_pointer< std::shared_ptr< Ty > >
{
	static constexpr bool value = true;
};

template < typename Ty >
constexpr bool is_smart_pointer_v = is_smart_pointer< Ty >::value;

template< typename Ty, typename Pointer, typename ... Args >
Pointer heap_alloc( Args&& ... args )
{
	if constexpr ( std::is_pointer_v< Pointer > )
	{
		return new Ty{ std::forward< Args >( args )... };
	}
	else if constexpr ( is_smart_pointer_v< Pointer > )
	{
		return Pointer{ new Ty{ std::forward< Args >( args )... } };
	}
	else
	{
		static_assert( false, "heap_alloc< Ty, Pointer >: Pointer type is not supported." );
	}
}

template < typename Pointer >
auto get_raw_pointer( Pointer& ptr )
{
	if constexpr ( std::is_pointer_v< Pointer > )
	{
		return ptr;
	}
	else
	{
		return ptr.operator->();
	}
}

#endif