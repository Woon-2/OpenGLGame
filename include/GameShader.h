#ifndef _gameshader
#define _gameshader

#include "game.h"

struct GameShader
{
    GameShader( const Shader& north_vs, const Shader& north_fs, const Shader& object_vs, const Shader& object_fs,
        const Shader& color_vs, const Shader& color_fs, const Shader& alpha_vs, const Shader& alpha_fs )
        : north_shader{ north_vs, north_fs }, object_shader{ object_vs, object_fs },
        color_shader{ color_vs, color_fs }, alpha_shader{ alpha_vs, alpha_fs }
    {
        north_pos = north_shader.push_location( 0u );
        north_texcoord = north_shader.push_location( 1u );
        north_model = north_shader.push_location( glGetUniformLocation( north_shader.id(), "model_transform" ) );
        north_view = north_shader.push_location( glGetUniformLocation( north_shader.id(), "view_transform" ) );
        north_proj = north_shader.push_location( glGetUniformLocation( north_shader.id(), "proj_transform" ) );
        north_tex = north_shader.push_location( glGetUniformLocation( north_shader.id(), "tex" ) );

        object_pos = object_shader.push_location( 0u );
        object_normal = object_shader.push_location( 1u );
        object_texcoord = object_shader.push_location( 2u );
        object_model = object_shader.push_location( glGetUniformLocation( object_shader.id(), "model_transform" ) );
        object_view = object_shader.push_location( glGetUniformLocation( object_shader.id(), "view_transform" ) );
        object_proj = object_shader.push_location( glGetUniformLocation( object_shader.id(), "proj_transform" ) );
        object_light_pos = object_shader.push_location( glGetUniformLocation( object_shader.id(), "LightPos" ) );
        object_light_color = object_shader.push_location( glGetUniformLocation( object_shader.id(), "LightColor" ) );
        object_light_ambient = object_shader.push_location( glGetUniformLocation( object_shader.id(), "Ambient" ) );
        object_light_shininess = object_shader.push_location( glGetUniformLocation( object_shader.id(), "Shininess" ) );
        object_light_viewpos = object_shader.push_location( glGetUniformLocation( object_shader.id(), "ViewPos" ) );
        object_tex = object_shader.push_location( glGetUniformLocation( object_shader.id(), "tex" ) );

        color_pos = color_shader.push_location( 0u );
        color_normal = color_shader.push_location( 1u );
        color_texcoord = color_shader.push_location( 2u );
        color_model = color_shader.push_location( glGetUniformLocation( color_shader.id(), "model_transform" ) );
        color_view = color_shader.push_location( glGetUniformLocation( color_shader.id(), "view_transform" ) );
        color_proj = color_shader.push_location( glGetUniformLocation( color_shader.id(), "proj_transform" ) );
        color_light_pos = color_shader.push_location( glGetUniformLocation( color_shader.id(), "LightPos" ) );
        color_light_color = color_shader.push_location( glGetUniformLocation( color_shader.id(), "LightColor" ) );
        color_light_ambient = color_shader.push_location( glGetUniformLocation( color_shader.id(), "Ambient" ) );
        color_light_shininess = color_shader.push_location( glGetUniformLocation( color_shader.id(), "Shininess" ) );
        color_light_viewpos = color_shader.push_location( glGetUniformLocation( color_shader.id(), "ViewPos" ) );
        color_color = color_shader.push_location( glGetUniformLocation( color_shader.id(), "ObjectColor" ) );

        alpha_pos = alpha_shader.push_location( 0u );
        alpha_normal = alpha_shader.push_location( 1u );
        alpha_texcoord = alpha_shader.push_location( 2u );
        alpha_alpha = alpha_shader.push_location( 3u );
        alpha_model = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "model_transform" ) );
        alpha_view = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "view_transform" ) );
        alpha_proj = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "proj_transform" ) );
        alpha_light_pos = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "LightPos" ) );
        alpha_light_color = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "LightColor" ) );
        alpha_light_ambient = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "Ambient" ) );
        alpha_light_shininess = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "Shininess" ) );
        alpha_light_viewpos = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "ViewPos" ) );
        alpha_tex = alpha_shader.push_location( glGetUniformLocation( alpha_shader.id(), "tex" ) );
    }

    ShaderProgram north_shader;
    ShaderProgram object_shader;
    ShaderProgram color_shader;
    ShaderProgram alpha_shader;

    GLuint north_pos;
    GLuint north_texcoord;
    GLuint north_model;
    GLuint north_view;
    GLuint north_proj;
    GLuint north_tex;

    GLuint object_pos;
    GLuint object_normal;
    GLuint object_texcoord;
    GLuint object_model;
    GLuint object_view;
    GLuint object_proj;
    GLuint object_light_pos;
    GLuint object_light_color;
    GLuint object_light_ambient;
    GLuint object_light_shininess;
    GLuint object_light_viewpos;
    GLuint object_tex;

    GLuint color_pos;
    GLuint color_normal;
    GLuint color_texcoord;
    GLuint color_model;
    GLuint color_view;
    GLuint color_proj;
    GLuint color_light_pos;
    GLuint color_light_color;
    GLuint color_light_ambient;
    GLuint color_light_shininess;
    GLuint color_light_viewpos;
    GLuint color_color;

    GLuint alpha_pos;
    GLuint alpha_normal;
    GLuint alpha_texcoord;
    GLuint alpha_alpha;
    GLuint alpha_model;
    GLuint alpha_view;
    GLuint alpha_proj;
    GLuint alpha_light_pos;
    GLuint alpha_light_color;
    GLuint alpha_light_ambient;
    GLuint alpha_light_shininess;
    GLuint alpha_light_viewpos;
    GLuint alpha_tex;
};

#endif