#pragma once
#include "defines.hpp"

#include "ray.hpp"
#include "hit_record.hpp"
#include "lights/light_sample.hpp"

namespace RT_ISICG 
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float & sigma ) : _kd( p_kd ), _pente( sigma ) {};

		Vec3f evaluate ( const Ray & p_ray, const HitRecord & p_hitRecord, const LightSample & p_lightSample ) const ;

		inline const Vec3f & getKd() const { return _kd; }

	  protected:
		Vec3f _kd = WHITE;
		float _pente;
	};

} // namespace RT_ISICG