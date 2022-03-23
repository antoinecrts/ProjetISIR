#ifndef __RT_ISICG_PLANE__
#define __RT_ISICG_PLANE__

#include "base_object.hpp"
#include "geometry/plane_geometry.hpp"

namespace RT_ISICG
{
	class Plane : public BaseObject
	{
	  public:
		Plane()		  = delete;
		virtual ~Plane() = default;

		Plane( const std::string & p_name, const Vec3f & point, const Vec3f & normal )
			: BaseObject( p_name ), _geometry( point, normal )
		{
		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		PlaneGeometry _geometry;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLANE__
