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
#ifdef OLC_PGEX_ROCK_OBJECT

#endif

namespace olc
{
	class RockObject : public PGEX
	{

	public:
		RockObject();
		RockObject(std::string ImagePath, bool bIsSpriteSheet = false);
		virtual ~RockObject();

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

		// Add your own public methods here


	public:

		/*
		* Stores data required for the background image(s) to display correctly
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f }; // Source poisition to draw drawing from, default: {0.0f, 0.0f)
			olc::vf2d vSize = { 1.0f, 1.0f };	// The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
			olc::vf2d vScaleSize = { 1.0f, 1.0f }; // Auto generated, stores the scale size of the image in pixels
			olc::vf2d vScale = { 1.0f, 1.0f };	// Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
			olc::Pixel pxTint = olc::WHITE;		// Tint colour for background, set to olc::DARK_GREY for a night time effect
		};

		/*
		* Stores the triangle needed for collision dection 
		*/
		struct Triangle
		{
			olc::vf2d vfPosition1 = { 0.0f, 0.0f };
			olc::vf2d vfPosition2 = { 0.0f, 0.0f };
			olc::vf2d vfPosition3 = { 0.0f, 0.0f };
			int8_t nRockNumber = 0;
			olc::vf2d vfCurrentPosition = { 0.0f, 0.0f };
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
			int8_t nObjectNumber = 0;	        // Object Number, Default 0 i.e. Backupground 1 , RockObject 2 etc

			IMAGESCALE SCALE = IMAGESCALE::AUTO_SCALE;	// Image Scaler, Default AUTO_SCALE
			
			bool bRockTop = false;						// Flag for if a Top rock or buttom 
			//olc::vf2d vfPosition = { 0.0f,0.0f };		// Image POS {x,y} (float), Auto generated
			std::string strImagePath = "";				// Sprite path, i.e. "images/mysprite.png", Default: ""

			olc::Renderable renImage;					// Keeps the sprImage and decImage in the one location

			std::vector<olc::Renderable> vecPlayerFrames;   // Holds the different frames for a object

			float fFrameChangeRate = 0.0f;  // This value will auto calcuate the rate of change depending on the current FPS
			float fFrameElapsedTime = 1.0f; // Keeps track of the time pass since the last frame change
			float fFramesPerSecound = 1.0f; // Set the number of times the frame is to change per second 

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			olc::vf2d vfPeekOffSet = { 0.0f, 0.0f }; // Stores the peak of the rock offset for collision control

			int8_t nRockCount = 1;					// Stores the number of Rocks to display on the screen; Default: 1
			

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

		/*
		* Collision Triangle
		*/
		Triangle collTriangle;

		/*
		* Stores the Collision Triaangles structs
		*/
		std::vector<Triangle> vecCollisionTriangles;

	private:
		bool bisSpriteSheet = false; // Flag for sprite sheet or 1 image
		
		

	};

}


#ifdef OLC_PGEX_ROCK_OBJECT
#undef OLC_PGEX_ROCK_OBJECT

namespace olc
{

	RockObject::RockObject() : PGEX(true)
	{
		bisSpriteSheet = false;
	}

	RockObject::RockObject(std::string ImagePath, bool bIsSpriteSheet) : PGEX(true)
	{
		bisSpriteSheet = bIsSpriteSheet;
		Properties.strImagePath = ImagePath;

	}


	RockObject::~RockObject()
	{

		Properties.vecPartialImages.clear();
		Properties.vecPlayerFrames.clear();

	}


	void RockObject::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

		// Load up our image
		Properties.renImage.Load(Properties.strImagePath);
		Properties.sImageInfo.vScaleSize = Properties.sImageInfo.vSize * Properties.sImageInfo.vScale;

		// Is the rock on top or buttom?
		/*if (Properties.bRockTop)
		{
			Properties.vfPosition.x = pge->ScreenWidth() - Properties.sImageInfo.vScaleSize.x;
			Properties.vfPosition.y = Properties.sImageInfo.vScaleSize.y;
		}
		else
		{
			Properties.vfPosition.x = pge->ScreenWidth() - Properties.sImageInfo.vScaleSize.x;
			Properties.vfPosition.y = pge->ScreenHeight() - Properties.sImageInfo.vScaleSize.y;

		}*/

		// We know that our peaks of the rocks are offset from center
		// To ensure our collision triangles are correctly calculated we need to work out this offset

		olc::Pixel FindPixel = olc::BLANK; 
		bool bFoundStart = false;
		olc::vf2d vfStartPostion = { 0.0f, 0.0f };
		olc::vf2d vfEndPostion = { 0.0f, 0.0f };
		vfEndPostion.x = Properties.sImageInfo.vSource.x + Properties.sImageInfo.vSize.x;
		// Note as we are using a sprite sheet we need to search at the position of the image not the start position of the spritesheet

		for (int y = Properties.sImageInfo.vSource.y; y < (Properties.sImageInfo.vSource.y + Properties.sImageInfo.vSize.y); y++)
		{
			if (bFoundStart) break;
			for (int x = Properties.sImageInfo.vSource.x; x < (Properties.sImageInfo.vSource.x + Properties.sImageInfo.vSize.x); x++)
			{
				if (!bFoundStart)
				{
					if (FindPixel != Properties.renImage.Sprite()->GetPixel(x, y))
					{
						// ok we found the first position
						bFoundStart = true;
						vfStartPostion.x = x;
						vfStartPostion.y = y;
					}
				}
				else
				{
					// We keep going on the x axis until we find another olc::BLANK
					// If we do not find it, it is ok as we have set vfEndPostion.x to the width of the image
					if (FindPixel == Properties.renImage.Sprite()->GetPixel(x, y))
					{
						// ok we found the end poisition
						vfEndPostion.x = x;
						vfEndPostion.y = y;
						break; // We break here as we need no more information
					}
				}

			}
		}

		// Ok let work out our off set (debug to better understand)
		// NOTE: The Decal World and Sprite World do not aline, you made need to play with the values to get it perfect

		// First the the Peek center x offset in SpriteSheet World
		float x = (vfStartPostion.x + ((vfEndPostion.x - vfStartPostion.x) / 2));

		//Next get the Peek center x offset in the Sprite world
		Properties.vfPeekOffSet.x = x - Properties.sImageInfo.vSize.x;
		Properties.vfPeekOffSet.y = vfStartPostion.y;

		// Finally apply the scaler
		Properties.vfPeekOffSet *= Properties.sImageInfo.vScale;

		// Now we need to set the number of rocks on the screen
		vecCollisionTriangles.clear();
		if (Properties.nRockCount < 0) Properties.nRockCount = 1;

		for (int i = 0; i < Properties.nRockCount; i++)
		{
			Triangle collTri;
			collTri.nRockNumber = i;
			collTri.vfCurrentPosition.x = pge->GetScreenSize().x + ((pge->GetScreenSize().x / Properties.nRockCount) * i);
			collTri.vfCurrentPosition.y = pge->GetScreenSize().y - Properties.sImageInfo.vScaleSize.y;
			vecCollisionTriangles.emplace_back(collTri);


		}

	}


	void RockObject::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	bool RockObject::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate

		// Lets work out our scale size..
		// Basicilly we are going to get our size and mulitple it by our scaler, and then apply any max values to it
		Properties.sImageInfo.vScaleSize = Properties.sImageInfo.vSize * Properties.sImageInfo.vScale;

		// Ok to keep everything in seq, we will use frames per second as our game speed
		// in this way we can increase and decrease the speed of the game and all objects will react 
		// the same.
		// We can also cause delays, i.e. we can set -4.0f and put in a 5 second count down timer
		// I think I will do this lol
		Properties.fFrameChangeRate = 1.0f / Properties.fFramesPerSecound;
		Properties.fFrameElapsedTime += fElapsedTime;

		if (Properties.fFrameElapsedTime > Properties.fFrameChangeRate)
		{
			for (auto& tri : vecCollisionTriangles)
			{
				tri.vfCurrentPosition.x -= Properties.vfVelocity.x * fElapsedTime;
			}
			
		}

		// TODO: Update rock position



		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}


	void RockObject::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}


	void RockObject::DrawDecal()
	{
		// As the user can change the image sizes on the fly we need to manage autoscale here
		// ok we need to work out our scaling factor
		// To do this we need to workout by how much we need to increase/decrease our image to fit

		switch (Properties.SCALE)
		{
		case AUTO_SCALE:
		{
			Properties.sImageInfo.vScale.x = pge->GetScreenSize().x / Properties.sImageInfo.vScaleSize.x;
			Properties.sImageInfo.vScale.y = pge->GetScreenSize().y / Properties.sImageInfo.vScaleSize.y;
			break;
		}
		case SCALE_WIDTH:
		{
			Properties.sImageInfo.vScale.x = pge->GetScreenSize().x / Properties.sImageInfo.vScaleSize.x;
			break;
		}
		case SCALE_HEIGHT:
		{
			Properties.sImageInfo.vScale.y = pge->GetScreenSize().y / Properties.sImageInfo.vScaleSize.y;
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


		for (auto& tri : vecCollisionTriangles)
		{
			// Ok lets check do we need to reset?
			if ((tri.vfCurrentPosition.x + Properties.sImageInfo.vScaleSize.x) < 0.0f)
			{
				tri.vfCurrentPosition.x = pge->GetScreenSize().x;
			}

			// Lets work out where our triangle is now
			// NOTE: The Decal World and Sprite World do not aline, you made need to play with the values to get it perfect
			// The vfPeekOff set is a approx. value, in my case I need to subtract 2 to get it perfect
			tri.vfPosition1.x = vi2d(tri.vfCurrentPosition).x + Properties.vfPeekOffSet.x - 2;
			tri.vfPosition1.y = vi2d(tri.vfCurrentPosition).y;

			tri.vfPosition2.x = vi2d(tri.vfCurrentPosition).x;
			tri.vfPosition2.y = vi2d(tri.vfCurrentPosition).y + Properties.sImageInfo.vScaleSize.y;

			tri.vfPosition3.x = vi2d(tri.vfCurrentPosition).x + Properties.sImageInfo.vScaleSize.x;
			tri.vfPosition3.y = vi2d(tri.vfCurrentPosition).y + Properties.sImageInfo.vScaleSize.y;

			// First image
			pge->DrawPartialDecal(tri.vfCurrentPosition,
				Properties.renImage.Decal(),
				Properties.sImageInfo.vSource,
				Properties.sImageInfo.vSize,
				Properties.sImageInfo.vScale,
				Properties.sImageInfo.pxTint);

			pge->FillTriangle(tri.vfPosition1, tri.vfPosition2, tri.vfPosition3, olc::RED);

		}

		


	

		
	}



} // olc

#endif
