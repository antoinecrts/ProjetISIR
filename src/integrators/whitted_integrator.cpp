#include "whitted_integrator.h"

#include "utils/random.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::trace( const Scene & p_scene,
									const Ray &	  p_ray,
									const float	  p_tMin,
									const float	  p_tMax,
									const float	  nbBounces ) const
	{
		HitRecord hitr;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitr ) )
		{
			/* Condition d'arrêt */
			if ( nbBounces >= _nbBounces ) return BLACK;

			/* */
			Vec3f li = Vec3f( 0.0f );

			for ( int i = 0; i < p_scene.getLights().size(); i++ )
			{
				// si le materiau est un miroir
				if ( hitr._object->getMaterial()->isMirror() )
				{
					Ray reflexion = Ray( hitr._point, glm::reflect( p_ray.getDirection(), hitr._normal ) );
					li += trace( p_scene, reflexion, p_tMin, p_tMax, nbBounces + 1 );
				}
				
				else
				{
					// source lumineuse surfacique
					if ( p_scene.getLights()[ i ]->getSurface() )
					{
						Vec3f moy_rayon = BLACK;

						#pragma omp parallel for
						// on lance plusieurs rayons d'ombrages pour diminuer le bruit 
						for ( int j = 0; j < _nbLightSamples; j++ )
						{
							LightSample ls = p_scene.getLights()[ i ]->sample( hitr._point );

							// rayon d'ombrage qui part du point d'intersection vers la source de lumière 
							Ray r = Ray( hitr._point, ls._direction );
							r.offset( hitr._normal );

							// si ce rayon intersecte quelque chose alors le point est dans l'ombre
							if ( !p_scene.intersectAny( r, p_tMin, ls._distance ) )
							{
								moy_rayon += hitr._object->getMaterial()->shade( p_ray, hitr, ls ) * ls._radiance
											 * glm::max( 0.0f, glm::dot( normalize( ls._direction ), hitr._normal ) );
							}
						}

						moy_rayon /= (float) _nbLightSamples;
						li += moy_rayon;
					}
					else // source de lumière ponctuelle
					{
						// si la source de lumière est ponctuelle on a pas besoin de lancer plusieurs rayons d'ombrages
						LightSample ls = p_scene.getLights()[ i ]->sample( hitr._point );

						Ray r = Ray( hitr._point, ls._direction );
						r.offset( hitr._normal );
						if ( !p_scene.intersectAny( r, p_tMin, ls._distance ) )
							li += hitr._object->getMaterial()->shade( p_ray, hitr, ls ) * ls._radiance
								  * glm::max( 0.0f, glm::dot( normalize( ls._direction ), hitr._normal ) );
					}
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
