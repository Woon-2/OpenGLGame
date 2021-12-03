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

#endif