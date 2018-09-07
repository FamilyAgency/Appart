#pragma once
#include "AbstractSingleArtViewer.h"
#include "../config/Config.h"

namespace frames
{
	typedef shared_ptr<class OneArtViewer> OneArtViewerPtr;

	class OneArtViewer: public AbstractSingleArtViewer
	{
	public:	
		virtual void setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config);
		virtual void setOwnResolution() override;

	};
}
