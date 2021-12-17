#ifndef _GameNode
#define _GameNode

#include "game.h"
#include "logo_scene.h"
#include "title_scene.h"
#include "battle_scene.h"
#include "GameShader.h"

class GameNode : public Game< GameNode >
{
public:
    GameNode( int argc, char** argv )
        : Game< GameNode >{ argc, argv, 100, 100, 1280, 720, "Space Cleaner" },
        scene{}, timer{ on_timer },
        shader{ new GameShader{
        Shader{ read_file( "shader/NorthVS.glsl" ), Shader::Type::VERTEX_SHADER },
        Shader{ read_file( "shader/NorthFS.glsl" ), Shader::Type::FRAGMENT_SHADER },
        Shader{ read_file( "shader/ObjectVS.glsl" ), Shader::Type::VERTEX_SHADER },
        Shader{ read_file( "shader/ObjectFS.glsl" ), Shader::Type::FRAGMENT_SHADER },
        Shader{ read_file( "shader/ColorVS.glsl" ), Shader::Type::VERTEX_SHADER },
        Shader{ read_file( "shader/ColorFS.glsl" ), Shader::Type::FRAGMENT_SHADER },
        Shader{ read_file( "shader/AlphaVS.glsl" ), Shader::Type::VERTEX_SHADER },
        Shader{ read_file( "shader/AlphaFS.glsl" ), Shader::Type::FRAGMENT_SHADER },
            } }
    {
        self = this;
        timer.run();
        scene.reset( new LogoScene{ scene_status, shader } );
    }

private:
    friend class Game< GameNode >;
    static GameNode* self;
    std::unique_ptr< Scene > scene;
    SceneStatus scene_status;
    std::shared_ptr< GameShader > shader;
    Timer timer;

    static void reshape( int w, int h ) { self->scene->reshape( w, h ); }
    static void mouse( int button, int state, int wx, int wy ) { self->scene->mouse( button, state, wx, wy ); }
    static void motion( int wx, int wy ) { self->scene->motion( wx, wy ); }
    static void passive_motion( int wx, int wy ) { self->scene->passive_motion( wx, wy ); }
    static void keyboard( unsigned char key, int wx, int wy ) { self->scene->keyboard( key, wx, wy ); }
    static void keyboardup( unsigned char key, int wx, int wy ) { self->scene->keyboardup( key, wx, wy ); }
    static void special_keyboard( int key, int x, int y ) { self->scene->special_keyboard( key, x, y ); }
    static void update()
    {
        self->scene->update( self->timer.get_frame_time() );
    }
    static void render()
    {
        self->scene->render();

        if ( self->scene_status.will_change )
        {
            self->timer.stop();
            self->scene_status.will_change = false;
               self->scene.release();

            if ( self->scene_status.next_scene_name == std::string{ typeid( LogoScene ).name() } )
                self->scene.reset( new LogoScene{ self->scene_status, self->shader } );
            else if ( self->scene_status.next_scene_name == std::string{ typeid( TitleScene ).name() } )
                self->scene.reset( new TitleScene{ self->scene_status, self->shader } );
            else if ( self->scene_status.next_scene_name == std::string{ typeid( BattleScene ).name() } )
                self->scene.reset( new BattleScene{ self->scene_status, self->shader } );

            self->timer.run();
        }
    }

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