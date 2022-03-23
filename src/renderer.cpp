#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

#include "integrators/direct_lighting_integrator.h"
#include "integrators/simple_lighting.h"
#include "integrators/whitted_integrator.h"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		
		case IntegratorType::DIRECT: 
			_integrator = new DirectLightingIntegrator(); 
			break;
		case IntegratorType::SIMPLE: 
			_integrator = new SimpleLighting(); 
			break;
		case IntegratorType::WHITTED: 
			_integrator = new WhittedIntegrator(); 
			break;
		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		#pragma omp parallel for 
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				// Antialiasing 
				// on lance plusieurs rayons par pixels, on peut ensuite récupérer la couleur moyenne 
				Vec3f moy_color = Vec3f(0.0f,0.0f,0.0f);

				for (int n = 0; n < _nbPixelSamples; n++) {
					float a = randomFloat();
					float b = randomFloat();
					Ray r = p_camera->generateRay( (float(i + a)) / width, (float(j + b)) / height );
					Vec3f color = _integrator->Li( p_scene, r, 0, 10000 );
					moy_color = moy_color + color;
				}
				moy_color = moy_color / (float) _nbPixelSamples; 
				p_texture.setPixel( i, j,glm::clamp(moy_color,Vec3f (0.0f), Vec3f (1.0f)));
			
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
