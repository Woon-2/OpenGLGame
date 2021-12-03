#ifndef _coord
#define _coord

#include "glincludes.h"
#include <vector>
#include "pool.h"
#include "quaternion_rotate.h"

template < size_t Dimension, typename Elem_t >
class ComponentCoord
{
public:
	using Pos_t = glm::vec< Dimension, Elem_t >;
	using Diff_t = Pos_t;
	using Rad_t = Pos_t;
	using Scale_t = Pos_t;
	using Mat_t = glm::mat< Dimension, Dimension, Elem_t >;
	using CartesianMat_t = glm::mat< 4, 4, Elem_t >;
	using CartesianPos_t = glm::vec< 4, Elem_t >;
	static constexpr size_t POOL_SIZE = 10000;

private:
	struct DiffTag {};
	struct RadTag {};
	struct ScaleTag {};
	
	template < typename TransAtt, typename Tag >
	struct TransOPAdaptor
	{
		ComponentCoord& master;
		TransAtt& trans_attribute;

		void operator+=( const TransAtt& vec ) { master.add_trans_impl< Tag >( trans_attribute, vec ); }
		void operator-=( const TransAtt& vec ) { master.sub_trans_impl< Tag >( trans_attribute, vec ); }
		void operator*=( const TransAtt& vec ) { master.mul_trans_impl< Tag >( trans_attribute, vec ); }
		void operator/=( const TransAtt& vec ) { master.div_trans_impl< Tag >( trans_attribute, vec ); }

		void operator+=( const Elem_t scalar ) { master.add_trans_impl< Tag >( trans_attribute, scalar ); }
		void operator-=( const Elem_t scalar ) { master.sub_trans_impl< Tag >( trans_attribute, scalar ); }
		void operator*=( const Elem_t scalar ) { master.mul_trans_impl< Tag >( trans_attribute, scalar ); }
		void operator/=( const Elem_t scalar ) { master.div_trans_impl< Tag >( trans_attribute, scalar ); }

		TransOPAdaptor( ComponentCoord& master, TransAtt& trans_attribute )
			: master{ master }, trans_attribute{ trans_attribute } {}
	};

	struct MovementOPAdaptor : public TransOPAdaptor< Diff_t, DiffTag >
	{
		MovementOPAdaptor( ComponentCoord& master )
			: TransOPAdaptor< Diff_t, DiffTag >( master, master.noncached->trans.movement ) {}
	};

	struct RotationOPAdaptor : public TransOPAdaptor< Rad_t, RadTag >
	{
		RotationOPAdaptor( ComponentCoord& master )
			: TransOPAdaptor< Rad_t, RadTag >( master, master.noncached->trans.rotation ) {}
	};

	struct ScaleOPAdaptor : public TransOPAdaptor< Scale_t, ScaleTag >
	{
		ScaleOPAdaptor( ComponentCoord& master )
			: TransOPAdaptor< Scale_t, ScaleTag >( master, master.noncached->trans.scale ) {}
	};

public:
	MovementOPAdaptor movement;
	RotationOPAdaptor rotation;
	ScaleOPAdaptor scale;

	void set_shader_world_transform( GLuint world_transform_location ) const
	{
		glUniformMatrix4fv( world_transform_location, 1, false, glm::value_ptr( getmat() ) );
	}

	void adopt_base( typename pool< ComponentCoord >::Sptr base )
	{
		this->base = base;
		update_cache();
	}

	const Pos_t& getpivot() const
	{
		update_cache();
		return cached->pivot;
	}

	const Rad_t& getradians() const
	{
		update_cache();
		return cached->radians;
	}

	const Scale_t& getscales() const
	{
		update_cache();
		return cached->scales;
	}

	const CartesianMat_t& getmat() const
	{
		update_cache();
		return cached->mat;
	}

	ComponentCoord( const Pos_t& initial_pos = Pos_t{ static_cast< Elem_t >( 0 ) } )
		: cached{ pool_cached.alloc( initial_pos ) }, noncached{ pool_noncached.alloc( initial_pos ) }, base{ nullptr },
		movement{ *this }, rotation{ *this }, scale{ *this } {}

	ComponentCoord( const ComponentCoord& other )
		: cached{ pool_cached.alloc( *other.cached ) }, noncached{ pool_noncached.alloc( *other.noncached ) }, base{ other.base },
		movement{ *this }, rotation{ *this }, scale{ *this } {}

	ComponentCoord& operator=( const ComponentCoord& other )
	{
		if ( this != &other )
		{
			*cached = *other.cached;
			*noncached = *other.noncached;
			base = other.base;
		}

		return *this;
	}

	ComponentCoord( ComponentCoord&& other ) noexcept
		: cached{ std::move( other.cached ) }, noncached{ std::move( other.noncached ) }, base{ std::move( other.base ) },
		movement{ *this }, rotation{ *this }, scale{ *this } {}

	ComponentCoord& operator=( ComponentCoord&& other ) noexcept
	{
		if ( this != &other )
		{
			cached = std::move( other.cached );
			noncached = std::move( other.noncached );
			base = std::move( other.base );
		}

		return *this;
	}


private:
	struct Transform
	{
		Diff_t movement;
		Rad_t rotation;
		Scale_t scale;

		Transform() : movement{ static_cast< Elem_t >( 0 ) }, rotation{ static_cast< Elem_t >( 0 ) }, scale( static_cast< Elem_t >( 1 ) ) {}
	};

	struct Cached
	{
		enum class Flag {
			DIRTY = 0x01, ROTATED = 0x02, SCALED = 0x04, DERIVED_CACHE_INVALID_PIVOT = 0x08,
			DERIVED_CACHE_INVALID_RADIANS = 0x10, DERIVED_CACHE_INVALID_SCALES = 0x20
		};
		using Flag_t = int;

		mutable Pos_t pivot;
		mutable Rad_t radians;
		mutable Scale_t scales;
		mutable CartesianMat_t mat;
		mutable Flag_t flag;

		Cached( const Pos_t& initial_pos )
			: pivot{ initial_pos }, radians{ static_cast< Elem_t >( 0 ) }, scales{ static_cast< Elem_t >( 1 ) },
			mat{ static_cast< Elem_t >( 1 ) }, flag{ etoi( Flag::DERIVED_CACHE_INVALID_PIVOT ) |
			etoi( Flag::DERIVED_CACHE_INVALID_RADIANS ) | etoi( Flag::DERIVED_CACHE_INVALID_SCALES ) } {}
	};

	struct NonCached
	{
		Pos_t initial_pos;
		mutable CartesianMat_t mat;
		Transform trans;

		NonCached( const Pos_t& initial_pos )
			: initial_pos{ initial_pos }, mat{ static_cast< Elem_t >( 1 ) } {}
	};

	static pool< Cached > pool_cached;
	static pool< NonCached > pool_noncached;

	typename pool< Cached >::Uptr cached;
	typename pool< NonCached >::Uptr noncached;
	typename pool< ComponentCoord >::Sptr base;

	void update_cache() const
	{
		if ( dirty_check() )	update_pivot();
		if ( rotated_check() )	update_radians();
		if ( scaled_check() )	update_scales();
	}

	const bool dirty_check() const
	{
		if ( base ) return base->dirty_check() || ( cached->flag & etoi( Cached::Flag::DIRTY ) ) ||
			( base->cached->flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT ) );
		else		return cached->flag & etoi( Cached::Flag::DIRTY );
	}

	const bool rotated_check() const
	{
		if ( base ) return base->rotated_check() || ( cached->flag & etoi( Cached::Flag::ROTATED ) ) ||
			( base->cached->flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS ) );
		else		return cached->flag & etoi( Cached::Flag::ROTATED );
	}

	const bool scaled_check() const
	{
		if ( base ) return base->scaled_check() || ( cached->flag & etoi( Cached::Flag::SCALED ) ) ||
			( base->cached->flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES ) );
		else		return cached->flag & etoi( Cached::Flag::SCALED );
	}

	void update_pivot() const
	{
		auto& mat = noncached->mat;
		auto& trans = noncached->trans;

		if ( cached->flag & etoi( Cached::Flag::DIRTY ) )
		{
			mat = CartesianMat_t{ static_cast< Elem_t >( 1 ) };
			mat = glm::translate( mat, trans.movement );
			mat = rotate_quat( mat, trans.rotation );
			mat = glm::scale( mat, trans.scale );
		}

		if ( base )
		{
			cached->mat = base->getmat() * mat;
			cached->pivot = cached->mat * noncached->initial_pos;

			base->cached->flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT );
		}
		else
		{
			cached->mat = mat;
			cached->pivot = mat * noncached->initial_pos;
		}

		cached->flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT );
		cached->flag &= ~etoi( Cached::Flag::DIRTY );
	}

	void update_radians() const
	{
		if ( base )
		{
			cached->radians = base->getradians() + noncached->trans.rotation;

			base->cached->flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS );
		}
		else cached->radians = noncached->trans.rotation;

		cached->flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS );
		cached->flag &= ~etoi( Cached::Flag::ROTATED );
	}

	void update_scales() const
	{
		if ( base )
		{
			cached->scales = base->getscales() + noncached->trans.scale;

			base->cached->flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES );
		}
		else cached->scales = noncached->trans.scale;

		cached->flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES );
		cached->flag &= ~etoi( Cached::Flag::SCALED );
	}

	template < typename Tag, typename Ty1, typename Ty2 >
	void add_trans_impl( Ty1& target, const Ty2& val )
	{
		target += val;
		invalidate_cache< Tag >();
	}

	template < typename Tag, typename Ty1, typename Ty2 >
	void sub_trans_impl( Ty1& target, const Ty2& val )
	{
		target -= val;
		invalidate_cache< Tag >();
	}

	template < typename Tag, typename Ty1, typename Ty2 >
	void set_trans_impl( Ty1& target, const Ty2& val )
	{
		target = val;
		invalidate_cache< Tag >();
	}

	template < typename Tag, typename Ty1, typename Ty2 >
	void mul_trans_impl( Ty1& target, const Ty2& val )
	{
		target *= val;
		invalidate_cache< Tag >();
	}

	template < typename Tag, typename Ty1, typename Ty2 >
	void div_trans_impl( Ty1& target, const Ty2& val )
	{
		target /= val;
		invalidate_cache< Tag >();
	}

	template < typename Tag >
	void invalidate_cache()
	{
		if constexpr ( std::is_same_v< Tag, RadTag > )			cached->flag |= etoi( Cached::Flag::ROTATED );
		else if constexpr ( std::is_same_v< Tag, ScaleTag > )	cached->flag |= etoi( Cached::Flag::SCALED );

		cached->flag |= etoi( Cached::Flag::DIRTY );
	}
};

template< size_t Dimension, typename Elem_t >
pool< typename ComponentCoord< Dimension, Elem_t >::Cached >
ComponentCoord< Dimension, Elem_t >::pool_cached{ ComponentCoord< Dimension, Elem_t >::POOL_SIZE };

template< size_t Dimension, typename Elem_t >
pool< typename ComponentCoord< Dimension, Elem_t >::NonCached >
ComponentCoord< Dimension, Elem_t >::pool_noncached{ ComponentCoord< Dimension, Elem_t >::POOL_SIZE };

#endif