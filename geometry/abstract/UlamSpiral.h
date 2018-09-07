#pragma once
#include "FrameGeometry.h"

namespace frames
{
	// short name for smart pointer
	typedef shared_ptr<class UlamSpiral> UlamSpiralPtr;

	class UlamSpiral : public FrameGeometry
	{
	public:
		UlamSpiral(float width, float height);
		virtual ~UlamSpiral();

		virtual void setup()	override;
		virtual void GUISetup() override;
		virtual void update()	override;
		virtual void draw()		override;

	protected:

	private:	
		ofxColorSlider bgColor;
		ofxIntSlider size;

	};
}
