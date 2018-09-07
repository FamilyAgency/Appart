#pragma once
#include "ofMain.h"
#include "Types.h"
#include "ArtDrawable.h"
#include "Config.h"

#include "../shaders/core/FrameShader.h"

#include "../shaders/core/ShaderTemplate.h"

#include "../shaders/noise/SimpleNoise.h"
#include "../shaders/noise/FurNoise.h"
#include "../shaders/noise/PlasticNoise.h"
#include "../shaders/noise/CurlNoise.h"

#include "../shaders/tunnel/SplineTunnel.h"

#include "../shaders/raymarching/MetaballShader.h"
#include "../shaders/raymarching/PixelMetaballShader.h"
#include "../shaders/raymarching/NoiseSphere.h"

#include "../shaders/kaleidoscope/KaleidoscopeTunnel.h"

#include "../geometry/infinity/Squares.h"
#include "../geometry/infinity/Triangles.h"
#include "../geometry/infinity/Hexagon.h"
#include "../geometry/infinity/InfinityDots.h"
#include "../geometry/infinity/Nautilus.h"

#include "../geometry/infinity/CubesArray.h"
#include "../geometry/infinity/CubesSteps.h"
#include "../geometry/infinity/CubesWave.h"

#include "../geometry/automata/GameOfLife.h"
#include "../geometry/automata/SnakesLife.h"

#include "../geometry/abstract/MondrianAnimated.h"
#include "../geometry/abstract/MondrianCube.h"
#include "../geometry/abstract/MondrianPingPong.h"
#include "../geometry/abstract/TwoCircles.h"
#include "../geometry/fractals/ApolloInfiniteZoom.h"


#include "../bigdata/FamilyTrain.h"




#include "../geometry/core/AnimGeometryExample.h"

#include "../artpacks/alice/AliceGrid.h"
#include "../artpacks/alice/AliceTunnel.h"
#include "../artpacks/alice/AliceRoom.h"
#include "../artpacks/alice/AliceNoise.h"


#include "../artpacks/classicglitch/ClassicGlitch.h"

#include "../shaders/geometry/RandomCircles.h"
#include "../shaders/geometry/RandomSquares.h"
#include "../shaders/geometry/DistortGrid.h"
#include "../shaders/geometry/RotatedGrid.h"
#include "../shaders/geometry/CircleMandala.h"
#include "../shaders/tunnel/HypnoTunnel.h"
#include "../shaders/fractals/ApolloFractal.h"

#include "../geometry/infinity/DotsRotation.h"
#include "../config/Config.h"


namespace frames
{
	typedef shared_ptr<class MainArtCreator> MainArtCreatorPtr;

	class MainArtCreator
	{
	public:
		virtual void create(synapse::ConfigPtr config);
		virtual frames::ArtDrawablePtr getArt(frames::FRAME_TYPES);
		vector<frames::ArtDrawablePtr> getAllForceUpdateArt();

	private:
		map<frames::FRAME_TYPES, frames::ArtDrawablePtr> artMap;
		void addArt(FRAME_TYPES type, const ArtDrawablePtr& art);
		synapse::ConfigPtr config;		
	};
}
