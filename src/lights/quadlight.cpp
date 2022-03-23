#include "quadlight.h"
#include "utils/random.hpp"
#include "glm/ext.hpp"

namespace RT_ISICG
{
	// Pour une source de lumière surfacique 
	LightSample QuadLight::sample( const Vec3f & p_point ) const 
	{ 

		Vec3f randomPos = _pos + _u * randomFloat() + _v * randomFloat();
		Vec3f direction = glm::normalize( randomPos - p_point );
		float distance = glm::distance( randomPos, p_point );
		float cos = glm::dot( _n, direction );
		float pdf = ( 1 / _area ) * ( distance * distance ) * cos;
		Vec3f radiance = ( _color * _power ) / pdf;

		if ( cos <= 0 ) { 
			return LightSample( direction, distance, VEC3F_ZERO, 1.f ); 
		}
		else
		{
			return LightSample( direction, distance, radiance, pdf );	
		}
	}



} // namespace RT_ISICG