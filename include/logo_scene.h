#ifndef _logo_scene
#define _logo_scene

#include "game.h"
#include "Billboard.h"
#include "Light.h"
#include "title_scene.h"

class LogoScene : public Scene
{
public:
	void update( const Time_t frame_time ) override
	{
		time += frame_time;
		sound::update();

		if ( time < 1000.f )
		{
			// [0, 2000] -> [0, 0.5]
			light.color += ( 0.45f / 1000 ) * frame_time;
		}
		else if ( time < 3000.f ) {}
		else if ( time < 4000.f )
		{
			light.color -= ( 0.45f / 1000 ) * frame_time;
		}
		else
		{
			scene_status.will_change = true;
			scene_status.next_scene_name = typeid( TitleScene ).name();
		}

		light.update( frame_time );
		camera->update( frame_time );
		logo->update( frame_time, region );
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

		light.render();
		logo->render();

		glutSwapBuffers();
	}

	void reshape( int w, int h ) override
	{

	}

	void mouse( int button, int state, int x, int y ) override
	{

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

	LogoScene( SceneStatus& scene_status, const std::shared_ptr< GameShader >& shader = nullptr )
		: scene_status{ scene_status }, shader{ shader }, light{ shader }, logo{ nullptr }
	{
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );

		camera.init();
		logo.reset( new Billboard{ camera->coord_component_eye, shader, "resources/object/display.obj", "resources/texture/kpulogo.png", glm::vec3{ 0.f, 0.f, -20.f } } );

		light.pos = glm::vec3{ 0.f, 0.f, 0.f };
		light.ambient = glm::vec3{ 0.4f, 0.4f, 0.4f };
		light.color = glm::vec3{ 0.0f, 0.0f, 0.0f };
		light.shininess = 4.f;

		s = sound::make( "resources/sound/logosound.wav", sound::mode::normal );
		s->play();

		camera->perspective();
	}

	~LogoScene()
	{
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
	}

private:
	std::shared_ptr< GameShader > shader;
	SceneStatus& scene_status;
	CCamera camera;
	std::unique_ptr< Billboard > logo;
	Region region;
	Light light;
	sound_ptr s;

	float time;
};

#endif