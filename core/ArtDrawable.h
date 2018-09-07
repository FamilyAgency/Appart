#pragma once
#include "ofMain.h"
#include "Config.h"

namespace frames
{
	typedef shared_ptr<class ArtDrawable> ArtDrawablePtr;

	class ArtDrawable
	{
	public:
		ArtDrawable(int width, int height);
		virtual ~ArtDrawable();
		virtual void draw();
		virtual void initFbo(int samples);

		virtual void	setup()  = 0;
		virtual void	update() = 0;	
		virtual void	forceUpdate() {};

		virtual void	clean() = 0;
		virtual void	setResolution(int width, int height);
		virtual void	GUISetup() = 0;
		virtual void	GUIvisible(bool value);

		virtual void	setWidth(int width);
		virtual void	setHeight(int height);

		virtual int		getWidth() const;
		virtual int		getHeight() const;

		virtual ofTexture getTexture() const;
		string getSpoutChannelName() const;
		string getName() const;

		ofEvent<FRAME_TYPES> showMeForce;
		FRAME_TYPES type;

		bool isForceUpdate() const
		{
			return needForceUpdateFeature;
		};

		void setType(FRAME_TYPES type)
		{
			this->type = type;
		};

		FRAME_TYPES getType() const
		{
			return type;
		}

	protected:
		string spoutChannelName;
		string name;
		ofFbo  fboArt;
		bool   guiVisible;
		int    width, height;
		bool   needForceUpdateFeature = false;
		int fboArtSamples = 2;

		virtual void resolutionChangedListener(const ofVec2f& resolution);

		virtual void showMeForceNotifyCall() 
		{
			ofNotifyEvent(showMeForce, type);
		};
	};
}
