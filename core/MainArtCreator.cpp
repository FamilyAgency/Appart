#include "MainArtCreator.h"
using namespace frames;

void MainArtCreator::create(synapse::ConfigPtr config)
{
	this->config = config;

	int w = config->getScreenWidth();
	int h = config->getScreenHeight();

	// ------------------------------------- SHADERS -------------------------------------//
	addArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL, ApolloFractalPtr(new ApolloFractal(w, h)));
	addArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE1, ApolloFractalPtr(new ApolloFractal(w, h, 5., 5.3, 3.5, 1.1)));
	addArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE2, ApolloFractalPtr(new ApolloFractal(w, h, 5., 3.9, 6.4, 2.)));
	addArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE3, ApolloFractalPtr(new ApolloFractal(w, h, 5., 5.3, 3.5, 1.1)));
	addArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE4, ApolloFractalPtr(new ApolloFractal(w, h, 9.9, 11.8, 7.2, 1.1)));
	
	//addArt(FRAME_TYPES::SHADER_TEMPLATE, ShaderTemplatePtr(new ShaderTemplate(w, h)));
	addArt(FRAME_TYPES::ALICE_NOISE, AliceNoisePtr(new AliceNoise(w, h)));

	addArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE1, FurNoisePtr(new FurNoise(w, h, 8.065, 2.14, 0.4, ofColor(144, 70, 54), ofColor(161, 188, 162), ofColor(179, 120, 240))));
	addArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE2, FurNoisePtr(new FurNoise(w, h, 2.4265, 2.14, 0.4, ofColor(144, 83, 54), ofColor(255, 72, 33), ofColor(186, 255, 240))));
	addArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE3, FurNoisePtr(new FurNoise(w, h, 0.8265, 2.14, 0.4, ofColor(41, 67, 54), ofColor(158, 199, 197), ofColor(227, 186, 240))));

	addArt(FRAME_TYPES::GEOMETRY_INFINITY_DOTS, InfinityDotsPtr(new InfinityDots(w, h)));

	addArt(FRAME_TYPES::SHADER_SIMPLE_NOISE_MODE1, SimpleNoisePtr(new SimpleNoise(w, h, 64, 2.295, ofVec2f(52., 100.), ofColor(48, 160, 135), ofColor(74, 143, 178))));
	addArt(FRAME_TYPES::SHADER_SIMPLE_NOISE_MODE2, SimpleNoisePtr(new SimpleNoise(w, h , 45, 2, ofVec2f(100., 100.), ofColor(18, 202, 117), ofColor(100, 10, 14))));
	addArt(FRAME_TYPES::SHADER_SIMPLE_NOISE_MODE3, SimpleNoisePtr(new SimpleNoise(w, h, 64, 2.58, ofVec2f(52., 100.), ofColor(36, 42, 191), ofColor(70, 153, 48))));

	addArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE1, PlasticNoisePtr(new PlasticNoise(w, h, 0.08, 3.9, 0.13, ofColor(230., 120, 100))));
	addArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE2, PlasticNoisePtr(new PlasticNoise(w, h)));
	addArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE3, PlasticNoisePtr(new PlasticNoise(w, h, 0.025, 2.9, 0.25, ofColor(71, 126, 158))));

	addArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE1, SplineTunnelPtr(new SplineTunnel(w, h, 2.5, 0.25, ofColor(255., 255., 255.))));
	addArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE2, SplineTunnelPtr(new SplineTunnel(w, h, 2.5, 0.9, ofColor(255., 255., 255.))));
	addArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE3, SplineTunnelPtr(new SplineTunnel(w, h, 2.5, 0.3, ofColor(255., 255., 255.))));

	addArt(FRAME_TYPES::SHADER_PIXEL_METABALLS, PixelMetaballShaderPtr(new PixelMetaballShader(w, h)));
	addArt(FRAME_TYPES::SHADER_METABALLS, MetaballShaderPtr(new MetaballShader(w, h)));

	addArt(FRAME_TYPES::RANDOM_SQUARES, RandomSquaresPtr(new RandomSquares(w, h)));
	addArt(FRAME_TYPES::RANDOM_CIRCLES, RandomCirclesPtr(new RandomCircles(w, h)));
	
	addArt(FRAME_TYPES::DISTORT_GRID, DistortGridPtr(new DistortGrid(w, h)));
	addArt(FRAME_TYPES::HYPNO_TUNNEL, HypnoTunnelPtr(new HypnoTunnel(w, h)));
	addArt(FRAME_TYPES::ROTATED_GRID, RotatedGridPtr(new RotatedGrid(w, h)));
	addArt(FRAME_TYPES::SHADER_KALEIDOSCOPE_TUNNEL, KaleidoscopeTunnelPtr(new KaleidoscopeTunnel(w, h)));
	addArt(FRAME_TYPES::SHADER_CIRCLE_MANDALA, CircleMandalaPtr(new CircleMandala(w, h)));	

	// ------------------------------------- GEOMETRY -------------------------------------//
	addArt(FRAME_TYPES::GEOMETRY_APOLLO_FRACTAL_INF_ZOOM, ApolloInfiniteZoomPtr(new ApolloInfiniteZoom(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_SQUARES, SquaresPtr(new Squares(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_TRIANGLES, TrianglesPtr(new Triangles(w, h)));

	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE1, MondrianPingPongPtr(new MondrianPingPong(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE2, MondrianPingPongPtr(new MondrianPingPong(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE3, MondrianPingPongPtr(new MondrianPingPong(w, h)));
	//addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_HOR, MondrianPingPongHorPtr(new MondrianPingPongHor(w, h)));

	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE1, MondrianAnimatedPtr(new MondrianAnimated(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE2, MondrianAnimatedPtr(new MondrianAnimated(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE3, MondrianAnimatedPtr(new MondrianAnimated(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_MONDRIAN_CUBE, MondrianCubePtr(new MondrianCube(w, h)));

	addArt(FRAME_TYPES::GEOMETRY_HEXAGON, HexagonPtr(new Hexagon(w, h)));
	addArt(FRAME_TYPES::GEOMETRY_TWO_CIRCLES, TwoCirclesPtr(new TwoCircles(w, h)));
	
	addArt(FRAME_TYPES::AUTOMA_GAME_OF_LIFE, GameOfLifePtr(new GameOfLife(w, h)));
	addArt(FRAME_TYPES::AUTOMA_SNAKES_LIFE, SnakesLifePtr(new SnakesLife(w, h)));

	addArt(FRAME_TYPES::NAUTILUS_MODE1, NautilusPtr(new Nautilus(w, h, ofColor(200, 200, 200), ofColor(0, 0, 0), ofColor(0,0,0), 450, 0, -45, 15, 30)));
	addArt(FRAME_TYPES::NAUTILUS_MODE2, NautilusPtr(new Nautilus(w, h, ofColor(25, 25, 25), ofColor(200, 200, 200), ofColor(255, 255, 255), 360, 0, -20, 50, 10)));
	addArt(FRAME_TYPES::NAUTILUS_MODE3, NautilusPtr(new Nautilus(w, h, ofColor(180, 170, 220), ofColor(20, 0, 10), ofColor(0, 0, 0), 550, 25, -90, 60, 60)));
		
	addArt(FRAME_TYPES::CUBES_ARRAY_3D, CubesArrayPtr(new CubesArray(w, h)));
	addArt(FRAME_TYPES::CUBES_STEPS_3D_MODE1, CubesStepsPtr(new CubesSteps(w, h, CubesSteps::ColorCodes::COLOR1)));
	addArt(FRAME_TYPES::CUBES_STEPS_3D_MODE2, CubesStepsPtr(new CubesSteps(w, h, CubesSteps::ColorCodes::COLOR2)));
	addArt(FRAME_TYPES::CUBES_STEPS_3D_MODE3, CubesStepsPtr(new CubesSteps(w, h, CubesSteps::ColorCodes::COLOR3)));

	addArt(FRAME_TYPES::CUBES_STEPS_LINES_3D, CubesStepsPtr(new CubesStepsLines(w, h)));

	addArt(FRAME_TYPES::CUBES_WAVE_3D, CubesWavePtr(new CubesWave(w, h)));

	addArt(FRAME_TYPES::DOTS_ROTATION, DotsRotationPtr(new DotsRotation(w, h)));

	// ------------------------------------- ART PACKS -------------------------------------//


	// ------------------------------------- ALICE -------------------------------------//
	addArt(FRAME_TYPES::ALICE_GRID, AliceGridPtr(new AliceGrid(w, h)));
	addArt(FRAME_TYPES::ALICE_TUNNEL, AliceTunnelPtr(new AliceTunnel(w, h)));
	addArt(FRAME_TYPES::ALICE_ROOM, AliceRoomPtr(new AliceRoom(w, h)));

	// ------------------------------------- GLITCH -------------------------------------//
	addArt(FRAME_TYPES::GLITCH_SIMPLE, ClassicGlitchPtr(new ClassicGlitch(w, h)));

	// ------------------------------------- BIG DATA -------------------------------------//
	addArt(FRAME_TYPES::FAMILY_TRAIN, FamilyTrainPtr(new FamilyTrain(w, h, config)));
}

vector<frames::ArtDrawablePtr> MainArtCreator::getAllForceUpdateArt()
{
	vector<frames::ArtDrawablePtr> forceUpdateArts;

	for (auto &item : artMap)
	{
		ArtDrawablePtr art = item.second;
		if (art->isForceUpdate())
		{
			forceUpdateArts.push_back(art);
		}
	}

	return forceUpdateArts;
}

frames::ArtDrawablePtr MainArtCreator::getArt(frames::FRAME_TYPES type)
{
	return artMap.at(type);
}

void MainArtCreator::addArt(FRAME_TYPES type, const ArtDrawablePtr& art)
{	
	art->initFbo(config->getfboNumSamples());
	art->GUISetup();
	art->setType(type);	
	artMap.insert(pair<FRAME_TYPES, ArtDrawablePtr>(type, art));
}