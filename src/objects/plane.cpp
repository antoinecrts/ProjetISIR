#include "plane.hpp"

namespace RT_ISICG
{
	bool Plane::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float t1;
		if ( _geometry.intersect( p_ray, t1) )
		{
			// Intersection found, fill p_hitRecord.
			if ( t1 >= p_tMin and t1 <= p_tMax )
			{
				p_hitRecord._point	= p_ray.pointAtT( t1 );
				p_hitRecord._normal = _geometry.getNormal();
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = t1;
				p_hitRecord._object	  = this;

				return true;
			}
		}
		return false;
	}

	bool Plane::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t1;
		if ( _geometry.intersect(p_ray, t1) ) { 
			if (t1 >= p_tMin and t1 <= p_tMin)
				return true;
		}
		return false;
	}

} // namespace RT_ISICG
