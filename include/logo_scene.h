#ifndef _logo_scene
#define _logo_scene

#include "game.h"
#include "Billboard.h"

class LogoScene : public Scene
{
public:
	void update( const Time_t frame_time ) override
	{

	}

	void render() override
	{

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
		: scene_status{ scene_status }, shader{ shader }
	{

	}

private:
	std::shared_ptr< GameShader > shader;
	SceneStatus& scene_status;
	CCamera camera;
	Region region;
};

#endif