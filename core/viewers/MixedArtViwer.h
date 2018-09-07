#pragma once
#include "AbstractViewer.h"
#include "../viewers/ThreeArtsViewer.h"
#include "../viewers/OneLargeArtwork.h"
#include "../config/Config.h"

namespace frames
{
	typedef shared_ptr<class MixedArtViwer> MixedArtViwerPtr;

	class MixedArtViwer : public AbstractViewer
	{
		synapse::Config::ConfigObject currentRotator;
		bool faderAnimated = false;


		int index = 0;
		void changeArt();

		void fadeinCompleteHandler();
		void fadeOutCompleteHandler();
		void timeRotatorCheck();

	public:		
		virtual void setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config) override;
		virtual void update()							override;
		virtual void draw()								override;
		virtual void clean()							override;
		virtual void setArtRotator(int seconds)			override;
		virtual void nextArt()							override;
		virtual void setOwnResolution()					override;

		virtual ofTexture getTexture() const			override;		
		
		frames::AbstractViewerPtr viewerThreeArt, viewerLarge, viewerCurrent;
	};
}
