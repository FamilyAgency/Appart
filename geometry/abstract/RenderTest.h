#pragma once
#include "FrameGeometry.h"
//#include ""
#include "../lib/gBuffer.h"
#include "../lib/ssaoPass.h"
#include "../lib/pointLight.h"
#include "../lib/primitives.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class RenderTest> RenderTestPtr;

	class RenderTest : public FrameGeometry
	{
	public:
		RenderTest(float width, float height);
		virtual ~RenderTest();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:

	private:
		struct Box
		{
			ofVec3f pos;
			float size;
			float angle;
			float axis_x;
			float axis_y;
			float axis_z;

			Box(ofVec3f pos = ofVec3f(0.0f, 0.0f, 0.0f), float angle = 0.0f, float ax = 0.0f, float ay = 0.0f, float az = 0.0f, float size = 2.0f) :
				pos(pos),
				size(size),
				angle(angle),
				axis_x(ax),
				axis_y(ay),
				axis_z(az)
			{
			}
		};

		enum TEXTURE_UNITS
		{
			TEX_UNIT_ALBEDO,
			TEX_UNIT_NORMALS_DEPTH,
			TEX_UNIT_SSAO,
			TEX_UNIT_POINTLIGHT_PASS,
			TEX_UNIT_NUM_UNITS
		};

		const int skNumBoxes = 50;
		const int skNumLights = 100;//will be deleted
		const int skRadius = 20; // will be deleted
		const int skMaxPointLightRadius = 8;

		//void setupModel();
		void setupLights();
		void setupScreenQuad();
		//void setupPointLightPassFbo();
		void resizeBuffersAndTextures();

		void addRandomLight();
		void createRandomBoxes();
		void randomizeLightColors();

		void unbindGBufferTextures();
		void bindGBufferTextures();

		void drawScreenQuad();

		void geometryPass();
		void pointLightStencilPass();
		void pointLightPass();
		void deferredRender();

		GBuffer m_gBuffer;
		SSAOPass m_ssaoPass;

		ofVbo m_quadVbo;

		ofShader m_shader;
		ofShader m_pointLightPassShader;
		ofShader m_pointLightStencilShader;

		ofEasyCam m_cam;

		ofImage m_texture;

		GLuint m_textureUnits[TEX_UNIT_NUM_UNITS];

		ofVbo  m_sphereVbo;
		int    m_numSphereVerts;

		ofVbo  m_boxVbo;
		int    m_numBoxVerts;

		float   m_angle;

		bool    m_bDrawDebug;
		bool    m_bPulseLights;

		int     m_windowWidth;
		int     m_windowHeight;

		vector<Box> m_boxes;
		vector<PointLight> m_lights;



	};
}
