#ifndef _camera
#define _camera

#include "coord.h"
#include "physics.h"
#include "screen.h"
#include "CClass.h"

class Camera
{
private:
	using Elem_t = float;
	using Rad_Scalar_t = Elem_t;
	using Pos_Scalar_t = Elem_t;
	using Scale_Scalar_t = Elem_t;
	using Time_t = Elem_t;
	static constexpr int Dimension = 3;
	using vec_t = glm::vec< Dimension, Elem_t >;
	using mat_t = glm::mat< Dimension, Dimension, Elem_t >;
	using cartesian_vec_t = glm::vec< Dimension + 1, Elem_t >;
	using cartesian_mat_t = glm::mat< Dimension + 1, Dimension + 1, Elem_t >;

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
		Pos_Scalar_t left, right, bottom, top, znear, zfar;

		constexpr explicit OrthoAttribute( const Pos_Scalar_t left, const Pos_Scalar_t right,
			const Pos_Scalar_t bottom, const Pos_Scalar_t top, const Pos_Scalar_t znear, const Pos_Scalar_t zfar )
			: left{ left }, right{ right }, bottom{ bottom }, top{ top }, znear{ znear }, zfar{ zfar } {}
	};

public:
	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';

			ss << fore_indent << indent << "eye: " << '\n';
			ss << coord_component_eye->get_desc( fore_indent + indent + indent ).rdbuf();
			ss << physic_component_eye->get_desc( fore_indent + indent + indent ).rdbuf();

			ss << fore_indent << indent << "at: " << '\n';
			ss << coord_component_at->get_desc( fore_indent + indent + indent ).rdbuf();
			ss << physic_component_at->get_desc( fore_indent + indent + indent ).rdbuf();

			ss << fore_indent << indent << "up: " << '\n';
			ss << coord_component_up->get_desc( fore_indent + indent + indent ).rdbuf();
			ss << physic_component_up->get_desc( fore_indent + indent + indent ).rdbuf();
		}

		return ss;
	}

	static constexpr const PerspectiveAttribute default_perspective()
	{
		return PerspectiveAttribute{
		static_cast< Rad_Scalar_t >( glm::radians( 45.f ) ),
		static_cast< Scale_Scalar_t >( 16 / 9.f ),
		static_cast< Pos_Scalar_t >( 1.f ),
		static_cast< Pos_Scalar_t >( 400.f )
		};
	};

	static constexpr const OrthoAttribute default_ortho()
	{
		return OrthoAttribute{
		static_cast< Pos_Scalar_t >( -5.f ),
		static_cast< Pos_Scalar_t >( 5.f ),
		static_cast< Pos_Scalar_t >( -5.f ),
		static_cast< Pos_Scalar_t >( 5.f ),
		static_cast< Pos_Scalar_t >( -30.f ),
		static_cast< Pos_Scalar_t >( 30.f )
		};
	};

	PerspectiveAttribute perspective_attribute;
	OrthoAttribute ortho_attribute;

	CCoord coord_component_eye;
	CCoord coord_component_at;
	CCoord coord_component_up;
	CPhysic physic_component_eye;
	CPhysic physic_component_at;
	CPhysic physic_component_up;

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

	Camera() : perspective_attribute{ default_perspective() }, ortho_attribute{ default_ortho() }, is_ortho{ true } { init(); }

	Camera( const Rad_Scalar_t fovy, const Scale_Scalar_t aspect, const Pos_Scalar_t znear, const Pos_Scalar_t zfar )
		: perspective_attribute{ fovy, aspect, znear, zfar }, ortho_attribute{ default_ortho() }, is_ortho{ false } { init(); }

	Camera( const Pos_Scalar_t left, const Pos_Scalar_t right, const Pos_Scalar_t bottom, const Pos_Scalar_t up,
		const Pos_Scalar_t znear, const Pos_Scalar_t zfar ) :
		ortho_attribute{ left, right, bottom, up, znear, zfar }, perspective_attribute{ default_perspective() }, is_ortho{ true } { init(); }

private:
	bool is_ortho;

	const cartesian_mat_t get_proj_matrix() const
	{
		if ( is_ortho ) return glm::ortho( ortho_attribute.left, ortho_attribute.right, ortho_attribute.bottom,
			ortho_attribute.top, ortho_attribute.znear, ortho_attribute.zfar );

		else return glm::perspective( perspective_attribute.fovy, perspective_attribute.aspect,
			perspective_attribute.znear, perspective_attribute.zfar );
	}

	void init()
	{
		coord_component_eye.init();
		coord_component_at.init( vec_t{ static_cast< Elem_t >( 0 ), static_cast< Elem_t >( 0 ), static_cast< Elem_t >( -1 ) } );
		coord_component_up.init( vec_t{ static_cast< Elem_t >( 0 ), static_cast< Elem_t >( 1 ), static_cast< Elem_t >( 0 ) } );

		physic_component_eye.init();
		physic_component_at.init();
		physic_component_up.init();
	}
};

bool Camera::will_get_desc = true;

#endif