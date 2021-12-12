#ifndef _GameNode
#define _GameNode

#include "game.h"

class GameNode : public Game< GameNode >
{
public:
    GameNode( int argc, char** argv )
        : Game< GameNode >{ argc, argv, 100, 100, 1280, 720, "Destroyer" },
        scene{ /* have to add specified scene */ }, timer{ on_timer },
        shader{ new ShaderProgram{
        Shader{ read_file( "shader/vertex_light.glsl" ), Shader::Type::VERTEX_SHADER },
        Shader{ read_file( "shader/fragment_light.glsl" ), Shader::Type::FRAGMENT_SHADER }
            } }
    {
        self = this;
        timer.run();
    }

private:
    friend class Game< GameNode >;
    static GameNode* self;
    std::unique_ptr< Scene > scene;
    std::shared_ptr< ShaderProgram > shader;
    Timer timer;

    static void reshape( int w, int h ) { self->scene->reshape( w, h ); }
    static void mouse( int button, int state, int wx, int wy ) { self->scene->mouse( button, state, wx, wy ); }
    static void motion( int wx, int wy ) { self->scene->motion( wx, wy ); }
    static void passive_motion( int wx, int wy ) { self->scene->passive_motion( wx, wy ); }
    static void keyboard( unsigned char key, int wx, int wy ) { self->scene->keyboard( key, wx, wy ); }
    static void keyboardup( unsigned char key, int wx, int wy ) { self->scene->keyboardup( key, wx, wy ); }
    static void special_keyboard( int key, int x, int y ) { self->scene->special_keyboard( key, x, y ); }
    static void update() { self->scene->update( self->timer.get_frame_time() ); }
    static void render() { self->scene->render(); }

    static void on_timer( int )
    {
        self->timer.update();

        if ( self->timer.getlag() < 2 * self->timer.get_frame_time_setted() )
        {
            update();

            if ( self->timer.getlag() < self->timer.get_frame_time_setted() )
            {
                render();
            }
        }
    }
};

GameNode* GameNode::self;

#endif