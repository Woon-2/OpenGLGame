#ifndef _camera
#define _camera

#include "coord.h"
#include "physics.h"
#include "screen.h"

template < size_t Dimension, typename Elem_t >
class Camera
{
private:
	using Rad_Scalar_t = Elem_t;
	using Pos_Scalar_t = Elem_t;
	using Scale_Scalar_t = Elem_t;
	using Time_t = Elem_t;

	struct PerspectiveAttribute
	{
		Rad_Scalar_t fovy;
		Scale_Scalar_t aspect;
		Pos_Scalar_t znear, zfar;

		constexpr explicit PerspectiveAttribute( const Rad_Scalar_t fovy, const Scale_Scalar_t aspect,
			const Pos_Scalar_t znear, const Pos_Scalar_t zfar )
			: fovy{ fovy }, aspect{ aspect }, znear{ znear }, zfar{ zfar } {}
	};

	struct OrthoAttribute
	{
		Pos_Scalar_t left, right, bottom, up, znear, zfar;

		constexpr explicit OrthoAttribute( const Pos_Scalar_t left, const Pos_Scalar_t right,
			const Pos_Scalar_t bottom, const Pos_Scalar_t up, const Pos_Scalar_t znear, const Pos_Scalar_t zfar )
			: left{ left }, right{ right }, bottom{ bottom }, up{ up }, znear{ znear }, zfar{ zfar } {}
	};

public:
	static constexpr const PerspectiveAttribute default_perspective
	{
		static_cast< Rad_Scalar_t >( glm::radians( 45.f ) ),
		static_cast< Scale_Scalar_t >( screen_width / screen_height ),
		static_cast< Pos_Scalar_t >( 0.1f ),
		static_cast< Pos_Scalar_t >( 100.f )
	};

	static constexpr const OrthoAttribute default_ortho
	{
		static_cast< Pos_Scalar_t >( -5.f ),
		static_cast< Pos_Scalar_t >( 5.f ),
		static_cast< Pos_Scalar_t >( -5.f ),
		static_cast< Pos_Scalar_t >( 5.f ),
		static_cast< Pos_Scalar_t >( -30.f ),
		static_cast< Pos_Scalar_t >( 30.f )
	};

	PerspectiveAttribute perspective_attribute;
	OrthoAttribute ortho_attribute;

	typename pool< ComponentCoord< Dimension, Elem_t > >::Sptr coord_component_eye;
	typename pool< ComponentCoord< Dimension, Elem_t > >::Sptr coord_component_at;
	typename pool< ComponentCoord< Dimension, Elem_t > >::Sptr coord_component_up;
	typename pool< ComponentPhysic< Dimension, Elem_t > >::Sptr physic_component_eye;
	typename pool< ComponentPhysic< Dimension, Elem_t > >::Sptr physic_component_at;
	typename pool< ComponentPhysic< Dimension, Elem_t > >::Sptr physic_component_up;

	void ortho() { is_ortho = true; }
	void perspective() { is_ortho = false; }

	void update( Time_t delta_time )
	{
		physic_component_eye->update( coord_component_eye->movement, coord_component_eye->rotation, delta_time );
		physic_component_at->update( coord_component_at->movement, coord_component_at->rotation, delta_time );
		physic_component_up->update( coord_component_up->movement, coord_component_up->rotation, delta_time );
	}

	void set_shader_camera_transform( GLuint view_transform_location, GLuint proj_transform_location ) const
	{
		auto view_matrix = glm::lookAt( coord_component_eye->getpivot(), coord_component_at->getpivot(), coord_component_up->getpivot() );
		glUniformMatrix4fv( view_transform_location, 1, false, glm::value_ptr( view_matrix ) );

		auto proj_matrix = get_proj_matrix();
		glUniformMatrix4fv( proj_transform_location, 1, false, glm::value_ptr( proj_matrix ) );
	}

	Camera() : perspective_attribute{ default_perspective }, ortho_attribute{ default_ortho }, is_ortho{ true } {}

	Camera( const Rad_Scalar_t fovy, const Scale_Scalar_t aspect, const Pos_Scalar_t znear, const Pos_Scalar_t zfar )
		: perspective_attribute{ fovy, aspect, znear, zfar }, ortho_attribute{ default_ortho }, is_ortho{ false } {}

	Camera( const Pos_Scalar_t left, const Pos_Scalar_t right, const Pos_Scalar_t bottom, const Pos_Scalar_t up,
		const Pos_Scalar_t znear, const Pos_Scalar_t zfar ) :
		ortho_attribute{ left, right, bottom, up, znear, zfar }, perspective_attribute{ default_perspective }, is_ortho{ true } {}

private:
	bool is_ortho;

	const auto get_proj_matrix() const
	{
		if ( is_ortho ) return glm::ortho( ortho_attribute.left, ortho_attribute.right, ortho_attribute.bottom,
			ortho_attribute.top, ortho_attribute.znear, ortho_attribute.zfar );

		else return glm::perspective( perspective_attribute.fovy, perspective_attribute.aspect,
			perspective_attribute.znear, perspective_attribute.zfar );
	}
};

#endif