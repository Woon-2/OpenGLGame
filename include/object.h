#ifndef _Object
#define _Object

#include "game.h"
#include "GameShader.h"

class Object : public GameObject
{
public:
	void update( const Time_t delta_time, Region& region ) override
	{

	}

	void render() const override
	{
		glUseProgram( shader->color_shader.id() );
		glUniform3fv( shader->color_color, 1, glm::value_ptr( color ) );

		coord->set_shader_world_transform( shader->color_model );
		renderer->draw( *vertices );
	}

	Object( const std::shared_ptr< GameShader >& shader )
		: shader{ shader }, color{ random_valuef( 0.0, 1.0 ), random_valuef( 0.0, 1.0 ), random_valuef( 0.0, 1.0 ) }
	{
		vertices.init( shader->color_shader.id() );

		if ( !prototype )
		{
			prototype.init( shader->color_shader.id() );
			prototype->load_obj( "resources/object/sphere.obj", 3 );
		}

		*vertices = *prototype;

		vertices->init_pos( shader->object_pos );
		vertices->init_normal( shader->object_normal );
		vertices->init_texture( shader->object_texcoord );
		vertices->init_index();

		renderer.init();
		renderer->append( ComponentRender::DrawElementsDetail{ GL_TRIANGLES, vertices->size(), GL_UNSIGNED_INT, nullptr } );

		coord.init();
		coord->movement += glm::vec3{ random_valuef( -200.0, 200.0 ), random_valuef( -200.0, 200.0 ), random_valuef( -200.0, 200.0 ) };
		coord->rotation += glm::vec3{ random_valuef( -3.0, 3.0 ), random_valuef( -3.0, 3.0 ), random_valuef( -3.0, 3.0 ) };
		auto scale = random_valuef( 0.5, 2.0 );
		coord->scale *= glm::vec3{ scale, scale, scale };
	}

private:
	std::shared_ptr< GameShader > shader;

	static CVertex prototype;

	CVertex vertices;
	CRender renderer;
	CCoord coord;
	glm::vec3 color;
};

CVertex Object::prototype;

#endif