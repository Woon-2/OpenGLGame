#ifndef _title_scene
#define _title_scene

#include "game.h"
#include "TitleSpace.h"
#include "Billboard.h"
#include "Light.h"
#include "battle_scene.h"

class TitleScene : public Scene
{
public:
	void update( const Time_t frame_time ) override
	{
		time += frame_time;
		sound::update();

		if ( time < 1000.f )
		{
			// [0, 1000] -> [0, 0.5]
			light.color += ( 0.98f / 1000 ) * frame_time;
		}

		if ( battle_timer_on )
		{
			if ( battle_time > std::numeric_limits< float >::epsilon() )
			{
				battle_time -= frame_time;
				light.color -= ( 0.45f / 1000 ) * frame_time;
				s->amplify( 0.96f );
			}
			else
			{
				scene_status.will_change = true;
				scene_status.next_scene_name = typeid( BattleScene ).name();
			}
		}

		center->rotation += glm::vec3( 0.f, glm::pi< float >() / 10 / 1000 * frame_time, 0.f );

		light.update( frame_time );
		camera->update( frame_time );
		space.update();

		title->update( frame_time, region );
		start->update( frame_time, region );
		quit->update( frame_time, region );
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
		space.render();

		light.render();
		title->render();
		light.render();
		start->render();
		light.render();
		quit->render();

		glutSwapBuffers();
	}

	void reshape( int w, int h ) override
	{

	}

	void mouse( int button, int state, int x, int y ) override
	{
		if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
		{
			if ( x > 492 / 1280.f * screen_width && x < 729 / 1280.f * screen_width
				&& y > 388 / 720.f * screen_height && y < 439 / 720.f * screen_height )
			{
				if ( !battle_timer_on )
				{
					e->play();
					battle_timer_on = true;
					battle_time = 2000.f;
				}
			}
			else if ( x > 545 / 1280.f * screen_width && x < 697 / 1280.f * screen_width
				&& y > 456 / 720.f * screen_height && y < 500 / 720.f * screen_height )
			{
				glutLeaveMainLoop();
			}
		}
	}

	void motion( int x, int y ) override
	{

	}

	void passive_motion( int x, int y ) override
	{

	}

	void keyboard( unsigned char key, int x, int y ) override
	{

	}

	void keyboardup( unsigned char key, int x, int y ) override
	{

	}

	void special_keyboard( int key, int x, int y ) override
	{

	}

	TitleScene( SceneStatus& scene_status, const std::shared_ptr< GameShader >& shader = nullptr )
		: scene_status{ scene_status }, shader{ shader }, light{ shader }, space{ shader }
	{
		glEnable( GL_DEPTH_TEST );
		//glEnable( GL_CULL_FACE );

		camera.init();
		center.init();
		camera->coord_component_eye->adopt_base( center );
		camera->coord_component_at->adopt_base( center );
		//camera->coord_component_up->adopt_base( center );

		//            pivot: ( 0 23.5 -43 )
		// radians: ( 0 3.1 0 )

		center->movement += glm::vec3{ 0.f, 28.5f, -5.f };
		camera->coord_component_eye->movement += glm::vec3( 0.f, 0.f, -48.f );
		camera->coord_component_at->movement += glm::vec3( 0.f, 0.f, -48.f );
		camera->coord_component_at->movement += glm::vec3( 0.f, -1.f, 2.6f );
		camera->coord_component_eye->rotation += glm::pi< float >();
		camera->coord_component_at->rotation += glm::pi< float >();

		light.ambient = glm::vec3{ 0.4f, 0.4f, 0.4f };
		light.color = glm::vec3{ 0.0f, 0.0f, 0.0f };
		//light.pos = glm::vec3{ 0.f, 1000.f, -300.f };
		light.shininess = 4.f;

		s = sound::make( "resources/sound/2.IceField.wav", sound::mode::loop );
		e = sound::make( "resources/sound/lazer.wav", sound::mode::normal );
		s->play();

		title.reset( new Billboard{ camera->coord_component_eye, shader, "resources/object/display.obj", "resources/texture/title.png", glm::vec3{ 0.f, -9.f, 20.f } } );
		title->coord()->rotation += glm::vec3( 0.f, glm::pi<float>(), 0.f );

		start.reset( new Billboard{ camera->coord_component_eye, shader, "resources/object/display.obj", "resources/texture/start.png", glm::vec3{ 0.f, -15.f, 20.f } } );
		start->coord()->rotation += glm::vec3( 0.f, glm::pi<float>(), 0.f );

		quit.reset( new Billboard{ camera->coord_component_eye, shader, "resources/object/display.obj", "resources/texture/exit.png", glm::vec3{ 0.f, -17.f, 20.f } } );
		quit->coord()->rotation += glm::vec3( 0.f, glm::pi<float>(), 0.f );

		camera->perspective();
	}

	~TitleScene()
	{
		glDisable( GL_DEPTH_TEST );
		s->stop();
		sound::update();
		//glDisable( GL_CULL_FACE );
	}

private:
	std::shared_ptr< GameShader > shader;
	SceneStatus& scene_status;
	CCamera camera;
	CCoord center;
	TitleSpace space;
	Region region;
	Light light;
	sound_ptr s;
	sound_ptr e;

	std::unique_ptr< Billboard > title;
	std::unique_ptr< Billboard > start;
	std::unique_ptr< Billboard > quit;

	float time = 0.f;
	float battle_time = 0.f;
	bool battle_timer_on = false;
};

#endif