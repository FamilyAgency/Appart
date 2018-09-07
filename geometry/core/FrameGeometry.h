#pragma once
#include "ArtDrawable.h"
#include "ofxGui.h"

namespace frames
{
	class FrameGeometry : public ArtDrawable
	{
	public:
		FrameGeometry(float width, float height);
		virtual ~FrameGeometry();

		virtual void  setup() override;
		virtual void  draw() override;
		virtual void  update() override;
		virtual void  clean() override;
		virtual void  GUISetup() override;	

	protected:
		ofxPanel gui;
		virtual void resolutionChangedListener(const ofVec2f& resolution) override;
	};
}