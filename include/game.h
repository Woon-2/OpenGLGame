#ifndef _game
#define _game

#include "MWEngineLow.h"
#include "timer.h"
#include "scene.h"
#include "behavior_tree.h"
#include "sound.h"
#include "gameobject.h"
#include <string>

template < typename GameNode >
class Game
{
public:
    Game( int argc, char** argv, int sx, int sy, int sw, int sh, const std::string& game_name )
        : game_name { game_name } { init( argc, argv, sx, sy, sw, sh ); }
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;

    void run() { glutMainLoop(); }

private:
    std::string game_name;

	void init( int argc, char** argv, int sx, int sy, int sw, int sh )
	{
        glutInit( &argc, argv );
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
        glutInitWindowPosition( sx, sy );
        glutInitWindowSize( sw, sh );
        glutCreateWindow( game_name.c_str() );

        screen_width = static_cast< decltype( screen_width ) >( sw );
        screen_height = static_cast< decltype( screen_height ) >( sh );

        glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK )
        {
            std::cerr << "unable to initialize GLEW" << std::endl;
            exit( EXIT_FAILURE );
        }
        else
        {
            std::cout << "GLEW initialized" << std::endl;
        }

        glutDisplayFunc( GameNode::render );
        glutReshapeFunc( GameNode::reshape );
        glutMouseFunc( GameNode::mouse );
        glutMotionFunc( GameNode::motion );
        glutPassiveMotionFunc( GameNode::passive_motion );
        glutKeyboardFunc( GameNode::keyboard );
        glutSpecialFunc( GameNode::special_keyboard );
	}
};

#endif