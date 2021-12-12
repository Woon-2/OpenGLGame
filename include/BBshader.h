#ifndef _BBshader
#define _BBshader

#include "glincludes.h"
#include "glbase.h"
#include "fileread.h"
#include "shader.h"

constexpr int BBShader_location_pos = 0;

const char* BBShaderV =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"uniform mat4 model_transform;\n"
"uniform mat4 view_transform;\n"
"uniform mat4 proj_transform;\n"
"void main()\n"
"{\n"
"gl_Position = proj_transform * view_transform * model_transform * vec4( pos, 1.0 );\n"
"}\n";

const char* BBShaderF =
"#version 330 core\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"frag_color = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
"}\n";

class BBShader
{
public:
	static const int id()
	{
		if ( !inst )
		{
			inst.reset( new ShaderProgram{
				Shader{ BBShaderV, Shader::Type::VERTEX_SHADER },
				Shader{ BBShaderF, Shader::Type::FRAGMENT_SHADER }
				} );
		}

		return inst->id();
	}

	static const GLuint get_location_world_trans()
	{
		if ( !location_world_trans )
		{
			location_world_trans = glGetUniformLocation( id(), "model_transform" );
		}

		return location_world_trans;
	}

	static const GLuint get_location_view_trans()
	{
		if ( !location_view_trans )
		{
			location_view_trans = glGetUniformLocation( id(), "view_transform" );
		}

		return location_view_trans;
	}

	static const GLuint get_location_proj_trans()
	{
		if ( !location_proj_trans )
		{
			location_proj_trans = glGetUniformLocation( id(), "proj_transform" );
		}

		return location_proj_trans;
	}

private:
	static std::unique_ptr< ShaderProgram > inst;
	static GLuint location_world_trans;
	static GLuint location_view_trans;
	static GLuint location_proj_trans;
};

std::unique_ptr< ShaderProgram > BBShader::inst;
GLuint BBShader::location_world_trans;
GLuint BBShader::location_view_trans;
GLuint BBShader::location_proj_trans;


#endif