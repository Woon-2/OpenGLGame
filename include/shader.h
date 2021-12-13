#ifndef _shader
#define _shader

#include <string>
#include "glincludes.h"
#include <iostream>
#include "TMP.h"
#include <vector>
#include "fileread.h"
#include "indent.h"

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
	static bool will_get_desc;

	void use() const
	{
		glUseProgram( my_id );
	}

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "Shader Program ID: " << my_id << '\n';
			ss << fore_indent << indent << "Shader IDs: [ ";
			for ( const auto sid : shaders )
			{
				ss << sid << ' ';
			}
			ss << "]\n";
			ss << fore_indent << indent << "Shaer Locations: [ ";
			for ( const auto loc : locations )
			{
				ss << loc << ' ';
			}
			ss << "]\n";
		}

		return ss;
	}

	const GLuint push_location( GLuint location )
	{
		locations.push_back( location );
		return locations.size() - 1;
	}

	const GLuint get_location( GLuint at ) const
	{
		if ( at >= locations.size() )
		{
			std::cerr << "ShaderProgram::get_location( GLuint ): get received " << at << " but locations.size() was " << locations.size() << '\n';
			return 12345678;
		}
		else return locations[ at ];
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

	const GLuint id() const
	{
		return my_id;
	}

	~ShaderProgram()
	{
		glDeleteProgram( my_id );
	}

private:
	static constexpr size_t ERROR_LOG_SIZE = 512;
	std::vector< GLuint > shaders;
	std::vector< GLuint > locations;
	GLuint my_id;
};

bool ShaderProgram::will_get_desc = true;

#endif