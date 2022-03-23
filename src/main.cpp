#include "cameras/perspective_camera.hpp"
#include "defines.hpp"
#include "renderer.hpp"

namespace RT_ISICG
{
	int main( int argc, char ** argv )
	{
		const int imgWidth	= 600;
		const int imgHeight = 400;

		// Create a texture to render the scene.
		Texture img = Texture( imgWidth, imgHeight );

		// Create and init scene.
		Scene scene;
		scene.init();

		// Create a perspective camera.
		// Utilisation d'une caméra en définissant uniquement l'aspect ratio de celle-ci 
		PerspectiveCamera camera( float( imgWidth ) / imgHeight );
		Vec3f p	= Vec3f( 0.f, 0.f, 12.f);
		Vec3f lat = Vec3f( 0.f, 0.f, 10.f);
		Vec3f up = Vec3f( 0.f, 1.f, 0.f);
		float fo  = 60.f;
		float asp_ratio = float( imgWidth ) / imgHeight;
		// PerspectiveCamera camera( p, lat, up, fo, asp_ratio );
		

		// Create and setup the renderer.
		Renderer renderer;
		//renderer.setIntegrator( IntegratorType::RAY_CAST );
		//renderer.setIntegrator( IntegratorType::DIRECT ); 
		// l'intégrateur que l'on va utiliser pour le rendu 
		// celui utilisé correspond à la version amélioré des précédents
		renderer.setIntegrator(IntegratorType::WHITTED); 
		renderer.setBackgroundColor( GREY );

		// Nombre d'échantillons par pixel
		renderer.setNbPixelSamples(128);

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

		float renderingTime = renderer.renderImage( scene, &camera, img );

		std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

		// Save rendered image.
		const std::string imgName = "image.jpg";
		img.saveJPG( RESULTS_PATH + imgName );

		return EXIT_SUCCESS;
	}
} // namespace RT_ISICG

int main( int argc, char ** argv )
{
	try
	{
		return RT_ISICG::main( argc, argv );
	}
	catch ( const std::exception & e )
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
	}
}
