#ifndef _shader
#define _shader

#include <string>
#include "glincludes.h"
#include <iostream>
#include "TMP.h"
#include <vector>
#include "fileread.h"

class Shader
{
public:
	enum class Type
	{
		VERTEX_SHADER = GL_VERTEX_SHADER, FRAGMENT_SHADER = GL_FRAGMENT_SHADER
	};

	Shader( const char* source, Type type )
		: my_id{ glCreateShader( etoi( type ) ) }
	{
		read_source( source, type );
	}

	Shader( const std::string& source, Type type )
		: my_id{ glCreateShader( etoi( type ) ) }
	{
		read_source( source.c_str(), type );
	}

	const GLuint id() const
	{
		return my_id;
	}

private:
	static constexpr size_t ERROR_LOG_SIZE = 512;
	GLuint my_id;

	void read_source( const char* source, Type type )
	{
		glShaderSource( my_id, 1, &source, NULL );
		glCompileShader( my_id );

		int result;
		char errorlog[ ERROR_LOG_SIZE ];
		glGetShaderiv( my_id, GL_COMPILE_STATUS, &result );

		if ( !result )
		{
			glGetShaderInfoLog( my_id, ERROR_LOG_SIZE, NULL, errorlog );
			std::cerr << "ERROR : shader compile failed\n" << errorlog << '\n';
		}
	}
};

class ShaderProgram
{
public:
	void use()
	{
		glUseProgram( my_id );
	}

	template < typename ... Args >
	ShaderProgram( Args&& ... args ) : my_id{ glCreateProgram() }
	{
		int dummy[ sizeof...( Args ) ] = { ( shaders.push_back( std::forward< Args >( args ).id() ), 0 )... };

		for ( const auto s : shaders )
		{
			glAttachShader( my_id, s );
		}

		glLinkProgram( my_id );

		for ( const auto s : shaders )
		{
			glDeleteShader( s );
		}

		int result;
		char error_log[ ERROR_LOG_SIZE ];
		glGetProgramiv( my_id, GL_LINK_STATUS, &result );

		if ( !result )
		{
			glGetProgramInfoLog( my_id, ERROR_LOG_SIZE, NULL, error_log );
			std::cerr << "ERROR : shader program link failed\n" << error_log << std::endl;
		}
	}

	const GLuint id()
	{
		return my_id;
	}

private:
	static constexpr size_t ERROR_LOG_SIZE = 512;
	std::vector< GLuint > shaders;
	GLuint my_id;
};

#endif