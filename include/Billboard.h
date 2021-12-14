#ifndef _billboard
#define _billboard

#include "game.h"
#include "GameShader.h"

class Billboard : public GameObject
{
public:
	void update( const Time_t frame_time, Region& region ) override
	{

	}

	void render() const override
	{
		glUseProgram( shader->object_shader.id() );

		m_coord->set_shader_world_transform( shader->object_model );
		renderer->draw( *vertices );
	}

	CCoord& coord() { return m_coord; }
	const CCoord& coord() const { return m_coord; }

	Billboard( const CCoord& cam_coord, const std::shared_ptr< GameShader >& shader, const char* obj_path,
		const char* image_path, const glm::vec3& movement )
		: shader{ shader }
	{
		vertices.init( shader->object_shader.id() );
		vertices->load_obj( obj_path, 3 );
		vertices->init_pos( shader->object_pos );
		vertices->init_normal( shader->object_normal );
		vertices->init_texture( shader->object_texcoord );
		vertices->init_index();

		renderer.init();
		renderer->texture.init( image_path, shader->object_tex );
		renderer->append( ComponentRender::DrawElementsDetail{ GL_TRIANGLES, vertices->size(), GL_UNSIGNED_INT, nullptr } );

		m_coord.init();
		m_coord->adopt_base( cam_coord );
		m_coord->movement += movement;
	}

private:
	CVertex vertices;
	CCoord m_coord;
	CRender renderer;
	std::shared_ptr< GameShader > shader;
};

#endif