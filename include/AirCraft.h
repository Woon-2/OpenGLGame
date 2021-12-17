#ifndef _AirCraft
#define _AirCraft

#include "game.h"
#include "GameShader.h"
#include <deque>

class BoosterParticle : public GameObject
{
public:

	const bool is_over() const
	{
		return life < 0.f;
	}

	void update( const Time_t delta_time, Region& region ) override
	{
		physic->update( coord->movement, coord->rotation, delta_time );
		//coord->movement += physic->velocity;
		life -= delta_time;
	}

	void render() const override
	{
		glUseProgram( shader->color_shader.id() );
		glUniform3fv( shader->color_color, 1, glm::value_ptr( color ) );

		coord->set_shader_world_transform( shader->color_model );
		renderer->draw( *vertices );
	}

	BoosterParticle( const std::shared_ptr< GameShader >& shader, const glm::vec3& movement )
		: shader{ shader }, color{ random_valuef( 0.8, 1.0 ), random_valuef( 0.2, 0.3 ), random_valuef( 0.0, 0.1 ) },
		life{ 2000.f }
	{
		vertices.init( shader->color_shader.id() );

		if ( !prototype )
		{
			prototype.init( shader->color_shader.id() );
			prototype->load_obj( "resources/object/MyCube.obj", 3 );
		}

		*vertices = *prototype;

		vertices->init_pos( shader->object_pos );
		vertices->init_normal( shader->object_normal );
		vertices->init_texture( shader->object_texcoord );
		vertices->init_index();

		renderer.init();
		renderer->append( ComponentRender::DrawElementsDetail{ GL_TRIANGLES, vertices->size(), GL_UNSIGNED_INT, nullptr } );

		coord.init();
		coord->movement += movement;
		coord->rotation += glm::vec3{ random_valuef( -3.0, 3.0 ), random_valuef( -3.0, 3.0 ), random_valuef( -3.0, 3.0 ) };
		auto scale = random_valuef( 0.2, 0.3 );
		coord->scale *= glm::vec3{ scale, scale, scale };

		physic.init();
		physic->velocity += glm::vec3{ random_valuef( -1.0, 1.0 ), random_valuef( -1.0, 1.0 ), random_valuef( -1.0, -0.2 ) };
		physic->time_unit = 100.f;
	}

private:
	std::shared_ptr< GameShader > shader;

	static CVertex prototype;

	float life;

	CVertex vertices;
	CRender renderer;
	CCoord coord;
	CPhysic physic;
	glm::vec3 color;
};

CVertex BoosterParticle::prototype;

class AirCraftPart
{
public:
	void update()
	{
		//std::cout << "AirCraft!\n";
		//std::cout << vertices->get_desc().rdbuf();
		//std::cout << renderer->get_desc().rdbuf();
		//std::cout << coord->get_desc().rdbuf() << "\n\n\n";
	}

	void render() const
	{
		glUseProgram( shader->object_shader.id() );

		glUniform3fv( shader->object_light_ambient, 1, glm::value_ptr( local_ambient ) );
		glUniform1f( shader->object_light_shininess, local_shininess );

		coord->set_shader_world_transform( shader->object_model );
		renderer->draw( *vertices );
	}

	AirCraftPart( const CCoord& base_coord, const std::shared_ptr< GameShader >& shader, const char* obj_path, const glm::vec3& movement )
		: shader{ shader }
	{
		// 파일을 읽어서 이미지나 조명 정보를 알아온다.
		// 앰비언트는 개별 적용한다.
		std::ifstream in{ obj_path };
		char ch;
		while ( in >> ch )
		{
			if ( ch == '#' )
			{
				std::string str;
				std::getline( in, str );

				std::stringstream ss;
				ss << str;

				// #은 이미 읽음
				// " A 1 1 1" 중 " A " 를 날려야 함.
				ss >> ch;

				switch ( ch )
				{
				default:
					break;

				case 'A':
					if ( ss.peek() != 'T' )
					{
						ss >> local_ambient;
						local_ambient /= 1.4f;
					}
					break;

				case 'D':
					if ( ss.get() == 'T' )
					{
						ss.get();
						std::getline( ss, texture_path );
					}
					break;

				case 'E':
					ss >> local_shininess;
					break;
				}

			}
		}
		// --------------------------------------------

		vertices.init( shader->object_shader.id() );
		vertices->load_obj( obj_path, 3 );
		vertices->init_pos( shader->object_pos );
		vertices->init_normal( shader->object_normal );
		vertices->init_texture( shader->object_texcoord );
		vertices->init_index();

		renderer.init();
		if ( texture_path.size() )
		{
			texture_path = std::string{ "resources/object/" } + texture_path;
			renderer->texture.init( texture_path.c_str(), shader->object_tex );
		}
		else
		{
			renderer->texture.init( "resources/object/a4.png", shader->object_tex );
			local_ambient /= 6.4f;
		}
		renderer->append( ComponentRender::DrawElementsDetail{ GL_TRIANGLES, vertices->size(), GL_UNSIGNED_INT, nullptr } );

		coord.init();
		coord->adopt_base( base_coord );
		coord->movement += movement;
	}

private:
	CVertex vertices;
	CCoord coord;
	CRender renderer;
	std::shared_ptr< GameShader > shader;

	glm::vec3 local_ambient;
	float local_shininess = 0.f;
	std::string texture_path;
};

class AirCraft : public GameObject
{
public:
	void update( const Time_t delta_time, Region& region ) override
	{
		m_physic->update( m_coord->movement, m_coord->rotation, delta_time );

		delay -= delta_time;

		if ( delay < 0.f )
		{
			delay = 50.f;
			for ( int i = 0; i < 2; ++i )
			{
				particles.push_back( BoosterParticle{ shader, m_coord->getpivot() + glm::vec3{0.f, -0.5f, 4.0f} } );
			}
		}

		int cnt = 0;
		for ( auto& particle : particles )
		{
			particle.update( delta_time, region );
			if ( particle.is_over() ) ++cnt;
		}

		for ( int i = 0; i < cnt; ++i )
		{
			particles.pop_front();
		}



		for ( auto& object : objects )
		{
			object.update();
		}
	}

	void render() const override
	{
		for ( const auto& particle : particles )
		{
			particle.render();
		}

		for ( const auto& object : objects )
		{
			object.render();
		}
	}

	CCoord& coord() { return m_coord; }
	const CCoord& coord() const { return m_coord; }
	CPhysic& physic() { return m_physic; }
	const CPhysic& physic() const { return m_physic; }

	AirCraft( const std::shared_ptr< GameShader >& shader )
		: shader{ shader }
	{
		m_coord.init();
		m_physic.init();

		for ( int i = 0; i < sizeof( obj_paths ) / sizeof( const char* ); ++i )
		{
			objects.emplace_back( m_coord, shader, obj_paths[ i ], glm::vec3{ 0.f, 0.f, 0.f } );
		}
	}

private:
	static constexpr const char* obj_paths[] = {
"resources/object/AirCraft1.obj",
"resources/object/AirCraft2.obj",
	};

	std::shared_ptr< GameShader > shader;
	std::vector< AirCraftPart > objects;
	CCoord m_coord;
	CPhysic m_physic;
	float delay = 0.f;

	std::deque< BoosterParticle > particles;
};

#endif