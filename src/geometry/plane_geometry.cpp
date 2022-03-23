#include "plane_geometry.hpp"

namespace RT_ISICG
{
	// fonction d'intersection entre le rayon et le plan 
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1 ) const
	{ 
		float d = dot( p_ray.getDirection(), _normal );
		if ( d >= 0.0f ) return false;
		p_t1 = dot( _normal, _point - p_ray.getOrigin() ) / d;
		return true;
	}

} // namespace RT_ISICG
