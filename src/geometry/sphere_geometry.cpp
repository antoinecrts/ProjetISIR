#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	/* definition de la fonction d'interection pour une sphère */
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{

		Vec3f oc = _center - p_ray.getOrigin();
		float oh = dot( oc, p_ray.getDirection() );

		if ( oh < 0 ) { 
			return false;
		}
			
		float ch2 = dot( oc, oc ) - (oh * oh) ;
		float r2  = _radius * _radius;
		if (ch2 > r2) { 
			return false;
		}

		float d2 = r2 - ch2; 

		float t1 = oh + sqrt( d2 );
		float t2 = oh - sqrt( d2 );

		p_t1 = glm::min(t1,t2);
		p_t2 = glm::max(t1,t2);

		return true;
		
		
		
	}

} // namespace RT_ISICG
