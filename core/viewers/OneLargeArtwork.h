#pragma once
#include "AbstractSingleArtViewer.h"

namespace frames
{
	typedef shared_ptr<class OneLargeArtwork> OneLargeArtworkPtr;

	class OneLargeArtwork : public AbstractSingleArtViewer
	{
	public:
		virtual void setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config);
		virtual void setOwnResolution() override;
	};
}
