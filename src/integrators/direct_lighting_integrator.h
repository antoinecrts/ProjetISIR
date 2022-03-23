#include "integrators/base_integrator.hpp"

namespace RT_ISICG
{
	class DirectLightingIntegrator : public BaseIntegrator
	{
	  public:
		DirectLightingIntegrator() : BaseIntegrator() {}
		virtual ~DirectLightingIntegrator() = default;

		const IntegratorType getType() const override { 
			return IntegratorType::DIRECT; 
		}

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		// il faut en mettre beaucoup 
		int _nbLightSamples = 50;



	};

} // namespace RT_ISICG
