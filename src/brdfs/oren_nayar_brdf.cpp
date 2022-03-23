#include "brdfs/oren_nayar_brdf.h"

namespace RT_ISICG
{
	// Equation de la BRDF pour celle de Oren-Nayar
	Vec3f OrenNayarBRDF::evaluate( const Ray & p_ray, const HitRecord & p_hitRecord, const LightSample & p_lightSample ) const 
	{
		Vec3f wo = -p_ray.getDirection();
		Vec3f wi = p_lightSample._direction;

		float costetao = glm::dot( wo, p_hitRecord._normal );
		float costetai = glm::dot( wi, p_hitRecord._normal );
		float tetao	   = glm::acos( costetao );
		float tetai	   = glm::acos( costetai );
		Vec3f wo_phi   = normalize (( wo - p_hitRecord._normal ) * costetao );

		Vec3f wi_phi = normalize( ( wi - p_hitRecord._normal ) * costetao );

		float cosphioi = glm::dot( wo_phi, wi_phi );

		float alpha = glm::max( tetao, tetai );
		float beta	= glm::min( tetao, tetai );

		float acarre = _pente * _pente;
		float a		 = 1 - ( 0.5 * ( acarre / ( acarre + 0.33 ) ) );
		float b		 = 0.45 * ( acarre / ( acarre + 0.09 ) );

		Vec3f fr = ( _kd * INV_PIf ) * ( a + ( b * glm::max( 0.f, cosphioi ) ) * sin( alpha ) * tan( beta ) );

		return fr ;
	}

}
	
