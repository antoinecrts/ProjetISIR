#pragma once
#include "integrators/base_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED; }

		Vec3f inline Li( const Scene & p_scene,
				   const Ray &	 p_ray,
				   const float	 p_tMin,
				   const float	 p_tMax) const override
		{
			return trace( p_scene, p_ray, p_tMin, p_tMax, 0);
		}

		Vec3f trace( const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 const float   nbBounces ) const;

	  private: 
		  // nombre de rayons d'ombrage lancés 
		  int _nbLightSamples = 50;
		  // nombre de rebond d'un rayon sur un miroir 
		  int _nbBounces	   = 5;

	};

} // namespace RT_ISICG
