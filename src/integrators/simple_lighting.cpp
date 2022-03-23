#include "integrators/simple_lighting.h"

namespace RT_ISICG
{
	Vec3f SimpleLighting::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitr;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitr ) )
		{
			/// version avec intersectioany qui fait bloquer le programme

			Vec3f li = Vec3f( 0.0f );

			for ( int i = 0; i < p_scene.getLights().size(); i++ )
			{
				LightSample ls = p_scene.getLights()[ i ]->sample( hitr._point );

				li += hitr._object->getMaterial()->getFlatColor() * ls._radiance
					* glm::max( 0.0f, glm::dot( normalize( ls._direction ), hitr._normal ) );
						
			}
			return li ;
		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG