#pragma once

#include "materials/base_material.hpp"

namespace RT_ISICG
{
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name ) : BaseMaterial( p_name ) {}

		~TransparentMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

		bool isTransparent() override { return true; }

		float getIOR() override { return _ior;
		}

	  private:
		float _ior = 1.3f;

	};

} // namespace RT_ISICG
