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
        
        north_pos = north_shader.get_location( north_pos );
        north_texcoord = north_shader.get_location( north_texcoord );
        north_model = north_shader.get_location( north_model );
        north_view = north_shader.get_location( north_view );
        north_proj = north_shader.get_location( north_proj );
        north_tex = north_shader.get_location( north_tex );

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

        object_pos = object_shader.get_location( object_pos );
        object_normal = object_shader.get_location( object_normal );
        object_texcoord = object_shader.get_location( object_texcoord );
        object_model = object_shader.get_location( object_model );
        object_view = object_shader.get_location( object_view );
        object_proj = object_shader.get_location( object_proj );
        object_light_pos = object_shader.get_location( object_light_pos );
        object_light_color = object_shader.get_location( object_light_color );
        object_light_ambient = object_shader.get_location( object_light_ambient );
        object_light_shininess = object_shader.get_location( object_light_shininess );
        object_light_viewpos = object_shader.get_location( object_light_viewpos );
        object_tex = object_shader.get_location( object_tex );

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

        color_pos = color_shader.get_location( color_pos );
        color_normal = color_shader.get_location( color_normal );
        color_texcoord = color_shader.get_location( color_texcoord );
        color_model = color_shader.get_location( color_model );
        color_view = color_shader.get_location( color_view );
        color_proj = color_shader.get_location( color_proj );
        color_light_pos = color_shader.get_location( color_light_pos );
        color_light_color = color_shader.get_location( color_light_color );
        color_light_ambient = color_shader.get_location( color_light_ambient );
        color_light_shininess = color_shader.get_location( color_light_shininess );
        color_light_viewpos = color_shader.get_location( color_light_viewpos );
        color_color = color_shader.get_location( color_color );

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

        alpha_pos = alpha_shader.get_location( alpha_pos );
        alpha_normal = alpha_shader.get_location( alpha_normal );
        alpha_texcoord = alpha_shader.get_location( alpha_texcoord );
        alpha_alpha = alpha_shader.get_location( alpha_alpha );
        alpha_model = alpha_shader.get_location( alpha_model );
        alpha_view = alpha_shader.get_location( alpha_view );
        alpha_proj = alpha_shader.get_location( alpha_proj );
        alpha_light_pos = alpha_shader.get_location( alpha_light_pos );
        alpha_light_color = alpha_shader.get_location( alpha_light_color );
        alpha_light_ambient = alpha_shader.get_location( alpha_light_ambient );
        alpha_light_shininess = alpha_shader.get_location( alpha_light_shininess );
        alpha_light_viewpos = alpha_shader.get_location( alpha_light_viewpos );
        alpha_tex = alpha_shader.get_location( alpha_tex );
    }

    ShaderProgram north_shader;
    ShaderProgram object_shader;
    ShaderProgram color_shader;
    ShaderProgram alpha_shader;

    GLint north_pos;
    GLint north_texcoord;
    GLint north_model;
    GLint north_view;
    GLint north_proj;
    GLint north_tex;

    GLint object_pos;
    GLint object_normal;
    GLint object_texcoord;
    GLint object_model;
    GLint object_view;
    GLint object_proj;
    GLint object_light_pos;
    GLint object_light_color;
    GLint object_light_ambient;
    GLint object_light_shininess;
    GLint object_light_viewpos;
    GLint object_tex;

    GLint color_pos;
    GLint color_normal;
    GLint color_texcoord;
    GLint color_model;
    GLint color_view;
    GLint color_proj;
    GLint color_light_pos;
    GLint color_light_color;
    GLint color_light_ambient;
    GLint color_light_shininess;
    GLint color_light_viewpos;
    GLint color_color;

    GLint alpha_pos;
    GLint alpha_normal;
    GLint alpha_texcoord;
    GLint alpha_alpha;
    GLint alpha_model;
    GLint alpha_view;
    GLint alpha_proj;
    GLint alpha_light_pos;
    GLint alpha_light_color;
    GLint alpha_light_ambient;
    GLint alpha_light_shininess;
    GLint alpha_light_viewpos;
    GLint alpha_tex;
};

#endif