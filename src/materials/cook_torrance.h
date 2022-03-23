#pragma once 
#include "materials/base_material.hpp"

// beosin de la bdrf
#include "brdfs/oren_nayar_brdf.h"
#include "brdfs/micro_brdf.h"

namespace RT_ISICG
{
	class CookMaterial : public BaseMaterial
	{
	  public:
		CookMaterial( const std::string & p_name,
						 const Vec3f & p_diffuse,
						 const float & sigma,
						 const Vec3f & f0,
						 const float & rugosite,
						 const float & metal ) : BaseMaterial( p_name ), _diff(p_diffuse,sigma), _spec( f0, rugosite ), _metalness(metal) 
		{
		}

		virtual ~CookMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			Vec3f diffuse  = _diff.evaluate( p_ray, p_hitRecord, p_lightSample );
			Vec3f specular = _spec.evaluate( p_ray, p_hitRecord, p_lightSample );

			return (1.0f -_metalness) * diffuse + (_metalness * specular)  ;
		}

		inline const Vec3f & getFlatColor() const override { return VEC3F_ZERO; }

	  protected:
		OrenNayarBRDF _diff;
		MicroBRDF _spec;
		float _metalness;
	};

} // namespace RT_ISICG
