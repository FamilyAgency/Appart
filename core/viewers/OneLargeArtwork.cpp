#include "OneLargeArtwork.h"
#include "Config.h"

using namespace frames;

void OneLargeArtwork::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{		
	this->config = config;
	ofSetWindowShape(config->getScreenWidthLarge(), config->getScreenHeight());

	//artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_TEMPLATE));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::AUTOMA_SNAKES_LIFE));

	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE4));	
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_CIRCLE_MANDALA));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GLITCH_SIMPLE));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::RANDOM_CIRCLES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::ALICE_NOISE));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_PIXEL_METABALLS));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::RANDOM_SQUARES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GLITCH_SIMPLE));
//	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_SQUARES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_TWO_CIRCLES));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GLITCH_SIMPLE));
	//artVector.push_back(artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_HOR));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::SHADER_METABALLS));
	//artVector.push_back(artCreator->getArt(FRAME_TYPES::AUTOMA_GAME_OF_LIFE));
	artVector.push_back(artCreator->getArt(FRAME_TYPES::GLITCH_SIMPLE));
	setOwnResolution();

	//artVector.push_back(artCreator->getArt(FRAME_TYPES::CUBES_ARRAY_3D));
	//artVector.push_back(artCreator->getArt(FRAME_TYPES::CUBES_WAVE_3D));

	int w = config->getScreenWidthLarge();
	int h = config->getScreenHeight();
	
	setFader(w, h);
	AbstractSingleArtViewer::setup(artCreator, config);
}

void OneLargeArtwork::setOwnResolution()
{
	int w = config->getScreenWidthLarge();
	int h = config->getScreenHeight();

	for (size_t i = 0; i < artVector.size(); i++)
	{
		artVector[i]->setResolution(w, h);
	}
}

