#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/triangle_mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Le plan 
#include "objects/plane.hpp"

//source pnctuelle 
#include "lights/point_light.h"


#include "integrators/direct_lighting_integrator.h"

//source surfacique 
#include "lights/quadlight.h"

//Matériaux
#include "materials/lambert_material.hpp"
#include "materials/matte_material.h"
#include "materials/plastic_material.h"
#include "materials/cook_torrance.h"
#include "materials/mirror_material.h"


namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{

		// ================================================================
		// Les différents matériaux utilisés 
		// ================================================================*
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) ); 
		_addMaterial( new MatteMaterial( "CyanMatte", CYAN, 0.6f ) ); 

		/* Tests avec le matérial de Phong */
		_addMaterial( new PlasticMaterial( "plastic", GREY,BLUE,64.0f ) );

		_addMaterial( new PlasticMaterial( "plastic_gauche", GREY, RED, 32.0f ) );

		/* Le material miroir */
		_addMaterial( new MirrorMaterial("miroir") );

		/* Tests avec celui  de Cook-Torrance */
		Vec3f color	   = Vec3f( 1.f, 0.85f, 0.57f );
		float rugosite = 0.3f;
		float metal	   = 0.5f;
		_addMaterial( new CookMaterial( "cook", color, 0.0f, color, rugosite, metal ) );

		// pour le mur du fond 
		_addMaterial( new CookMaterial( "cook2", color, 0.0f, MAGENTA, rugosite, 0.8 ) );

		// pour le sol 
		_addMaterial( new CookMaterial( "cook3", color, 0.0f, MAGENTA, rugosite, metal ) );

		// ================================================================
		// Ajout d'objets 
		// ================================================================
		// Spheres .
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 2.5f ), 1.f ) );
		_attachMaterialToObject( "miroir", "Sphere1" );
		_addObject( new Sphere( "Sphere2", Vec3f( 3.f, 0.f, -5.f ), 2.f ) );
		_attachMaterialToObject( "cook", "Sphere2" );

		_addObject( new Sphere( "Sphere3", Vec3f( 3.5f, -1.f, -1.f ), 0.5f ) );
		_attachMaterialToObject( "miroir", "Sphere3" );

		_addObject( new Sphere( "Sphere4", Vec3f( -3.5f, -1.f, -4.f ), 2.f ) );
		_attachMaterialToObject( "plastic", "Sphere4" );


		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "cook3", "PlaneGround" );

		_addObject( new Plane( "PlaneLeft", Vec3f(15.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneLeft" );

		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 9.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "CyanMatte", "PlaneCeiling" );

		_addObject( new Plane( "PlaneRight", Vec3f( -15.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedMatte", "PlaneRight" );

		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "cook2", "PlaneFront" );
		// ================================================================
		// Ajout de la lumière 
		// ================================================================
		
		// lumière surfacique 
		_addLight( new QuadLight("q", WHITE, 
			Vec3f( 1.f, 4.f, -2.f ), 
			50.f, 
			Vec3f( -1.f, 0.f, 0.f ),
			Vec3f( 0.f, 1.f, 2.f )) );
		
	}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		uint cptTriangles = 0;
		uint cptVertices  = 0;

		for ( uint m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( uint v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( uint f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				//_addMaterial( new PlasticMaterial( std::string( mtlName.C_Str() ), kd, ks, s ) );
				//_attachMaterialToObject( mtlName.C_Str(), meshName );
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
