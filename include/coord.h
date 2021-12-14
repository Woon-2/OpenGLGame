#ifndef _coord
#define _coord

#include "glincludes.h"
#include <vector>
#include "pool.h"
#include "quaternion_rotate.h"
#include "TMP.h"
#include "pointer.h"
#include "CClass.h"

class ComponentCoord
{
public:
	static constexpr const int Dimension = 3;
	using Pos_t = glm::vec3;
	using Elem_t = float;
	using Diff_t = Pos_t;
	using Rad_t = Pos_t;
	using Scale_t = Pos_t;
	using Mat_t = glm::mat3;
	using CartesianMat_t = glm::mat4;
	using CartesianPos_t = glm::vec4;
	static constexpr size_t POOL_SIZE = 100000;

private:
	struct DiffTag {};
	struct RadTag {};
	struct ScaleTag {};

	template < typename TransAtt, typename Tag >
	struct TransOPAdaptor
	{
		ComponentCoord* master;
		TransAtt* trans_attribute;

		void operator=( const TransAtt& vec ) { master->cop_trans_impl< Tag >( *trans_attribute, vec ); }
		void operator+=( const TransAtt& vec ) { master->add_trans_impl< Tag >( *trans_attribute, vec ); }
		void operator-=( const TransAtt& vec ) { master->sub_trans_impl< Tag >( *trans_attribute, vec ); }
		void operator*=( const TransAtt& vec ) { master->mul_trans_impl< Tag >( *trans_attribute, vec ); }
		void operator/=( const TransAtt& vec ) { master->div_trans_impl< Tag >( *trans_attribute, vec ); }

		void operator+=( const Elem_t scalar ) { master->add_trans_impl< Tag >( *trans_attribute, scalar ); }
		void operator-=( const Elem_t scalar ) { master->sub_trans_impl< Tag >( *trans_attribute, scalar ); }
		void operator*=( const Elem_t scalar ) { master->mul_trans_impl< Tag >( *trans_attribute, scalar ); }
		void operator/=( const Elem_t scalar ) { master->div_trans_impl< Tag >( *trans_attribute, scalar ); }
	};

public:
	TransOPAdaptor< Diff_t, DiffTag > movement;
	TransOPAdaptor< Rad_t, RadTag > rotation;
	TransOPAdaptor< Scale_t, ScaleTag > scale;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << "Mem: " << this << '\n';

			ss << fore_indent << indent << "base: " << &*base << '\n';
			ss << fore_indent << indent << "pivot: ( " << getpivot() << " )\n";
			ss << fore_indent << indent << "radians: ( " << getradians() << " )\n";
			ss << fore_indent << indent << "scales: ( " << getscales() << " )\n";
			ss << fore_indent << indent << "movement: ( " << noncached.trans.movement << " )\n";
			ss << fore_indent << indent << "rotation: ( " << noncached.trans.rotation << " )\n";
			ss << fore_indent << indent << "scale: ( " << noncached.trans.scale << " )\n";
		}

		return ss;
	}

	void set_shader_world_transform( GLuint world_transform_location ) const
	{
		glUniformMatrix4fv( world_transform_location, 1, false, glm::value_ptr( getmat() ) );
	}

	void adopt_base( const CCoord& base )
	{
		this->base = base;
		update_cache();
	}

	const CCoord get_base() const
	{
		return base;
	}

	const Pos_t& getpivot() const
	{
		update_cache();
		return cached.pivot;
	}

	const Pos_t& get_initial_pos() const
	{
		return noncached.initial_pos;
	}

	const Rad_t& getradians() const
	{
		update_cache();
		return cached.radians;
	}

	const Scale_t& getscales() const
	{
		update_cache();
		return cached.scales;
	}

	const CartesianMat_t& getmat() const
	{
		update_cache();
		return cached.mat;
	}

	ComponentCoord( const Pos_t& initial_pos = Pos_t{ static_cast< Elem_t >( 0 ) } )
		: cached{ initial_pos }, noncached{ initial_pos }
	{
		init_adaptor();
	}

	ComponentCoord( const ComponentCoord& other )
		: cached{ other.cached }, noncached{ other.noncached }, base{ other.base }
	{
		init_adaptor();
	}

	ComponentCoord& operator=( const ComponentCoord& other )
	{
		if ( this != &other )
		{
			cached = other.cached;
			noncached = other.noncached;
			base = other.base;
		}

		return *this;
	}

	ComponentCoord( ComponentCoord&& other ) noexcept
		: cached{ std::move( other.cached ) }, noncached{ std::move( other.noncached ) }, base{ std::move( other.base ) },
		movement{}, rotation{}, scale{}
	{
		init_adaptor();
	}

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

		Transform() : movement{ static_cast<Elem_t>( 0 ) }, rotation{ static_cast<Elem_t>( 0 ) }, scale( static_cast<Elem_t>( 1 ) ) {}
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
			: pivot{ initial_pos }, radians{ static_cast<Elem_t>( 0 ) }, scales{ static_cast<Elem_t>( 1 ) },
			mat{ static_cast<Elem_t>( 1 ) }, flag{ etoi( Flag::DERIVED_CACHE_INVALID_PIVOT ) |
			etoi( Flag::DERIVED_CACHE_INVALID_RADIANS ) | etoi( Flag::DERIVED_CACHE_INVALID_SCALES ) } {}
	};

	struct NonCached
	{
		Pos_t initial_pos;
		mutable CartesianMat_t mat;
		Transform trans;

		NonCached( const Pos_t& initial_pos )
			: initial_pos{ initial_pos }, mat{ static_cast<Elem_t>( 1 ) } {}
	};

	Cached cached;
	NonCached noncached;
	CCoord base;

	void init_adaptor()
	{
		movement.master = this;
		movement.trans_attribute = &noncached.trans.movement;
		rotation.master = this;
		rotation.trans_attribute = &noncached.trans.rotation;
		scale.master = this;
		scale.trans_attribute = &noncached.trans.scale;
	}

	void update_cache() const
	{
		if ( dirty_check() )	update_pivot();
		if ( rotated_check() )	update_radians();
		if ( scaled_check() )	update_scales();
	}

	const bool dirty_check() const
	{
		if ( base ) return base->dirty_check() || ( cached.flag & etoi( Cached::Flag::DIRTY ) ) ||
			( base->cached.flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT ) );
		else		return cached.flag & etoi( Cached::Flag::DIRTY );
	}

	const bool rotated_check() const
	{
		if ( base ) return base->rotated_check() || ( cached.flag & etoi( Cached::Flag::ROTATED ) ) ||
			( base->cached.flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS ) );
		else		return cached.flag & etoi( Cached::Flag::ROTATED );
	}

	const bool scaled_check() const
	{
		if ( base ) return base->scaled_check() || ( cached.flag & etoi( Cached::Flag::SCALED ) ) ||
			( base->cached.flag & etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES ) );
		else		return cached.flag & etoi( Cached::Flag::SCALED );
	}

	void update_pivot() const
	{
		auto& mat = noncached.mat;
		auto& trans = noncached.trans;

		if ( cached.flag & etoi( Cached::Flag::DIRTY ) )
		{
			mat = CartesianMat_t{ static_cast<Elem_t>( 1 ) };
			mat = glm::translate( mat, trans.movement );
			mat = rotate_quat( mat, trans.rotation );
			mat = glm::scale( mat, trans.scale );
		}

		if ( base )
		{
			cached.mat = base->getmat() * mat;
			cached.pivot = cached.mat * CartesianPos_t{ noncached.initial_pos, 1.f };

			// base->cached.flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT );
		}
		else
		{
			cached.mat = mat;
			cached.pivot = mat * CartesianPos_t{ noncached.initial_pos, 1.f };
		}

		cached.flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_PIVOT );
		cached.flag &= ~etoi( Cached::Flag::DIRTY );
	}

	void update_radians() const
	{
		if ( base )
		{
			cached.radians = base->getradians() + noncached.trans.rotation;

			base->cached.flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS );
		}
		else cached.radians = noncached.trans.rotation;

		cached.flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_RADIANS );
		cached.flag &= ~etoi( Cached::Flag::ROTATED );
	}

	void update_scales() const
	{
		if ( base )
		{
			cached.scales = base->getscales() * noncached.trans.scale;

			base->cached.flag &= ~etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES );
		}
		else cached.scales = noncached.trans.scale;

		cached.flag |= etoi( Cached::Flag::DERIVED_CACHE_INVALID_SCALES );
		cached.flag &= ~etoi( Cached::Flag::SCALED );
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

	template < typename Tag, typename Ty1, typename Ty2 >
	void cop_trans_impl( Ty1& target, const Ty2& val )
	{
		target = val;
		invalidate_cache< Tag >();
	}

	template < typename Tag >
	void invalidate_cache()
	{
		if constexpr ( std::is_same_v< Tag, RadTag > )			cached.flag |= etoi( Cached::Flag::ROTATED );
		else if constexpr ( std::is_same_v< Tag, ScaleTag > )	cached.flag |= etoi( Cached::Flag::SCALED );

		cached.flag |= etoi( Cached::Flag::DIRTY );
	}
};

bool ComponentCoord::will_get_desc = true;

#endif