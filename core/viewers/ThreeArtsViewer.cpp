#include "ThreeArtsViewer.h"
#include "Config.h"

using namespace frames;

ThreeArtsViewer::ThreeArtsViewer()
{
	spoutChannelName = "Triple art channel";
}

void ThreeArtsViewer::setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)
{
	this->config = config;
	ofSetWindowShape(config->getScreenWidthLarge(), config->getScreenHeight());

	oneWidth = config->getScreenWidth();
	bigFBO.allocate(config->getScreenWidthLarge(), config->getScreenHeight(), GL_RGBA, config->getfboNumSamplesThreeArts());

	TripleArt tripleArt;	

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::CUBES_STEPS_3D_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::CUBES_STEPS_3D_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::CUBES_STEPS_3D_MODE3);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::SHADER_APOLLO_FRACTAL_MODE3);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::ROTATED_GRID);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::HYPNO_TUNNEL);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::DISTORT_GRID);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::GEOMETRY_SQUARES);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::GEOMETRY_TRIANGLES);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::GEOMETRY_INFINITY_DOTS);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::SHADER_FUR_NOISE_MODE3);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::ALICE_GRID);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::ALICE_ROOM);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::ALICE_TUNNEL);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_ANIMATED_MODE3);
	tripleArts.push_back(tripleArt);

	//tripleArt.art1 = artCreator->getArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE1);
	//tripleArt.art2 = artCreator->getArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE2);
	//tripleArt.art3 = artCreator->getArt(FRAME_TYPES::SHADER_SPLINE_TUNNEL_MODE3);
	//tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::NAUTILUS_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::NAUTILUS_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::NAUTILUS_MODE3);
	tripleArts.push_back(tripleArt);

	//tripleArt.art1 = artCreator->getArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE1);
	//tripleArt.art2 = artCreator->getArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE2);
	//tripleArt.art3 = artCreator->getArt(FRAME_TYPES::SHADER_PLASTIC_NOISE_MODE3);
	//tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::GEOMETRY_TRIANGLES);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::GEOMETRY_SQUARES);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::GEOMETRY_HEXAGON);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::ALICE_NOISE);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::SHADER_CIRCLE_MANDALA);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::ALICE_NOISE);
	tripleArts.push_back(tripleArt);

	tripleArt.art1 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE1);
	tripleArt.art2 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE2);
	tripleArt.art3 = artCreator->getArt(FRAME_TYPES::GEOMETRY_MONDRIAN_PING_PONG_MODE3);
	tripleArts.push_back(tripleArt);	

	setOwnResolution();
	setLargeFader();

	if (config->getArtRotateMode())
	{
		setArtRotator(config->getChangeArtSeconds());
	}

	currentArtIndex = 0;
	switchArt();
}

void ThreeArtsViewer::setOwnResolution()
{
	int w = config->getScreenWidth();
	int h = config->getScreenHeight();


	for (size_t i = 0; i < tripleArts.size(); i++)
	{
		tripleArts[i].art1->setResolution(w, h);
		tripleArts[i].art2->setResolution(w, h);
		tripleArts[i].art3->setResolution(w, h);
	}
}

void ThreeArtsViewer::mapThreeArtsFrames()
{
	
}

void ThreeArtsViewer::update()
{
	ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
	currentTripleArt.update();
	timeRotatorCheck();	
}

void ThreeArtsViewer::draw()
{
	bigFBO.begin();
	ofBackground(255);
	ofClear(255, 255, 255, 0);
	ofEnableAlphaBlending();
	ofPushMatrix();
		currentTripleArt.art1->draw();
		ofTranslate(oneWidth, 0);
		currentTripleArt.art2->draw();
		ofTranslate(oneWidth, 0);
		currentTripleArt.art3->draw();
	ofPopMatrix();
	bigFBO.end();

	bigFBO.draw(0, 0);

	drawFader();
}

void ThreeArtsViewer::setArtRotator(int seconds)
{
	secondsArtShow = seconds;

	for (auto& itam : tripleArts)
	{
		itam.GUIvisible(false);
	}
	
	curentSaveTime = ofGetElapsedTimef();
}

void ThreeArtsViewer::nextArt()
{
	currentArtIndex++;
	if (currentArtIndex >= tripleArts.size())
	{
		currentArtIndex = 0;
	}

	switchArt();

	curentSaveTime = ofGetElapsedTimef();
}

void ThreeArtsViewer::switchArt()
{
	currentTripleArt.clean();
	currentTripleArt = tripleArts[currentArtIndex];
	currentTripleArt.setup();
}

ofTexture ThreeArtsViewer::getTexture() const
{
	return bigFBO.getTexture();
}

void ThreeArtsViewer::clean()
{

}
