#include "CubesSteps.h"

using namespace frames;

CubesSteps::CubesSteps(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Cubes Steps";


	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
	moveZSetup();
	
	setBoxColorsBase();
}
CubesSteps::CubesSteps(float width, float height, ColorCodes colorSetID) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Cubes Steps";

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());
	moveZSetup();

	setBoxColorsBase();
	presetColorsFlag = true;
	colorPresetID = int(colorSetID);
}
CubesStepsLines::CubesStepsLines(float width, float height) : CubesSteps(width, height)
{
	boxMesh = setBoxMesh(boxW);
	boxFillMesh = setBoxFillMesh(boxW*.97);
}

CubesSteps::~CubesSteps()
{
	
}
CubesStepsLines::~CubesStepsLines()
{

}

void CubesSteps::setup()
{
	bgColor.set(25, 25, 25);
	mainColor.set(240, 240, 240);

	ofBackground(bgColor);
	ofSetSmoothLighting(true);	

	cam.lookAt(ofPoint(0, 0, 0));
	cam.setDistance(300);

	pointLight.setSpotlight(45.f, 0.f);
	pointLight.setAmbientColor(ofColor(0, 0, 0, 255));
	pointLight.setDiffuseColor(ofFloatColor(.85, .85, .85));
	pointLight.setSpecularColor(ofFloatColor(.7f, .7f, .7f));
	pointLight.lookAt(ofPoint(0, 0, 0));

	ambientLight.setAreaLight(1000, 1000);
	//ambientLight.setAmbientColor(255, 255, 255);

	material.setShininess(120);
	material.setSpecularColor(ofColor(255, 255, 255, 255));
	material.setAmbientColor(ofColor(255, 255, 255, 255));
	material.setDiffuseColor(ofColor(255, 255, 255, 255));
	
	setupCount++;
	setupBoxColors();
}

void frames::CubesSteps::setupBoxColors()
{
	int colorIndex;
	if (presetColorsFlag)
	{
		colorIndex = colorPresetID;
	}
	else
	{
		colorIndex = setupCount % boxColorsBase.size();
	}
	
	boxColor1 = boxColorsBase[colorIndex][0];
	boxColor2 = boxColorsBase[colorIndex][1];
	boxColor3 = boxColorsBase[colorIndex][2];
}

void frames::CubesSteps::setBoxColors()
{
	setupCount++;
}

void CubesSteps::GUISetup()
{

#ifndef release
	FrameGeometry::GUISetup();

	gui.add(rotateX.setup("rotateX ", 145, 0., 360.));
	gui.add(rotateY.setup("rotateY ", 0, 0., 360.));
	gui.add(rotateZ.setup("rotateZ ", 45, 0., 360.));

	gui.add(lightX.setup("light x ", 50, -50., 50.));
	gui.add(lightY.setup("light y ", 50, -50., 50.));
	gui.add(lightZ.setup("light z ", 175, -50., 200.));


	gui.add(period.setup("period ", 0.5, 0.00001, 2));
	gui.add(animTime.setup("animTime ", 2.5, 0.00001, 5.));
	gui.add(waveHeight.setup("waveHeight ", 30., 0.0001, 250.));
#else
	rotateX.setup("rotateX ", 145, 0., 360.);
	rotateY.setup("rotateY ", 0, 0., 360.);
	rotateZ.setup("rotateZ ", 45, 0., 360.);

	lightX.setup("light x ", 50, -50., 50.);
	lightY.setup("light y ", 50, -50., 50.);
	lightZ.setup("light z ", 175, -50., 200.);


	period.setup("period ", 0.5, 0.00001, 2);
	animTime.setup("animTime ", 2.5, 0.00001, 5.);
	waveHeight.setup("waveHeight ", 30., 0.0001, 250.);
#endif

}

void CubesSteps::draw()
{
	fboArt.begin();
	ofClear(255, 255, 255, 0);	
	ofEnableDepthTest();
	//ofEnableLighting();

	cam.begin();
	{
		//pointLight.enable();
		//pointLight.setPosition(ofPoint(lightX, lightY, lightZ));
		//pointLight.lookAt(ofPoint(0, 0, 0));

		//ambientLight.enable();

		ofPushMatrix();
		{
			ofRotateX(rotateX);
			ofRotateY(rotateY);
			ofRotateZ(rotateZ);
			for (int y = 0; y < maxBoxW; y++)
			{
				ofPushMatrix();
				{
					ofTranslate(0, boxW*(y - (maxBoxW - 1)*.5), 0);
					for (int x = 0; x < maxBoxW; x++)
					{
						ofPushMatrix();
						{
							ofTranslate(boxW*(x - (maxBoxW - 1)*.5), 0, 0);
							float cx = x - (float)maxBoxW*.5;
							float cy = y - (float)maxBoxH*.5;
							float state;
							if (maxBoxW != 0 && maxBoxW != 0)
							{
								state = ((float)x + (float)y) / ((float)maxBoxW + (float)maxBoxH);
							}
							float sinState = remap(state, -PI*.5, PI*.5);
							float sinMoveZ = remap(moveZ, -PI, PI);
							float offset = sin(  ofClamp( (sinState*period*2*PI + sinMoveZ*2), -PI*.5, PI*.5))*boxW;
							ofTranslate(0, 0, offset- moveZ*boxW*2);
							

							ofPushMatrix();
							{
								drawBox();
							}
							ofPopMatrix();



						}
						ofPopMatrix();
					}//for end
				}
				ofPopMatrix();
			}//dor end

		}
		ofPopMatrix();
		//pointLight.draw();
	}
	cam.end();

	ofDisableDepthTest();
	ofDisableLighting();
	//ofDisableNormalizedTexCoords();

	fboArt.end();

	pointLight.disable();

	//FBO draws in parent
	FrameGeometry::draw();
}

void CubesSteps::update()
{
	timeline->stepTo(ofGetElapsedTimef());
}

float CubesSteps::spaceDiagonal(float size)
{
	return sqrt(3)*size;
}

float CubesSteps::faceDiagonal(float size)
{
	return sqrt(2)*size;
}

void CubesSteps::moveZSetup()
{
	float startDotsScale = 1.;
	float endDotsScale = 0.;
	timeline->apply(&moveZ,
		startDotsScale,
		endDotsScale,
		animTime)
		.finishFn(std::bind(&CubesSteps::moveZFinish, this));
}

void CubesSteps::moveZFinish()
{
	moveZSetup();
}

float CubesSteps::remap(float value, float min, float max)
{	
	return value * (max - min) + min;
}

void frames::CubesSteps::drawBox()
{
	drawColorBox(boxW);
}
void frames::CubesStepsLines::drawBox()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		glDisable(GL_LIGHTING);

		ofSetLineWidth(lineWidth);
		{
			ofSetColor(ofColor(255, 255, 255));
			ofNoFill();
			boxMesh.draw();

			ofSetColor(ofColor(0, 0, 0));
			ofFill();
			boxFillMesh.draw();
		}
		ofSetLineWidth(1);//reset line w

		glEnable(GL_LIGHTING);
	}
	glPopAttrib();
}

ofMesh CubesStepsLines::setBoxMesh(float size)
{
	ofMesh boxMesh;
	ofVec3f vert;
	float s = boxW*0.5;
	boxMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
		//vert.set()
	boxMesh.addVertex(ofVec3f(-s, s, -s));
	boxMesh.addVertex(ofVec3f(-s, -s, -s));
	boxMesh.addVertex(ofVec3f(s, -s, -s));
	boxMesh.addVertex(ofVec3f(s, s, -s));
	boxMesh.addVertex(ofVec3f(-s, s, -s));

	boxMesh.addVertex(ofVec3f(-s, s, s));
	boxMesh.addVertex(ofVec3f(-s, -s, s));
	boxMesh.addVertex(ofVec3f(-s, -s, -s));
	boxMesh.addVertex(ofVec3f(-s, s, -s));

	boxMesh.addVertex(ofVec3f(s, s, -s));
	boxMesh.addVertex(ofVec3f(s, s, s));
	boxMesh.addVertex(ofVec3f(s, -s, s));
	boxMesh.addVertex(ofVec3f(s, -s, -s));
	boxMesh.addVertex(ofVec3f(s, s, -s));
	boxMesh.addVertex(ofVec3f(s, s, s));

	//boxMesh.addVertex(ofVec3f(s, s, s));
	boxMesh.addVertex(ofVec3f(s, -s, s));
	boxMesh.addVertex(ofVec3f(-s, -s, s));
	boxMesh.addVertex(ofVec3f(-s, s, s));
	boxMesh.addVertex(ofVec3f(s, s, s));

	return boxMesh;
}
ofMesh CubesStepsLines::setBoxFillMesh(float size)
{
	ofBoxPrimitive tempBoxPrim;
	tempBoxPrim.set(size);
	return tempBoxPrim.getMesh();
}


void CubesSteps::drawModel(ofMesh& mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.getVerticesPointer()->x);
	glDrawElements(ofGetGLPrimitiveMode(mesh.getMode()), mesh.getNumIndices(), GL_UNSIGNED_INT, mesh.getIndexPointer());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void CubesSteps::drawColorBox(float size)
{
	float offset = size*.5;


	ofPushMatrix();//top
	{
		ofSetColor(boxColor1);
		ofTranslate(0, -offset, 0);
		ofRotate(90, 1, 0, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();
	ofPushMatrix();//bot
	{
		ofSetColor(boxColor1);
		ofTranslate(0, offset, 0);
		ofRotate(90, 1, 0, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();

	ofPushMatrix();//right
	{
		ofSetColor(boxColor2);
		ofTranslate(-offset, 0, 0);
		ofRotate(90, 0, 1, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();
	ofPushMatrix();//left
	{
		ofSetColor(boxColor2);
		ofTranslate(offset, 0, 0);
		ofRotate(90, 0, 1, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();

	ofPushMatrix();//back
	{
		ofSetColor(boxColor3);
		ofTranslate(0, 0, -offset);
		//ofRotate(90, 1, 0, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();
	ofPushMatrix();//front
	{
		ofSetColor(boxColor3);
		ofTranslate(0, 0, offset);
		//ofRotate(90, 1, 0, 0);
		ofDrawPlane(size, size);
	}
	ofPopMatrix();
}

void CubesSteps::setBoxColorsBase()
{
	if (boxColorsBase.empty())
	{
		vector<ofColor> colorSet;

		colorSet.clear();
		colorSet.push_back(ofColor(86, 203, 249));
		colorSet.push_back(ofColor(175, 190, 209));
		colorSet.push_back(ofColor(234, 197, 216));
		boxColorsBase.push_back(colorSet);

		colorSet.clear();
		colorSet.push_back(ofColor(24, 58, 55));
		colorSet.push_back(ofColor(239, 214, 172));
		colorSet.push_back(ofColor(196, 73, 0));
		boxColorsBase.push_back(colorSet);

		colorSet.clear();
		colorSet.push_back(ofColor(219, 22, 47));
		colorSet.push_back(ofColor(219, 223, 172));
		colorSet.push_back(ofColor(95, 117, 142));
		boxColorsBase.push_back(colorSet);

		colorSet.clear();
		colorSet.push_back(ofColor(247, 113, 125));
		colorSet.push_back(ofColor(222, 99, 154));
		colorSet.push_back(ofColor(127, 41, 130));
		boxColorsBase.push_back(colorSet);
	}// end empty if
}