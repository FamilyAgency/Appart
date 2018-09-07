#pragma once
#include "AbstractViewer.h"

namespace frames
{
	typedef shared_ptr<class AbstractSingleArtViewer> AbstractSingleArtViewerPtr;

	class AbstractSingleArtViewer : public AbstractViewer
	{
	public:
		virtual void setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config) override;
		virtual void update()							override;
		virtual void draw()								override;
		virtual void clean()							override;
		virtual void setArtRotator(int seconds)			override;
		virtual void nextArt()							override;

		virtual ofTexture getTexture() const			override;

		virtual void switchArt(int value);

		virtual void setForceArt(FRAME_TYPES type)		override;
	};
}
