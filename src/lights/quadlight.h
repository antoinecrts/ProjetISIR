#pragma once

#include "defines.hpp"
#include "lights/base_light.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		QuadLight( const std::string & name,
				   const Vec3f &	   p_color,
				   const Vec3f &	   position,
				   const float		   p_power,
				   const Vec3f &	   u,
				   const Vec3f &	   v )
			: BaseLight( name, p_color, p_power ), _pos( position ), _u( u ), _v( v )
		{
		}

		LightSample sample( const Vec3f & p_point ) const override;

		bool getSurface() override { 
			return _isSurface;
		}

	  private:
		Vec3f _pos;
		Vec3f _u;
		Vec3f _v;
		Vec3f _n	= glm::normalize(glm::cross( _u, _v ));
		float _area = glm::length( _u ) * glm::length( _v );
		bool  _isSurface = true;
		


	};

} // namespace RT_ISICG