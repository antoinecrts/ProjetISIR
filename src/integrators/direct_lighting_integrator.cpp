#include "direct_lighting_integrator.h"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitr;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax,hitr) )
		{
			/// version avec intersectioany qui fait bloquer le programme 

			Vec3f li = Vec3f( 0.0f );

			for ( int i = 0; i < p_scene.getLights().size(); i++ )
			{
				// source lumineuse surfacique 
				if ( p_scene.getLights()[ i ]->getSurface() )
				{
					#pragma omp parallel for
					for ( int j = 0; j < _nbLightSamples; j++ )
					{
						LightSample ls = p_scene.getLights()[ i ]->sample( hitr._point );

						Ray r = Ray( hitr._point, ls._direction );
						r.offset( hitr._normal );

						if ( !p_scene.intersectAny( r, p_tMin, ls._distance ) )
						{
							li += hitr._object->getMaterial()->shade(p_ray,hitr,ls) * ls._radiance
								  * glm::max( 0.0f, glm::dot( normalize( ls._direction ), hitr._normal ) );
						}
					}
					li /= (float) _nbLightSamples;
				} 
				else // source de lumière ponctuelle 
				{	
					LightSample ls = p_scene.getLights()[ i ]->sample( hitr._point );

					Ray r = Ray( hitr._point, ls._direction );
					r.offset( hitr._normal );
					if ( !p_scene.intersectAny(r, p_tMin, ls._distance ) )
						li += hitr._object->getMaterial()->shade( p_ray,hitr,ls) * ls._radiance
							* glm::max( 0.0f, glm::dot( normalize( ls._direction ), hitr._normal ) );
				
				}
			}
			return li; 
		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
