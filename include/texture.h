#ifndef _texture
#define _texture

#include "glincludes.h"
#include "bitmap.h"
#include "pool.h"

class Texture
{
public:
	static constexpr const size_t tex_pool_size = 0x1000;
	static constexpr const float border_color[] = { 0.05f, 0.05f, 0.05f };

	void load( const char* file_name )
	{
		my_bitmap.load( file_name );

		bind_this();

		glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color );

		glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, my_bitmap.width, my_bitmap.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, my_bitmap.bits );
		glGenerateMipmap( GL_TEXTURE_2D );

		my_bitmap.unload();
	}

	void bind_this() const
	{
		glUniform1i( location_tex, 0 );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, id );
	}

	void unbind_this() const
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	Texture( GLuint location_tex ) : location_tex{ location_tex }, my_bitmap{}
	{
		glGenTextures( 1, &id );
	}

	Texture( const char* file_name, GLuint location_tex ) : Texture( location_tex )
	{
		load( file_name );
	}

	~Texture()
	{
		glDeleteTextures( 1, &id );
	}

private:
	GLuint location_tex;
	GLuint id;
	Bitmap my_bitmap;
};

pool< Texture > pool_texture{ Texture::tex_pool_size };

#endif