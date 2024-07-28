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
#ifdef OLC_PGEX_BACKGROUND_OBJECT

#endif

namespace olc
{
	class BackgroundObject : public PGEX
	{

	public:
		BackgroundObject(); 
		BackgroundObject(std::string ImagePath, bool bIsSpriteSheet = false);
		virtual ~BackgroundObject(); 

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


		struct ObjectProperites
		{
			std::string strName = "New Player";	// Object Name. Default "New Player"
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , BackgroundObject 2 etc

			bool bAutoScale = true;				// Automatically scales the background image to fit within the screen size

			olc::vf2d vfPosition = { 0.0f,0.0f };	// Image POS {x,y} (float), Default {0.0f,0.0f}

			std::string strImagePath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
			
			olc::Renderable renImage;               // Keeps the sprImage and decImage in the one location

			std::vector<olc::Renderable> vecPlayerFrames;   // Holds the different frames for a object

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


#ifdef OLC_PGEX_BACKGROUND_OBJECT
#undef OLC_PGEX_BACKGROUND_OBJECT

namespace olc
{

	// See Step 3: Rename to your Class name
	BackgroundObject::BackgroundObject() : PGEX(true)
	{
		bisSpriteSheet = false;
	}

	BackgroundObject::BackgroundObject(std::string ImagePath, bool bIsSpriteSheet) : PGEX(true)
	{
		bisSpriteSheet = bIsSpriteSheet;
		Properties.strImagePath = ImagePath;
		
	}

	// See Step 3: Rename to your Class name
	BackgroundObject::~BackgroundObject()
	{

		Properties.vecPartialImages.clear();
		Properties.vecPlayerFrames.clear();

	}

	// See Step 3: Rename to your Class name
	void BackgroundObject::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate
		
		// Load up our image
		Properties.renImage.Load(Properties.strImagePath);

	}

	// See Step 3: Rename to your Class name
	void BackgroundObject::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool BackgroundObject::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void BackgroundObject::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}

	// See Step 3: Rename to your Class name
	void BackgroundObject::DrawDecal()
	{
		// As the user can change the image sizes on the fly we need to manage autoscale here
		if (Properties.bAutoScale)
		{
			// ok we need to work out our scaling factor
			// To do this we need to workout by how much we need to increase/decrease our image to fit
			olc::vf2d vfScreenSize = pge->GetScreenSize();
			Properties.sImageInfo.vScale.x = vfScreenSize.x / Properties.sImageInfo.vSize.x;
			Properties.sImageInfo.vScale.y = vfScreenSize.y / Properties.sImageInfo.vSize.y;
			// and thats all folks, the engine will take care of the rest for us
		}


		pge->DrawPartialDecal(Properties.vfPosition,
			Properties.renImage.Decal(),
			Properties.sImageInfo.vSource,
			Properties.sImageInfo.vSize,
			Properties.sImageInfo.vScale,
			Properties.sImageInfo.pxTint);

		

	}

	// See Step 3: Rename to your Class name
	void BackgroundObject::DrawSprite()
	{

		// Sprites do not support AutoScaling.... yet
		pge->DrawPartialSprite(olc::vi2d(Properties.vfPosition), 
			Properties.renImage.Sprite(), 
			Properties.sImageInfo.vSource, 
			Properties.sImageInfo.vSize);
	}



} // olc

#endif
