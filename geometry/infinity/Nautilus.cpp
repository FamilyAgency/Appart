#include "Nautilus.h"

using namespace frames;

Nautilus::Nautilus(
	float width,
	float height,
	ofColor mainColor,
	ofColor fillColor,
	ofColor bgColor,
	float camDist,
	float longt,
	float latit,
	float maxRotZ,
	float maxRotX
) : FrameGeometry(width, height),
	bgColor(bgColor),
	mainColor(mainColor),
	fillColor(fillColor),
	mainColorGL(mainColor),
	fillColorGL(fillColor),
	camDist(camDist),
	maxRotZ(maxRotZ),
	maxRotX(maxRotX)
{	
	spoutChannelName = "Geometry Art";
	name = "Nautilus";

	cam.setDistance(camDist);
	cam.orbit(longt, latit, camDist, ofPoint(0, 0, 0));

	timeline = ofxCinderTimeline::Timeline::create();
	timeline->stepTo(ofGetElapsedTimef());

	sphere.setResolution(sphereResolutin);
	sphere.setRadius(radius);
	sphereRingMesh.setMode(OF_PRIMITIVE_LINE_LOOP);

	sphereMesh = sphere.getMesh();
	vector<ofVec3f>& verts = sphereMesh.getVertices();

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		if (verts[i].y > 0)
		{
			verts[i].y = verts[i].y*-1;
			if (verts[i].y > -10)//TODO:const
			{
				sphereRingMesh.addVertex(verts[i]);
			}
		}
	}
}

Nautilus::~Nautilus()
{

}

void Nautilus::setup()
{
	ofBackground(bgColor);	
	timeline->stepTo(ofGetElapsedTimef());
	animSetup();	
}

void Nautilus::GUISetup()
{

#ifndef release
	FrameGeometry::GUISetup();
	gui.add(camDist.setup("camDist", 300, 0, 750.));
#else
	camDist.setup("camDist", 300, 0, 750.);
#endif

}

void Nautilus::draw()
{	
	// draw in FBO
	fboArt.begin();

	ofClear(255, 255, 255, 0);
	ofBackground(bgColor);

	ofEnableDepthTest();
	cam.begin();
	{
		for (int i = 1; i < spheresNum; i++)
		{
			ofPushMatrix();
			{
				//TODO: cast to float in division 1 time
				float rotationZ = sin(((float)i / (float)spheresNum) * 2 + anim * 2 * 3.14159265359)*maxRotZ;
				ofRotateZ(rotationZ);
				float rotationX = sin(((float)i / (float)spheresNum) * 5 + anim * 2 * 3.14159265359)*maxRotX;
				ofRotateX(rotationX);
				float scale = ((float)i / (float)spheresNum);
				ofScale(scale, scale, scale);

				glPushAttrib(GL_ALL_ATTRIB_BITS);
				{
					glDisable(GL_LIGHTING);

					glClearStencil(0);
					glClear(GL_STENCIL_BUFFER_BIT);
					glEnable(GL_STENCIL_TEST);

					glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glColor3f(fillColorGL.r, fillColorGL.b, fillColorGL.g);
					drawModel(sphereMesh);//fill
						
					glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF);
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glLineWidth(lineWidth);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glColor3f(mainColorGL.r, mainColorGL.g, mainColorGL.b);
					drawModel(sphereMesh);//outline

					glEnable(GL_LIGHTING);
					glDisable(GL_STENCIL_TEST);
				}
				glPopAttrib();

				ofSetLineWidth(lineWidth);
				{
					ofSetColor(mainColor);
					sphereRingMesh.draw();
				}
				ofSetLineWidth(1);//reset line w
			}//matrix end
			ofPopMatrix();
		}//for end
	}
	cam.end();

	ofDisableDepthTest();
	ofDisableDepthTest();
	ofDisableLighting();
	ofDisableNormalizedTexCoords();
	ofEnableArbTex();

	fboArt.end();
	//FBO draws in parent
	FrameGeometry::draw();		
}

void Nautilus::update()
{
	timeline->stepTo(ofGetElapsedTimef());
}

void Nautilus::animSetup()
{
	float start = 1.;
	float end = 0.;
	timeline->apply(&anim,
		start,
		end,
		animTime)
		.finishFn(std::bind(&Nautilus::animFinish, this));
}

void Nautilus::animFinish()
{
	animSetup();
}

void Nautilus::drawModel(ofMesh& mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.getVerticesPointer()->x);
	glDrawElements(ofGetGLPrimitiveMode(mesh.getMode()), mesh.getNumIndices(), GL_UNSIGNED_INT, mesh.getIndexPointer()); 
	glDisableClientState(GL_VERTEX_ARRAY);
}