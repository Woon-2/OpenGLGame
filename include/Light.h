#ifndef _Light
#define _Light

#include "game.h"
#include "GameShader.h"

class Light
{
public:
	// Light Data -----------------------
	glm::vec3 ambient;
	glm::vec3 pos;
	glm::vec3 color;
	float shininess;
	// ----------------------------------

	void update( const float frame_time )
	{
		m_physic->update( m_coord->movement, m_coord->rotation, frame_time );
	}

	void render()
	{
		glUseProgram( shader->alpha_shader.id() );
		glUniform3fv( shader->alpha_light_ambient, 1, glm::value_ptr( ambient ) );
		glUniform3fv( shader->alpha_light_pos, 1, glm::value_ptr( pos ) );
		glUniform1f( shader->alpha_light_shininess, shininess );
		glUniform3fv( shader->alpha_light_color, 1, glm::value_ptr( color ) );

		glUseProgram( shader->color_shader.id() );
		glUniform3fv( shader->color_light_ambient, 1, glm::value_ptr( ambient ) );
		glUniform3fv( shader->color_light_pos, 1, glm::value_ptr( pos ) );
		glUniform1f( shader->color_light_shininess, shininess );
		glUniform3fv( shader->color_light_color, 1, glm::value_ptr( color ) );

		glUseProgram( shader->object_shader.id() );
		glUniform3fv( shader->object_light_ambient, 1, glm::value_ptr( ambient ) );
		glUniform3fv( shader->object_light_pos, 1, glm::value_ptr( pos ) );
		glUniform1f( shader->object_light_shininess, shininess );
		glUniform3fv( shader->object_light_color, 1, glm::value_ptr( color ) );
	}

	CCoord& coord() { return m_coord; }
	const CCoord& coord() const { return m_coord; }
	CPhysic& physic() { return m_physic; }
	const CPhysic& physic() const { return m_physic; }

	Light( const std::shared_ptr< GameShader >& shader ) : shader{ shader }
	{
		m_physic.init();
		m_coord.init();
	}

private:
	CPhysic m_physic;
	CCoord m_coord;
	std::shared_ptr< GameShader > shader;
};

#endif