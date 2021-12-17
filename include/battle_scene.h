#ifndef _battle_scene
#define _battle_scene

#include "game.h"
#include "Billboard.h"
#include "Light.h"
#include "AirCraft.h"
#include "object.h"

class BattleScene : public Scene
{
public:
	void update( const Time_t frame_time ) override
	{
		time += frame_time;
		sound::update();

		if ( time < 1000.f )
		{
			// [0, 2000] -> [0, 0.5]
			light.color += ( 0.95f / 1000 ) * frame_time;
		}

		light.update( frame_time );
		camera->update( frame_time );
		player.update( frame_time, region );

		for ( auto& object : objects )
		{
			object->update( frame_time, region );
		}
	}

	void render() override
	{
		glClearColor( 0.f, 0.f, 0.f, 1.f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		const auto& campos = camera->coord_component_eye->getpivot();

		glUseProgram( shader->alpha_shader.id() );
		camera->set_shader_camera_transform( shader->alpha_view, shader->alpha_proj );
		glUniform3fv( shader->alpha_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->color_shader.id() );
		camera->set_shader_camera_transform( shader->color_view, shader->color_proj );
		glUniform3fv( shader->color_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->object_shader.id() );
		camera->set_shader_camera_transform( shader->object_view, shader->object_proj );
		glUniform3fv( shader->object_light_viewpos, 1, glm::value_ptr( campos ) );

		glUseProgram( shader->north_shader.id() );
		camera->set_shader_camera_transform( shader->north_view, shader->north_proj );

		light.pos = camera->coord_component_eye->getpivot();

		light.render();
		player.render();

		for ( const auto& object : objects )
		{
			light.render();
			object->render();
		}

		glutSwapBuffers();
	}

	void reshape( int w, int h ) override
	{

	}

	void mouse( int button, int state, int x, int y ) override
	{
		if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
		{
			drag = true;
			old_x = x;
			old_y = y;
		}
		else if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) drag = false;
	}

	void motion( int x, int y ) override
	{
		if ( drag )
		{
			if ( old_x > 0 )
			{
				auto dx = x - old_x;
				auto dy = y - old_y;

				center->rotation += glm::vec3( dy * glm::pi<float>() / 4 / screen_height, dx * glm::pi<float>() / 4 / screen_width, 0.f );
			}

			old_x = x;
			old_y = y;
		}
	}

	void passive_motion( int x, int y ) override
	{

	}

	void keyboard( unsigned char key, int x, int y ) override
	{
		switch ( key )
		{
		default:
			break;

		case 'w':
			player.physic()->accel -= glm::vec3( 0.f, 0.f, 0.2f );
			break;

		case 's':
			player.physic()->accel += glm::vec3( 0.f, 0.f, 0.2f );
			break;

		case 'p':
			std::cout << player.coord()->get_desc().rdbuf() << '\n';
			std::cout << player.physic()->get_desc().rdbuf() << '\n';
			break;
		}
	}

	void keyboardup( unsigned char key, int x, int y ) override
	{
	}

	void special_keyboard( int key, int x, int y ) override
	{

	}

	BattleScene( SceneStatus& scene_status, const std::shared_ptr< GameShader >& shader = nullptr )
		: scene_status{ scene_status }, shader{ shader }, light{ shader }, player{ shader }
	{
		glEnable( GL_DEPTH_TEST );
		//glEnable( GL_CULL_FACE );

		s = sound::make( "resources/sound/2.IceBoss.wav", sound::mode::loop );
		s->play();

		camera.init();
		camera->coord_component_eye->adopt_base( player.coord() );
		camera->coord_component_at->adopt_base( player.coord() );

		center.init();
		player.coord()->adopt_base( center );
		player.physic()->time_unit = 1000.f;

		camera->coord_component_eye->movement += glm::vec3{ 0.f, 2.5f, 8.f };
		camera->coord_component_at->movement += glm::vec3{ 0.f, 2.f, 4.f };

		light.pos = glm::vec3{ 0.f, 0.f, 0.f };
		light.ambient = glm::vec3{ 1.f, 1.f, 1.f };
		light.color = glm::vec3{ 0.0f, 0.0f, 0.0f };
		light.shininess = 0.f;

		for ( int i = 0; i < 2500; ++i )
		{
			objects.emplace_back( new Object{ shader } );
		}
		camera->perspective();
	}

	~BattleScene()
	{
		glDisable( GL_DEPTH_TEST );
		//glDisable( GL_CULL_FACE );
	}

private:
	std::shared_ptr< GameShader > shader;
	SceneStatus& scene_status;
	AirCraft player;
	CCoord center;
	CCamera camera;
	Region region;
	Light light;

	sound_ptr s;

	bool drag = false;
	int old_x = 0;
	int old_y = 0;

	std::vector< std::unique_ptr< GameObject > > objects;

	float time = 0.f;
};

#endif