#pragma once
#include "AbstractViewer.h"

namespace frames
{
	typedef shared_ptr<class ThreeArtsViewer> ThreeArtsViewerPtr;

	struct TripleArt
	{
		frames::ArtDrawablePtr art1;
		frames::ArtDrawablePtr art2;
		frames::ArtDrawablePtr art3;

		void setup()
		{
			art1->setup();
			art2->setup();
			art3->setup();
		}

		void update()
		{
			art1->update();
			art2->update();
			art3->update();
		}	

		void clean()
		{
			if (art1)
			{
				art1->clean();
			}

			if (art2)
			{
				art2->clean();
			}

			if (art3)
			{
				art3->clean();
			}
		}

		void GUIvisible(bool value)
		{
			art1->GUIvisible(value);
			art2->GUIvisible(value);
			art3->GUIvisible(value);
		}
	};

	class ThreeArtsViewer : public AbstractViewer
	{
	public:
		ThreeArtsViewer();
	
		virtual void setup(MainArtCreatorPtr artCreator, synapse::ConfigPtr config)	override;
		virtual void update()								override;
		virtual void draw()									override;
		virtual void clean()								override;
		virtual void setOwnResolution()						override;

		virtual void setArtRotator(int seconds)  override;
		virtual void nextArt()					 override;
		virtual ofTexture getTexture() const	 override;

		virtual void switchArt();
	
	private:
		int currentArtIndex = 0;
		float oneWidth;
		ofFbo bigFBO;

		TripleArt currentTripleArt;
		vector<TripleArt> tripleArts;

		void mapThreeArtsFrames();		
	};	
}
