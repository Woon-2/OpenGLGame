#ifndef _gamenodeguide
#define _gamenodeguide

#include "game.h"
#include "fileread.h"

class GameNodeGuide : public Game< GameNodeGuide >
{
public:
    GameNodeGuide( int argc, char** argv )
        : Game< GameNodeGuide >{ argc, argv, 100, 100, 1600, 900, "GameNodeGuide" }
    {
        init_shader();
    }

private:
    friend class Game< GameNodeGuide >;
    static std::unique_ptr< ShaderProgram > shader_program;

    static void init_shader()
    {
        Shader vertex_shader{ read_file( "shader/vertex_practice.glsl" ), Shader::Type::VERTEX_SHADER };
        Shader fragment_shader{ read_file( "shader/fragment_practice.glsl" ), Shader::Type::FRAGMENT_SHADER };
        shader_program.reset( new ShaderProgram{ vertex_shader, fragment_shader } );
    }

    static void draw_scene()
    {
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        //glClearDepth( 0.f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        //glViewport( 100, 100, 700, 500 );

        glutSwapBuffers();
    }

    static void reshape( int w, int h )
    {
        glViewport( 0, 0, w, h );
    }


    static void mouse( int button, int state, int wx, int wy )
    {
        auto glpt = glPtconvert( wx, wy, 800, 600 );
        auto x = glpt.first;
        auto y = glpt.second;
    }

    static void motion( int wx, int wy )
    {
        auto glpt = glPtconvert( wx, wy, 800, 600 );
        auto x = glpt.first;
        auto y = glpt.second;
    }

    static void keyboard( unsigned char key, int wx, int wy )
    {
        switch ( key )
        {
        default:
            break;
        }
    }

    static void special_keyboard( int key, int x, int y )
    {
        switch ( key )
        {
        default:
            break;
        }
    }

};

std::unique_ptr< ShaderProgram > GameNodeGuide::shader_program;

#endif