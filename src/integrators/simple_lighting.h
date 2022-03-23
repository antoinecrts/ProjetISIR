#include "integrators/base_integrator.hpp"

namespace RT_ISICG
{
	class SimpleLighting : public BaseIntegrator
	{
	  public:
		SimpleLighting() : BaseIntegrator() {}
		virtual ~SimpleLighting() = default;

		const IntegratorType getType() const override { return IntegratorType::SIMPLE; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	};

} // namespace RT_ISICG
