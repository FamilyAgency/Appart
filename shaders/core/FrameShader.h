#pragma once

#include "AbstractShader.h"
#include "ArtDrawable.h"
#include "ofxGui.h"

namespace frames
{
	class FrameShader : public AbstractShader, public ArtDrawable
	{
	public:
		FrameShader(float width, float height);
		virtual ~FrameShader();

		virtual void  draw()	 override;
		virtual void  update()   override;
		virtual void  clean()    override;
		virtual void  GUISetup() override;

	protected:
		ofxPanel gui;
		ofFbo	 fboMask;
		float    startTime = 0.0f;
		virtual void  resolutionChangedListener(const ofVec2f& resolution) override;		
	
	};
}