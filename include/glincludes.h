#ifndef _glincludes
#define _glincludes

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
#include <tuple>

std::pair< float, float > glPtconvert( int wx, int wy, int width, int height )
{
    return std::make_pair( wx * 2.0f / width - 1.0f,
        wy * -2.0f / height + 1.0f );
}

template < size_t Dimension, typename Elem_t >
std::ostream& operator<<( std::ostream& os, const glm::vec< Dimension, Elem_t >& v )
{
    for ( int i = 0; i < v.length() - 1; ++i )
    {
        os << v[ i ] << ' ';
    }
    os << v[ length() - 1 ];

    return os;
}

#endif