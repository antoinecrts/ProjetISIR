#ifndef __RT_ISICG_PLANE_GEOMETRY__
#define __RT_ISICG_PLANE_GEOMETRY__

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class PlaneGeometry : public BaseGeometry
	{
	  public:
		PlaneGeometry()			  = delete;
		// virtual ~PlaneGeometry() = default;

		PlaneGeometry( const Vec3f & p, const Vec3f & n ) : _point( p ), _normal( n ) {}

		bool intersect( const Ray & p_ray, float & p_t1) const;

		inline const Vec3f & getNormal() const { return _normal;}

		private:
		Vec3f _point;
		Vec3f _normal;


	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLANE_GEOMETRY__
