#pragma once

#include "lights/base_light.hpp"
#include "defines.hpp"


namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
	  public: 
		PointLight( const std::string & name,
					const Vec3f &		p_color,
					const Vec3f &		pos_lum,
					const float			p_power)
			: BaseLight( name, p_color, p_power ), _pos( pos_lum )
		{
		}

	  LightSample sample( const Vec3f & p_point ) const override ; 

	  private:
		Vec3f _pos;

	};

} // namespace RT_ISICG
