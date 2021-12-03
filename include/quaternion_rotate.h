#ifndef _quaternion_rotate
#define _quaternion_rotate

#include "glincludes.h"
#include <gl/glm/glm/gtc/quaternion.hpp>
#include <gl/glm/glm/gtx/quaternion.hpp>

template< typename Elem_t >
glm::mat< 4, 4, Elem_t > rotate_quat( const glm::mat< 4, 4, Elem_t >& mat, const glm::vec< 3, Elem_t >& euler_angles )
{
	glm::quat my_quat{ euler_angles };
	return mat * glm::toMat4< Elem_t >( my_quat );
}

#endif