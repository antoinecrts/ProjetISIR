#pragma once
#include "defines.hpp"
#include "hit_record.hpp"
#include "lights/light_sample.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float & s ) : _ks( p_ks ), _s( s ) {};

		inline Vec3f evaluate( const Ray &		   p_ray,
							   const HitRecord &   p_hitRecord,
							   const LightSample & p_lightSample ) const
		{
			// Equation de la BRDF pour Phong 
			Vec3f wo = -p_ray.getDirection();
			Vec3f wi = p_lightSample._direction;
			Vec3f wr = glm::reflect(wi,p_hitRecord._normal);

			float cosalpha = glm::dot( normalize(wo), normalize(wr) );

			return _ks * pow( cosalpha, _s ); 
		
		}

		inline const Vec3f & getKs() const { return _ks; }

	  protected:
		Vec3f _ks = WHITE;
		float _s;
	};

} // namespace RT_ISICG