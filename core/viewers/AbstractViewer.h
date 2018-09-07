#pragma once

#include "ofMain.h"
#include "Types.h"
#include "ArtDrawable.h"
#include "MainArtCreator.h"
#include "../../config/Config.h"

namespace frames
{
	typedef shared_ptr<class AbstractViewer> AbstractViewerPtr;

	class AbstractViewer
	{
	public:
		virtual void	setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config);

		virtual void	update()	= 0;
		virtual void	draw()		= 0;
		virtual void	clean()		= 0;	

		virtual ofTexture getTexture() const			 = 0;
		virtual void setArtRotator(int seconds)			 = 0;
		virtual void nextArt()							 = 0;
		virtual void setOwnResolution() = 0;

		virtual void timeRotatorCheck();
		virtual string getSpoutChannelName() const;		

		virtual void setForceArt(FRAME_TYPES type) {};

	protected:
		frames::ArtDrawablePtr art;
		frames::FRAME_TYPES currentArtType;	
		synapse::ConfigPtr config;

		map<frames::FRAME_TYPES, frames::ArtDrawablePtr> artMap;
		vector<frames::ArtDrawablePtr> artVector;

		int currentArtNumber;
		float curentSaveTime = 0.0f;		
		string spoutChannelName;

		ofRectangle faderRectangle;
		ofxCinderTimeline::Anim<float> faderAlpha;
		shared_ptr<ofxCinderTimeline::Timeline> mTimeline;
		bool faderAnimated;
		float secondsArtShow;

		virtual void drawFader();
		void fadeinCompleteHandler();
		void fadeOutCompleteHandler();		

		void setSmallFader();
		void setLargeFader();
		void setFader(int w, int h);
	};
}
