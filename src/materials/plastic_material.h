#include "materials/base_material.hpp"

// beosin de la bdrf
#include "brdfs/phong_brdf.h"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name,
						 const Vec3f &		 p_diffuse,
						 const Vec3f &		 p_speculaire,
						 const float &		 s)
			: BaseMaterial( p_name ), _diff( p_diffuse ), _spec(p_speculaire,s)
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			Vec3f diff = _diff.evaluate();
			Vec3f spec = _spec.evaluate( p_ray, p_hitRecord, p_lightSample );

			return (0.7f * diff) + (spec * 0.3f) ; 
		}

		inline const Vec3f & getFlatColor() const override { 
			return _diff.getKd(), _spec.getKs() ; 
		}

	  protected:
		LambertBRDF _diff;
		PhongBRDF _spec;

	};

} // namespace RT_ISICG
