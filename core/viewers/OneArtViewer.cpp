#include "OneArtViewer.h"
#include "Config.h"
#include "../artpacks/classicglitch/ClassicGlitch.h"

#include "../artpacks/alice/AliceTunnel.h"
#include "../artpacks/alice/AliceGrid.h"
#include "../artpacks/alice/AliceTunnel2.h"
#include "../artpacks/alice/AliceRoom.h"

#include "../geometry/abstract/MondrianPingPong.h"
#include "../artpacks/classicglitch/glitchTest.h"
#include "../geometry/abstract/TwoCircles.h"
#include "../geometry/infinity/CubesRing.h"
#include "../geometry/infinity/CubesArray.h"
#include "../geometry/infinity/CubesWave.h"
#include "../geometry/infinity/Nautilus.h"

#include "../geometry/infinity/DotsRotation.h"

#include "../shaders/raymarching/RocksShader.h"

using namespace frames;

#define test
void OneArtViewer::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{
	ofSetWindowShape(config->getScreenWidth(), config->getScreenHeight());
	this->config = config;
	//artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_TEMPLATE));	
	//artVector.push_back(artCreator->getArt(FRAME_TYPES::FAMILY_TRAIN));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::CUBES_STEPS_3D_MODE1));
	

	artVector.push_back(artCreator->getArt(FRAME_TYPES::AUTOMA_GAME_OF_LIFE));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_CIRCLE_MANDALA));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_KALEIDOSCOPE_TUNNEL));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::DOTS_ROTATION));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::AUTOMA_GAME_OF_LIFE));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::ROTATED_GRID));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::HYPNO_TUNNEL));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::RANDOM_SQUARES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::DISTORT_GRID));	
	artVector.push_back(artCreator->getArt(FRAME_TYPES::CUBES_ARRAY_3D));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::CUBES_WAVE_3D));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_HEXAGON));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_SIMPLE_NOISE_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_PIXEL_METABALLS));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_METABALLS));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_SQUARES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_TRIANGLES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_CUBE));	
	artVector.push_back(artCreator->getArt(FRAME_TYPES::AUTOMA_GAME_OF_LIFE));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::NAUTILUS_MODE1));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_INFINITY_DOTS));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_TWO_CIRCLES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::ALICE_TUNNEL));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::ALICE_GRID));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::ALICE_NOISE));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GLITCH_SIMPLE));

	setOwnResolution();
	int w = config->getScreenWidth();
	int h = config->getScreenHeight();

	setFader(w, h);	

	AbstractSingleArtViewer::setup(artCreator, config);


#ifdef test

	//art = DotsRotationPtr(new DotsRotation(ofGetWindowWidth(), ofGetWindowHeight()));
	//art->GUISetup();
	//art->setup();

#endif
}

void OneArtViewer::setOwnResolution()
{
	int w = config->getScreenWidth();
	int h = config->getScreenHeight();

	for (size_t i = 0; i < artVector.size(); i++)
	{
		artVector[i]->setResolution(w, h);
	}
}
