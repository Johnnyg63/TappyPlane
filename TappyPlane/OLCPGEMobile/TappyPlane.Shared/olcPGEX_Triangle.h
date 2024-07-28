#pragma once

//////////////////////////////////////////////////////////////////
// Pixel Game Engine Mobile Release 2.2.8                       //
// John Galvin aka Johnngy63: 29-May-2024                       //
// iOS Sensor NOT supported, coming soon                        //
// Please report all bugs to https://discord.com/invite/WhwHUMV //
// Or on Github: https://github.com/Johnnyg63					//
//////////////////////////////////////////////////////////////////



/*
* This define is use just so we can select the correct header for Android, iOS and Mac
*/
#if defined (__ANDROID__) || (__APPLE__)

#include "olcPixelGameEngine_Mobile.h"
#else

#include "olcPixelGameEngine.h"
#endif // defined (__ANDROID__) || (__APPLE__)

/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_TRIANGLE_OBJECT

#endif

namespace olc
{
	class TriangleObject : public PGEX
	{

	public:
		TriangleObject();
		TriangleObject(std::string ImagePath, bool bIsSpriteSheet = false);
		virtual ~TriangleObject();

		// Fires just before the main OnUserCreate
		virtual void OnBeforeUserCreate() override;

		// Fires just After the main OnUserCreate
		virtual void OnAfterUserCreate() override;

		// Fires just before the main OnUserUpdate
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) override;

		// Fires just After the main OnUserUpdate
		virtual void OnAfterUserUpdate(float fElapsedTime) override;

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created decal
		void DrawDecal();

		// Call this method from the onUserUpdate of Main.cpp, or anywhere, to draw the created sprite
		void DrawSprite();

		// Add your own public methods here


	public:

		/*
		* Stores data required for the background image(s) to display correctly
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f }; // Source poisition to draw drawing from, default: {0.0f, 0.0f)
			olc::vf2d vSize = { 1.0f, 1.0f };	// The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
			olc::vf2d vScale = { 1.0f, 1.0f };	// Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
			olc::Pixel pxTint = olc::WHITE;		// Tint colour for background, set to olc::DARK_GREY for a night time effect
		};

		enum IMAGESCALE
		{
			NONE = 0,		// Do not scale the image
			AUTO_SCALE,		// Automatically scales the background image to fit within the screen size
			SCALE_WIDTH,	// Automatically scales image Width to fit within the screen size
			SCALE_HEIGHT	// Automatically scales image Height to fit within the screen size


		};

		struct ObjectProperites
		{
			std::string strName = "New Player";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , TriangleObject 2 etc

			IMAGESCALE SCALE = IMAGESCALE::AUTO_SCALE;	// Image Scaler, Default AUTO_SCALE
			olc::vf2d vfPosition = { 0.0f,0.0f };		// Image POS {x,y} (float), Default {0.0f,0.0f}

			std::string strImagePath = "";				// Sprite path, i.e. "images/mysprite.png", Default: ""

			olc::Renderable renImage;					// Keeps the sprImage and decImage in the one location

			std::vector<olc::Renderable> vecPlayerFrames;   // Holds the different frames for a object

			float fFrameChangeRate = 0.0f;  // This value will auto calcuate the rate of change depending on the current FPS
			float fFrameElapsedTime = 1.0f; // Keeps track of the time pass since the last frame change
			float fFramesPerSecound = 1.0f; // Set the number of times the frame is to change per second 

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			/*
			* Holds the vSource and vSize of the image
			* Edit this value to the location of the Sprite when using a Sprite sheet
			*/
			ImageInfo sImageInfo;

			/*
			*
			* Stores the location of the partial image from the sprSpriteSheet
			* ImageInfo.vSource {x,y} of the top left of the partial image to draw
			* ImageInfo.vSize {w, h} size of the partial image to be drawn
			*
			*/
			std::vector<ImageInfo> vecPartialImages;



		};

		ObjectProperites Properties;

	private:
		bool bisSpriteSheet = false; // Flag for sprite sheet or 1 image

	};

}


#ifdef OLC_PGEX_TRIANGLE_OBJECT
#undef OLC_PGEX_TRIANGLE_OBJECT

namespace olc
{

	// See Step 3: Rename to your Class name
	TriangleObject::TriangleObject() : PGEX(true)
	{
		bisSpriteSheet = false;
	}

	TriangleObject::TriangleObject(std::string ImagePath, bool bIsSpriteSheet) : PGEX(true)
	{
		bisSpriteSheet = bIsSpriteSheet;
		Properties.strImagePath = ImagePath;

	}

	// See Step 3: Rename to your Class name
	TriangleObject::~TriangleObject()
	{

		Properties.vecPartialImages.clear();
		Properties.vecPlayerFrames.clear();

	}

	// See Step 3: Rename to your Class name
	void TriangleObject::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

		// Load up our image
		Properties.renImage.Load(Properties.strImagePath);

	}

	// See Step 3: Rename to your Class name
	void TriangleObject::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool TriangleObject::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate

		// Ok to keep everything in seq, we will use frames per second as our game speed
		// in this way we can increase and decrease the speed of the game and all objects will react 
		// the same.
		// We can also cause delays, i.e. we can set -4.0f and put in a 5 second count down timer
		// I think I will do this lol
		Properties.fFrameChangeRate = 1.0f / Properties.fFramesPerSecound;
		Properties.fFrameElapsedTime += fElapsedTime;

		if (Properties.fFrameElapsedTime > Properties.fFrameChangeRate)
		{
			Properties.vfPosition.x -= Properties.vfVelocity.x * fElapsedTime;
		}




		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void TriangleObject::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}

	// See Step 3: Rename to your Class name
	void TriangleObject::DrawDecal()
	{
		// As the user can change the image sizes on the fly we need to manage autoscale here
		// ok we need to work out our scaling factor
		// To do this we need to workout by how much we need to increase/decrease our image to fit

		switch (Properties.SCALE)
		{
		case AUTO_SCALE:
		{
			Properties.sImageInfo.vScale.x = pge->GetScreenSize().x / Properties.sImageInfo.vSize.x;
			Properties.sImageInfo.vScale.y = pge->GetScreenSize().y / Properties.sImageInfo.vSize.y;
			break;
		}
		case SCALE_WIDTH:
		{
			Properties.sImageInfo.vScale.x = pge->GetScreenSize().x / Properties.sImageInfo.vSize.x;
			break;
		}
		case SCALE_HEIGHT:
		{
			Properties.sImageInfo.vScale.y = pge->GetScreenSize().y / Properties.sImageInfo.vSize.y;
			break;
		}
		case NONE:
		default:
			break;
		}

		// Ok we what the ground to scroll to the left, for it to be continous 
		// Therefore we draw the image twice, with the secound image depending on the first image position
		// once the first image is off the screen we reset

		olc::vf2d newPos = { 0.0f, 0.0f }; // Set our second image position to nothing

		// Ok lets check do we need to reset?
		if ((Properties.vfPosition.x + pge->ScreenWidth()) < 0)
		{
			// ok we are off the screen lets reset
			Properties.vfPosition.x = 0.0f;
		}

		newPos.y = Properties.vfPosition.y;
		newPos.x = pge->ScreenWidth() + Properties.vfPosition.x;

		pge->FillTriangle(olc::vi2d(Properties.vfPosition), 
			{ int32_t(Properties.vfPosition.x - 100.0f), 
			int32_t(Properties.vfPosition.y + 100.0f) },
			{ int32_t(Properties.vfPosition.x + 100.0f), 
			int32_t(Properties.vfPosition.y + 100.0f) }, olc::RED
		);

		//// First image
		//pge->DrawPartialDecal(Properties.vfPosition,
		//	Properties.renImage.Decal(),
		//	Properties.sImageInfo.vSource,
		//	Properties.sImageInfo.vSize,
		//	Properties.sImageInfo.vScale,
		//	Properties.sImageInfo.pxTint);

		//// Secound image
		//pge->DrawPartialDecal(newPos,
		//	Properties.renImage.Decal(),
		//	Properties.sImageInfo.vSource,
		//	Properties.sImageInfo.vSize,
		//	Properties.sImageInfo.vScale,
		//	Properties.sImageInfo.pxTint);
	}



} // olc

#endif
