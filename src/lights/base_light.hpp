#ifndef __RT_ISICG_BASE_LIGHT__
#define __RT_ISICG_BASE_LIGHT__

#include "light_sample.hpp"

namespace RT_ISICG
{
	class BaseLight 
	{
	  public:
		BaseLight( const std::string & p_name, const Vec3f & p_color, const float p_power )
			: _name( p_name ), _color( p_color ), _power( p_power ) {}
		virtual ~BaseLight() = default;

		inline const Vec3f & getFlatColor() const { return _color; }

		virtual LightSample sample( const Vec3f & p_point ) const = 0;

		virtual bool getSurface()
		{
			return _isSurface ; 
		}

	  protected:
		// TODO: give a name, like objects and materials
		std::string _name;
		float _power = 1.f;
		Vec3f _color = WHITE;
		bool _isSurface = false;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__
