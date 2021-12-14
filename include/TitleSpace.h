#ifndef _TitleSpace
#define _TitleSpace

#include "game.h"
#include "GameShader.h"

class TitleSpaceObject
{
public:
	void update()
	{
		//std::cout << "Billboard!\n";
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

	TitleSpaceObject( const CCoord& base_coord, const std::shared_ptr< GameShader >& shader, const char* obj_path, const glm::vec3& movement )
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

class TitleSpace
{
public:
	void update()
	{
		for ( auto& object : objects )
		{
			object.update();
		}
	}

	void render() const
	{
		for ( const auto& object : objects )
		{
			object.render();
		}
	}

	CCoord& coord() { return m_coord; }
	const CCoord& coord() const { return m_coord; }
	CPhysic& physic() { return m_physic; }
	const CPhysic& physic() const { return m_physic; }
	
	TitleSpace( const std::shared_ptr< GameShader >& shader )
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
"resources/object/Circle.020_Circle.003.obj",
"resources/object/door.001_Circle.026.obj",
"resources/object/door.002_Circle.017.obj",
"resources/object/door.003_Circle.008.obj",
"resources/object/door.004_Circle.026.obj",
"resources/object/door.005_Circle.008.obj",
"resources/object/door.006_Circle.017.obj",
"resources/object/door.007_Circle.016.obj",
"resources/object/door.008_Circle.026.obj",
"resources/object/door.009_Circle.026.obj",
"resources/object/door_Circle.016.obj",
"resources/object/Drohn_1.001_Torus.003.obj",
"resources/object/Drohn_1.002_Cube.obj",
"resources/object/Drohn_1.003_Torus.014.obj",
"resources/object/Drohn_1.004_Torus.014.obj",
"resources/object/Drohn_1.005_Cube.obj",
"resources/object/Drohn_1.006_Torus.003.obj",
"resources/object/Drohn_1.007_Cube.obj",
"resources/object/Drohn_1.008_Torus.014.obj",
"resources/object/Drohn_1.009_Torus.003.obj",
"resources/object/Drohn_1.010_Torus.014.obj",
"resources/object/Drohn_1.011_Cube.obj",
"resources/object/Drohn_1.012_Torus.014.obj",
"resources/object/Drohn_1.013_Torus.003.obj",
"resources/object/Drohn_1.014_Cube.obj",
"resources/object/Drohn_1.015_Torus.003.obj",
"resources/object/Drohn_1.016_Torus.014.obj",
"resources/object/Drohn_1.017_Cube.obj",
"resources/object/Drohn_1.018_Torus.003.obj",
"resources/object/Drohn_1.019_Torus.003.obj",
"resources/object/Drohn_1.020_Torus.014.obj",
"resources/object/Drohn_1.021_Cube.obj",
"resources/object/Drohn_1.022_Torus.014.obj",
"resources/object/Drohn_1.023_Cube.obj",
"resources/object/Drohn_1_Torus.003.obj",
"resources/object/hologram_ground_Circle.024.obj",
"resources/object/hologram_radar.001_Cube.001.obj",
"resources/object/hologram_radar.002_Torus.017.obj",
"resources/object/hologram_radar.002_Torus.017_2.obj",
"resources/object/hologram_radar.003_Torus.017.obj",
"resources/object/hologram_radar.003_Torus.017_2.obj",
"resources/object/hologram_radar.004_Torus.017.obj",
"resources/object/hologram_radar.004_Torus.017_2.obj",
"resources/object/hologram_radar.005_Torus.017.obj",
"resources/object/hologram_radar.005_Torus.017_2.obj",
"resources/object/hologram_radar.006_Torus.017.obj",
"resources/object/hologram_radar.006_Torus.017_2.obj",
"resources/object/hologram_radar.007_Torus.017.obj",
"resources/object/hologram_radar.007_Torus.017_2.obj",
"resources/object/hologram_radar_Torus.017.obj",
"resources/object/hologram_radar_Torus.017_2.obj",
"resources/object/hologram_Sphere.001.obj",
"resources/object/Laser-Tower-Gun.001_tower.002.obj",
"resources/object/Laser-Tower-Gun.002_tower.002.obj",
"resources/object/Laser-Tower-Gun.003_tower.002.obj",
"resources/object/Laser-Tower-Gun.004_tower.003.obj",
"resources/object/Laser-Tower-Gun.005_tower.003.obj",
"resources/object/Laser-Tower-Gun.006_tower.003.obj",
"resources/object/Laser-Tower-Gun.007_tower.003.obj",
"resources/object/Laser-Tower-Gun.008_tower.004.obj",
"resources/object/Laser-Tower-Gun.009_tower.004.obj",
"resources/object/Laser-Tower-Gun.010_tower.004.obj",
"resources/object/Laser-Tower-Gun.011_tower.004.obj",
"resources/object/Laser-Tower-Gun.012_tower.004.obj",
"resources/object/Laser-Tower-Gun.013_tower.004.obj",
"resources/object/Laser-Tower-Gun.014_tower.004.obj",
"resources/object/Laser-Tower-Gun.015_tower.004.obj",
"resources/object/Laser-Tower-Gun.016_tower.004.obj",
"resources/object/Laser-Tower-Gun.017_tower.004.obj",
"resources/object/Laser-Tower-Gun.018_tower.004.obj",
"resources/object/Laser-Tower-Gun.019_tower.004.obj",
"resources/object/Laser-Tower-Gun.020_tower.004.obj",
"resources/object/Laser-Tower-Gun.021_tower.004.obj",
"resources/object/Laser-Tower-Gun.022_tower.004.obj",
"resources/object/Laser-Tower-Gun.023_tower.004.obj",
"resources/object/Laser-Tower-Gun_tower.002.obj",
"resources/object/Laser-Tower.001_tower.001.obj",
"resources/object/Laser-Tower.002_tower.001.obj",
"resources/object/Laser-Tower.003_tower.001.obj",
"resources/object/Laser-Tower_tower.001.obj",
"resources/object/Panic_Bot.001_Panic_Bot.002.obj",
"resources/object/Railing_Circle.033.obj",
"resources/object/Red_Laser.001_Cube.004.obj",
"resources/object/Red_Laser.002_Cube.004.obj",
"resources/object/Red_Laser.003_Cube.004.obj",
"resources/object/Red_Laser.004_Cube.004.obj",
"resources/object/Red_Laser.005_Cube.004.obj",
"resources/object/Red_Laser.006_Cube.004.obj",
"resources/object/Red_Laser.007_Cube.004.obj",
"resources/object/Red_Laser.008_Cube.004.obj",
"resources/object/Red_Laser.009_Cube.004.obj",
"resources/object/Red_Laser.010_Cube.004.obj",
"resources/object/Red_Laser.011_Cube.004.obj",
"resources/object/Red_Laser.012_Cube.004.obj",
"resources/object/Red_Laser.013_Cube.004.obj",
"resources/object/Red_Laser.014_Cube.004.obj",
"resources/object/Red_Laser.015_Cube.004.obj",
"resources/object/Red_Laser_Cube.004.obj",
"resources/object/Sartfighter.001_SF.obj",
"resources/object/Sartfighter.002_SF.obj",
"resources/object/Sartfighter_SF.obj",
"resources/object/scifi_column.001_Circle.037.obj",
"resources/object/scifi_column_Circle.037.obj",
"resources/object/Starfighter_Laser.001_SF.002.obj",
"resources/object/Starfighter_Laser.002_SF.002.obj",
"resources/object/Star_Fighter_Fire.001_SF.001.obj",
"resources/object/Star_Fighter_Fire.002_SF.001.obj",
"resources/object/Star_Fighter_Fire.003_SF.001.obj",
"resources/object/Star_Fighter_Fire.004_SF.001.obj",
"resources/object/Star_Fighter_Fire.005_SF.001.obj",
"resources/object/Star_Fighter_Fire_SF.001.obj",
"resources/object/Station-Inside_Circle.065.obj",
"resources/object/Station-Outside_out_side_low.obj",
"resources/object/Station_Mesh.001.obj",
"resources/object/Text.003_Mesh.003.obj",
"resources/object/Under_Attack_Mesh.obj",
"resources/object/Warning_Mesh.002.obj",
"resources/object/Windows_Circle.002.obj",
"resources/object/window_shutter_down_Circle.025.obj",
"resources/object/window_shutter_up_Circle.000.obj"
	};

	std::vector< TitleSpaceObject > objects;
	CCoord m_coord;
	CPhysic m_physic;
};

#endif