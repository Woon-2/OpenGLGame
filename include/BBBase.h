#ifndef _BBBase
#define _BBBase

#include "glincludes.h"
#include "glbase.h"
#include "coord.h"
#include "BBshader.h"
#include "camera.h"

class BBBase
{
public:
	void init( const CCoord& base, const glm::vec3& initial_half_len, const glm::vec3& initial_movement = glm::vec3{ 0.f, 0.f, 0.f } )
	{
		half_len = initial_half_len;
		movement = initial_movement;

		auto pos = base->get_initial_pos();

		auto left = pos.x - half_len.x;
		auto right = pos.x + half_len.x;
		auto top = pos.y + half_len.y;
		auto bottom = pos.y - half_len.y;
		auto zfar = pos.z - half_len.z;
		auto znear = pos.z + half_len.z;

		vertices.init( BBShader::id() );
		renderer.init();
		coord.init();

		coord->adopt_base( base );
		coord->movement += movement;

		vertices->append_pos(
			glm::vec3{ right, top, zfar },
			glm::vec3{ right, bottom, zfar },
			glm::vec3{ right, top, znear },
			glm::vec3{ right, bottom, znear },
			glm::vec3{ left, top, zfar },
			glm::vec3{ left, bottom, zfar },
			glm::vec3{ left, top, znear },
			glm::vec3{ left, bottom, znear }
		);

		vertices->append_index(
			4, 2, 0,
			2, 7, 3,
			6, 5, 7,
			1, 7, 5,
			0, 3, 1,
			4, 1, 5,
			4, 6, 2,
			2, 6, 7,
			6, 4, 5,
			1, 3, 7,
			0, 2, 3,
			4, 0, 1
		);

		renderer->append( ComponentRender::DrawElementsDetail{ GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr } );
	}

	void render( const Camera& cam ) const
	{
		coord->set_shader_world_transform( BBShader::get_location_world_trans() );
		cam.set_shader_camera_transform( BBShader::get_location_view_trans(), BBShader::get_location_proj_trans() );
		renderer->draw( *vertices );
	}

	friend class ComponentCollide;

protected:
	CVertex vertices;
	CRender renderer;
	CCoord coord;

	const glm::vec3 get_half_len() const
	{
		return half_len * coord->getscales();
	}

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		ss << fore_indent << typeid( *this ).name() << '\n';

		ss << fore_indent << vertices->get_desc( fore_indent + indent ).rdbuf();
		ss << fore_indent << renderer->get_desc( fore_indent + indent ).rdbuf();
		ss << fore_indent << coord->get_desc( fore_indent + indent ).rdbuf();

		return ss;
	}

private:
	glm::vec3 half_len;
	glm::vec3 movement;
};

#endif