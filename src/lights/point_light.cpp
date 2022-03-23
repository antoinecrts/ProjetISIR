#include "point_light.h"

namespace RT_ISICG
{
	// Pour une source de lumière ponctuelle 
	LightSample PointLight::sample(const Vec3f & p_point) const
	{
		
		Vec3f direction = glm::normalize( _pos - p_point);
		float distance	= glm::distance( _pos,p_point );
		Vec3f radiance	= ( _color * _power ) / ( distance * distance );

		return LightSample( direction, distance, radiance, 1.f );

	}


}