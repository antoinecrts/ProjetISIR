#pragma once
#include "defines.hpp"
#include "hit_record.hpp"
#include "lights/light_sample.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class MicroBRDF
	{
	  public:
		MicroBRDF( const Vec3f & f, const float & sigma ) : _f0( f ), _sigma( sigma ) {};

		
		/* Calculs des différents fonctions pour une BRDF avec micro facettes */

		inline Vec3f H( const Vec3f & wo, const Vec3f & wi ) const
		{ 
			Vec3f r	 = wo + wi;
			return normalize(r);
		}


		inline float D( const Vec3f & h, const Vec3f & n) const
		{ 
			
			float alpha = _sigma * _sigma;
			float alpha2 = alpha * alpha; 
			
			float a	 = dot( n, h );
			float a2 = a * a;
			float b	 = alpha2 - 1;
			float c	 = (a2 * b) + 1;
			return alpha2 / (( c * c ) * PIf);
		
		}

		inline float G1(const float x) const { 
		
			float a = ( _sigma + 1);
			float a2 = a * a;
			float k	= a2 / 8;

			return x / ( x * ( 1 - k ) + k );
			
		}


		inline float G( const Vec3f & wo, const Vec3f & wi, const Vec3f & n ) const
		{
			float x = dot(n,wo );
			float a = G1( x );
			float y = dot(n,wi);
			float b = G1( y );

			return a * b ; 
		
			
		}

		inline Vec3f F( const Vec3f & wo, const Vec3f & h ) const
		{ 
			
			float a	 = dot( wo, h );
			float b	 = ( 1 - a );
			Vec3f c	 = ( 1.0f - _f0 );

			return _f0 + (c * float(pow(b, 5 )));
			

		}


		inline Vec3f evaluate( const Ray &		   p_ray,
							   const HitRecord &   p_hitRecord,
							   const LightSample & p_lightSample ) const
		{

			Vec3f wo = -p_ray.getDirection();
			Vec3f wi = p_lightSample._direction;
			Vec3f n	 = p_hitRecord._normal;


			Vec3f h = H( wo, wi );
			float d = D( h, n );
			float g = G( wo, n, wi );
			Vec3f f = F( wo, h );

			float a = dot( wo, n);
			float b = dot(wi, n );

			return (d * f * g) / (4 * a * b );
		}

		inline const Vec3f & getf() const { return _f0; }

	  protected:
		Vec3f _f0 = WHITE;
		float _sigma;
	};

} // namespace RT_ISICG