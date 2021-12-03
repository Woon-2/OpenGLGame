#ifndef _bitmap
#define _bitmap

#include <iostream>
#include <FreeImage.h>

struct Bitmap
{
	FIBITMAP* bitmap_handle;
	size_t width;
	size_t height;
	unsigned char* bits;

	void load( const char* file_name )
	{
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType( file_name );

		if ( format == -1 )
		{
			std::cerr << "Could not find image: \"" << file_name << "\"\n";
			return;
		}
		else if ( format == FIF_UNKNOWN )
		{
			std::cerr << "Couldn't determine file format - attempting to get from file extension...\n";
			format = FreeImage_GetFIFFromFilename( file_name );

			if ( !FreeImage_FIFSupportsReading( format ) )
			{
				std::cerr << "Detected image format cannot be read!\n";
				return;
			}
		}

		FIBITMAP* bitmap = FreeImage_Load( format, file_name );
		int bits_per_pixel = FreeImage_GetBPP( bitmap );

		if ( bits_per_pixel == 32 )
		{
			bitmap_handle = bitmap;
		}
		else
		{
			bitmap_handle = FreeImage_ConvertTo32Bits( bitmap );
		}

		width = FreeImage_GetWidth( bitmap_handle );
		height = FreeImage_GetHeight( bitmap_handle );
		bits = FreeImage_GetBits( bitmap_handle );
	}

	void unload()
	{
		if ( bitmap_handle )
		{
			FreeImage_Unload( bitmap_handle );
			bitmap_handle = nullptr;
		}
	}

	Bitmap() = default;

	Bitmap( const char* file_name )
	{
		load( file_name );
	}

	~Bitmap()
	{
		unload();
	}
};

#endif