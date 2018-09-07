#include "CubesRing.h"

using namespace frames;

CubesRing::CubesRing(float width, float height) : FrameGeometry(width, height)
{
	spoutChannelName = "Geometry Art";
	name = "Cubes Ring";

	background.set(60, 60, 60);
	mainColor.set(220, 220, 220);
	lightDiff.set(255, 255, 255);
	lightSpec.set(255, 255, 255);
	orbitColorDiff.set(255, 0, 0);
	orbitColorSpec.set(255, 255,255);
	ofVec3f nullVec(0, 0, 0);
	ofVec3f upVec(0, 0, 1);
}

CubesRing::~CubesRing()
{
	mesh.clear();
	//center.clear();
}

void CubesRing::setup()
{
	//ofSetVerticalSync(true);
	ofBackground(background);
	ofEnableDepthTest();

	cam.setAutoDistance(true);

	ofSetSmoothLighting(true);//true
	ofEnableLighting();

	pointLight.setPointLight();
	pointLight.setDiffuseColor(lightDiff);
	pointLight.setSpecularColor(lightSpec);
	pointLight.enable();

	orbitLight.setPointLight();
	orbitLight.setDiffuseColor(orbitColorDiff);
	orbitLight.setSpecularColor(orbitColorSpec);
	orbitLight.enable();

	
	//pointLight.setGlobalPosition(cam.getGlobalPosition());
	//pointLight.setGlobalOrientation(cam.getGlobalOrientation());
	

	material.setShininess(255);
	material.setSpecularColor(ofColor(255, 255, 255, 255));

	//offsetRotate = 0;
	//cam.setPosition(0, 0, 200);


	mesh.clear();
	center.setPosition(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		corners[i].setParent(center);
	}
	corners[0].setPosition(-25, 0, -25);
	corners[1].setPosition(-25, 0, 25);
	corners[2].setPosition(25, 0, 25);
	corners[3].setPosition(25, 0, -25);
	mesh.setMode(OF_PRIMITIVE_TRIANGLES); //OF_PRIMITIVE_LINE_LOOP

	center.rotate(90, 0, 1, 0);

	innerRotate = 0;
	
}

void CubesRing::GUISetup()
{
	FrameGeometry::GUISetup();
	gui.add(rotation.setup("rotation ", 0., 0., 360.));
	gui.add(wireframe.setup("wireframe", true));
}

void CubesRing::draw()
{	
	fboArt.begin();
	ofClear(255, 255, 255, 0);

	mesh.clear();

	ofEnableDepthTest();
	ofEnableLighting();
	ofEnableNormalizedTexCoords();

	for (int ci = 0; ci < numCubes; ci++)
	{
		center.setPosition(ci * 40 + 10, 0, 0);
		center.orbit(90, ((360 / numCubes)*(float)ci), 50, nullVec);
		float offSetRotate = sin( ((float)ci/numCubes)*2*PI )*offSetRotateMax;

		float meshRotate = (totalRotate / numCubes)*ci;
		center.rotate(meshRotate + offSetRotate + innerRotate, center.getYAxis());

		//center.draw();
		for (int i = 0; i < 4; i++)
		{
			//corners[i].draw();
			ofPoint pt = corners[i].getGlobalPosition();
			mesh.addVertex(pt);
			mesh.addColor(mainColor);
		}
		int nv = mesh.getNumVertices() - 1;
		if (mesh.getNumVertices() >= 8 )//&& mesh.getNumVertices()<=1200)
		{
			int nv = mesh.getNumVertices() - 1;
			mesh.addTriangle(nv - 5, nv - 1, nv - 4);
			mesh.addTriangle(nv - 0, nv - 4, nv - 1);
			mesh.addTriangle(nv - 3, nv - 7, nv - 4);
			mesh.addTriangle(nv - 0, nv - 3, nv - 4);

			mesh.addTriangle(nv - 6, nv - 7, nv - 2);
			mesh.addTriangle(nv - 2, nv - 7, nv - 3);
			mesh.addTriangle(nv - 6, nv - 2, nv - 5);
			mesh.addTriangle(nv - 5, nv - 2, nv - 1);

			//          saved version 
			//mesh.addTriangle(nv - 1, nv - 5, nv - 4);
			//mesh.addTriangle(nv - 4, nv - 0, nv - 1);
			//mesh.addTriangle(nv - 7, nv - 3, nv - 4);
			//mesh.addTriangle(nv - 3, nv - 0, nv - 4);

			//mesh.addTriangle(nv - 7, nv - 6, nv - 2);
			//mesh.addTriangle(nv - 7, nv - 2, nv - 3);
			//mesh.addTriangle(nv - 2, nv - 6, nv - 5);
			//mesh.addTriangle(nv - 2, nv - 5, nv - 1);
			
		}
		//cout << mesh.getNumVertices() << endl;
	}
	/*if (mesh.getNumVertices() >= 8)
	{
		if (totalRotate == 0)
		{
			int nv = mesh.getNumVertices() - 1;
			mesh.addTriangle(0, 1, nv - 3);
			mesh.addTriangle(1, nv-2, nv - 3);
			mesh.addTriangle(0, nv - 2, nv - 3);
			mesh.addTriangle(1, nv - 2, 2);
			mesh.addTriangle(nv - 2, 2, nv - 1);
			mesh.addTriangle(nv - 1, 2, 3);
			mesh.addTriangle(3, nv - 0, nv - 1);
			mesh.addTriangle(0, nv - 3, 3);
			mesh.addTriangle(nv - 3, nv - 0, 3);
		}
		if (totalRotate == 360)
		{
			int nv = mesh.getNumVertices() - 1;
			mesh.addTriangle(nv - 2, nv - 3, 1);
			mesh.addTriangle(0, nv - 3, 1);
			mesh.addTriangle(nv - 2, nv - 1, 1);
			mesh.addTriangle(nv - 1, 2, 1);
			mesh.addTriangle(nv - 1, 2, 3);
			mesh.addTriangle(nv - 1, 0, 3);
			mesh.addTriangle(nv - 3, nv, 0);
			mesh.addTriangle(3, nv, 0);
		}
		if (totalRotate == 180)
		{
			int nv = mesh.getNumVertices() - 1;
			mesh.addTriangle(0, 1, nv - 1);
			mesh.addTriangle(nv, 1, nv - 1);
			mesh.addTriangle(nv, 1, nv - 3);
			mesh.addTriangle(2, 1, nv - 3);
			mesh.addTriangle(2, 3, nv - 3);
			mesh.addTriangle(nv - 2, 3, nv - 3);
			mesh.addTriangle(0, 3, nv - 1);
			mesh.addTriangle(0, nv - 2, nv - 1);
		}
	}*/
	cam.begin();
	{
		ofBackground(background);
		mesh.enableNormals();
		calcNormals(mesh);
		mesh.draw();
		orbitLight.draw();
		pointLight.draw();
		ofDisableLighting();
		{
			if (wireframe)
			{
				mesh.drawWireframe();
			}
			ofSetColor(0, 0, 255);
			vector<ofMeshFace> faces = mesh.getUniqueFaces();
			ofMeshFace face;
			ofVec3f c, n;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				face = faces[i];
				c = calculateCenter(&face);
				n = face.getFaceNormal();
				//ofLine(c.x, c.y, c.z, c.x + n.x*10, c.y + n.y*10, c.z + n.z*10);
			}
		}
		ofEnableLighting();
	}
	cam.end();

	ofDisableDepthTest();
	ofDisableLighting();
	ofDisableNormalizedTexCoords();
	ofEnableArbTex();

	fboArt.end();
	FrameGeometry::draw();		
}

void CubesRing::update()
{
	ofPoint center(0, 0, 0);
	orbitLight.orbit(0., ofGetElapsedTimef() * 30, 200, center);
	orbitLight.lookAt(center);

	offsetRotate = offsetRotate + 1.1;
	pointLight.setPosition((ofGetWidth()*.5) + cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight() / 2, 500);
}
ofPoint CubesRing::pointFromAngle(const float& angle, const float& d)
{
	float a = ofDegToRad(angle);
	ofPoint point;
	point.set(d*cos(a), d*sin(a));
	return point;
}

void CubesRing::calcNormals(ofMesh &mesh)
{
	for (int i = 0; i < mesh.getVertices().size(); i++) mesh.addNormal(ofPoint(0, 0, 0));

	for (int i = 0; i < mesh.getIndices().size(); i += 3)
	{
		const int ia = mesh.getIndices()[i];
		const int ib = mesh.getIndices()[i + 1];
		const int ic = mesh.getIndices()[i + 2];

		ofVec3f e1 = mesh.getVertices()[ia] - mesh.getVertices()[ib];
		//e1 = e1 *-1;
		ofVec3f e2 = mesh.getVertices()[ic] - mesh.getVertices()[ib];
		//e2 = e1 *-1;
		ofVec3f no = e2.cross(e1);

		// depending on your clockwise / winding order, you might want to reverse the e2 / e1 above if your normals are flipped. 

		mesh.getNormals()[ia] += no;
		mesh.getNormals()[ib] += no;
		mesh.getNormals()[ic] += no;
	}
}


void CubesRing::drawNormals(ofMesh mesh)
{
	float length = 50;
	//float length, bool bFaceNormals
	//ofNode::transformGL(ofGetCurrentRenderer().get());

	const vector<ofVec3f>& normals = mesh.getNormals();
	const vector<ofVec3f>& vertices = mesh.getVertices();
	ofVec3f normal;
	ofVec3f vert;

	normalsMesh.setMode(OF_PRIMITIVE_LINES);
	normalsMesh.getVertices().resize(normals.size() * 2);
	//normalsMesh.clear();

	for (int i = 0; i < (int)normals.size(); i++)
	{
		if (i % 3 == 0)
		{
			vert = (vertices[i] + vertices[i + 1] + vertices[i + 2]) / 3;
		}
		else if (i % 3 == 1)
		{
			vert = (vertices[i - 1] + vertices[i] + vertices[i + 1]) / 3;
		}
		else if (i % 3 == 2)
		{
			vert = (vertices[i - 2] + vertices[i - 1] + vertices[i]) / 3;
		}
		normalsMesh.setVertex(i * 2, vert);
		normal = normals[i].getNormalized();
		normal *= length;
		normalsMesh.setVertex(i * 2 + 1, normal + vert);
	}


	normalsMesh.draw();
}

ofVec3f CubesRing::calculateCenter(ofMeshFace *face)
{
	int lastPointIndex = 3;
	ofVec3f result;
	for (unsigned int i = 0; i < 3; i++)
	{
		result += face->getVertex(i);
	}
	result /= lastPointIndex;
	return result;
}