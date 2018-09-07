#pragma once
#include "FrameShader.h"

namespace frames
{
	typedef shared_ptr<class KaleidoscopeTunnel> KaleidoscopeTunnelPtr;

	class KaleidoscopeTunnel : public FrameShader
	{
	public:
		KaleidoscopeTunnel(float width, float height);
		virtual ~KaleidoscopeTunnel();

		virtual void setup() override;
		virtual void GUISetup() override;
		virtual void update() override;

	protected:
		virtual string getFragSrc();

	private:
		ofImage texture;
	};
}
