#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class GeometryTemplate> GeometryTemplatePtr;

	class GeometryTemplate : public FrameGeometry
	{
	public:
		GeometryTemplate(float width, float height);
		virtual ~GeometryTemplate();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;
		virtual void draw() override;

	protected:

	private:
	};
}
