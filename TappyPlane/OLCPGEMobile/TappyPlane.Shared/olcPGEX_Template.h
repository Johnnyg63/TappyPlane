#pragma once

//////////////////////////////////////////////////////////////////
// Pixel Game Engine Mobile Release 2.2.8                       //
// John Galvin aka Johnngy63: 29-May-2024                       //
// iOS Sensor NOT supported, coming soon                        //
// Please report all bugs to https://discord.com/invite/WhwHUMV //
// Or on Github: https://github.com/Johnnyg63					//
//////////////////////////////////////////////////////////////////



/*
* This is a Template Pixel Game Engine Extension
* Use this template to control objects that need to be updated before/after the master OnUserCreate and OnUserUpdate
* For example, you can:
*
*   1: Create multiple versions of this template to handle an object position
*   2: Use the OnBeforeUserUpdate to move the enemies into position
*   3: Call the DrawDecal() from main.cpp/master OnUserUpdate to draw the object to the screen
*   4: Create a version of this template to handle collision
*   5: Use the OnAfterUserUpdate to check for collision and handle the object position as required
*
* No complex config required, just follow these steps:
*
*   1: Create a copy of this olcPGEX_Template.h and rename it to, what ever, example "olcPGEX_Player_Object.h"
*   2: Rename OLC_PGEX_TEMPLATE to your header file name, example OLC_PGEX_PLAYER_OBJECT (and yes you do not need to use CAPS, I'm old school)
*   3: Rename the all instances of Template to your Class Name, Example : PlayerObject
*   4: When you want to use this class/object #define OLC_PGEX_<YOUR_CLASS> followed by your header name. Example:
*       a: At top of main.cpp...
*          #define OLC_PGEX_PLAYER_OBJECT           (NOTE: only needs to be done once at the first point of contact)
*          #include "olcPGEX_Player_Object.h"
*
*       b: You only need to #define once, if you want to use the header in other areas, just #include the header
*          for example, at the top of olcPGX_collision.h...
*          #include "olcPGEX_Player_Object.h"
*
*       c: Under a public: keyword
*          olc::PlayerObject MyPlayer;
*
*   5: And thats it, have fun... lots of fun,
*/


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
#ifdef OLC_PGEX_TEMPLATE
// Add extra defines for sub header files you might be using :)
#endif

namespace olc
{
	/*
	* See Step 3: Rename to your Class name
	*/
	class Template : public PGEX
	{

	public:
		Template(); // See Step 3: Rename to your Class name
		virtual ~Template(); // See Step 3: Rename to your Class name

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
		* Example struct that can be used within the Properties struct for a vector, vecPartialImages, in this case
		*/
		struct ImageInfo
		{
			olc::vf2d vSource = { 0.0f, 0.0f };
			olc::vf2d vSize = { 1.0f, 1.0f };
		};

		/*
		* This is your Class/Object properities example
		* Add and edit as you need, it will allow you to make changes to the object on the fly
		* As this Class is connected to the engine it will be automatically called when needed
		* no complex config required, "we do the engine, you do the fun stuff"
		*/
		struct ObjectProperites
		{
			std::string strName = "GameObject"; // Object name. Default "GameObject"
			int8_t nObjectNumber = 0;	        // Object number, Default 0 i.e. Player 1 , Player 2 etc

			uint32_t nLives = 3;	        // Lives, Default 3
			uint32_t nMaxFrames = 3;        // Max number of frames, Default 0, this means we are not anitmating this object
			uint32_t nCurrentFrame = 0;     // Stores the number of the last frame displayed
			uint32_t nCurrentFPS = 0;       // Stores the current Engine FPS

			float fFrameChangeRate = 0.0f;  // This value will auto calcuate the rate of change depending on the current FPS
			float fFrameElapsedTime = 0.0f; // Keeps track of the time pass since the last frame change
			float fFramesPerSecound = 1.0f; // Set the number of times the frame is to change per second 

			olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

			olc::vi2d viStartPosition = { 0,0 };	        // Start Position {x,y} (Int32_t), Default {0,0}
			//olc::vf2d vfStartPosition = { 0.0f, 0.0f };	    // Start Positon POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			olc::vi2d viPosition = { 0,0 };	        // Player current POS {x,y} (Int32_t), Default {0,0}
			olc::vf2d vfPosition = { 0.0f,0.0f };	// Player current POS {x,y} (float), Default {0.0f,0.0f}, recommended for decals

			std::string strSpritePath = "";     // Sprite path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprImage = nullptr;	// Player Sprite image, Default nullptr
			olc::Decal* decImage = nullptr;	    // Player Decal Image, Defalut nullptr
			ImageInfo sprImageInfo;             // Stores the Source and Size of the sprImage


			std::string strSpriteSheetPath = "";     // SpriteSheetPath path, i.e. "images/mysprite.png", Default: ""
			olc::Sprite* sprSpriteSheet = nullptr;	// Player Sprite Sheet image, Default nullptr
			olc::Decal* decSpriteSheet = nullptr;	// Plaer Decal Sheet Image, Defalut nullptr

			olc::Renderable renImage;      // Keeps the sprImage and decImage in the one location
			olc::Renderable renSpriteSheet;  // Keeps the sprSpriteSheet and decSpriteSheet in the one location
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

	};

}

/*
* See Step 2: Rename to your header file name
*/
#ifdef OLC_PGEX_TEMPLATE
#undef OLC_PGEX_TEMPLATE

namespace olc
{

	// See Step 3: Rename to your Class name
	Template::Template() : PGEX(true)
	{

	}

	// See Step 3: Rename to your Class name
	Template::~Template()
	{

		/*
		*			Everyone is always afraid of Memory Leaks!!!!!, but memory management in C/C++ is super easy
		*/

		/*
		* 
		* The keywords, new, malloc, calloc, are evil!,
		*	 but the new keyword is the worse by far! 
		* 
		* We must ensure we clean up after we have used these nasty keywords ;)
		* If you are using a Smart Pointer (i.e. std::unique_ptr<olc::PlayerObject> ...) then the Smart Pointer will automatically
		* call this deconstructor.... you have nothing to worry about
		*
		* However:
		* 
		* If you are using a pointer (i.e. olc::PlayerObject* pPlayer) or a direct referance (olc::PlayerObject Player)
		* You will need to ensure you call the deconstructor when your class is out of scope (i.e. no longer needed)
		* 
		* For example in the main.cpp, deconstructor ( ~MainClass() ) or in a method like OnUserDestroy():
		*   //Pointer example:
		*   pPlayer->~Player();
		*
		*   // Ref example
		*   Player.~Player();
		*
		* This will ensure all your memory is cleaned up and removed :)
		*/


		/*
		*	 Some examples of clean up
		* 
		* Although under the hood Sprites & Decals are automatically cleaned up, it is always good practise to delete them anyway
		* NOTE: delete can be called on a nullptr (i.e. you can request to delete memory that does not exist and nothing bad will happen)
		* 
		*/
		if (Properties.decImage != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.decImage;
		}

		if (Properties.sprImage != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.sprImage;
		}

		if (Properties.decSpriteSheet != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.decSpriteSheet;
		}

		if (Properties.sprSpriteSheet != nullptr)
		{
			// Only delete if it exists in memory
			delete Properties.sprSpriteSheet;
		}

		/*
		*  By calling the clear() function of std::vector, will also call any elements deconstructors,
		*  and will automatically clean everything up
		*/
		Properties.vecPartialImages.clear();

		/*
		*  To Clean up malloc/calloc you really should clean these up from within the method
		*  for example:
		*  buffer = malloc(num_items*sizeof(double));
		   // do something
		   free(buffer);
		* 
		*  for arrays...
		*	 double **cross_norm=(double**)malloc(150 * sizeof(double *));
			 for(i=0; i<150;i++)
			 {
				cross_norm[i]=(double*)malloc(num_items*sizeof(double));
			 }

			 // do something

			for(i=0; i<150;i++)
			{
				free(cross_norm[i]);
			}

			free(cross_norm); // yes you need to clear each element and then the object itself... 

		*/

		/*
		* 
		* Clean-up of simplistic types, i.e. int, double, float is not required
		*	These will be taken care of automatically
		* 
		* Clean-up of std::string
		*	 You can if you wish, but as it is a part of the Standard Lib, it will be taken care of :)
		*
		* Clean-up of Standard lib elements (std::???)
		*	Good news, the Standard Lib pretty much takes care of itself, when it comes to memory management
		*	or else it will give you a simple command to work it,
		*	for example, std::vector
		*		there will be a vector.clear(), vector.erase() :) 
		* 
		*/


	}

	// See Step 3: Rename to your Class name
	void Template::OnBeforeUserCreate()
	{
		// Fires just before the main OnUserCreate

	}

	// See Step 3: Rename to your Class name
	void Template::OnAfterUserCreate()
	{
		// Fires just After the main OnUserCreate
	}

	// See Step 3: Rename to your Class name
	bool Template::OnBeforeUserUpdate(float& fElapsedTime)
	{
		// Fires just before the main OnUserUpdate
		return false; // Return true to cancel any other OnBeforeUserUpdate() not recommended 
	}

	// See Step 3: Rename to your Class name
	void Template::OnAfterUserUpdate(float fElapsedTime)
	{
		// Fires just After the main OnUserUpdate
	}

	// See Step 3: Rename to your Class name
	void Template::DrawDecal()
	{

	}

	// See Step 3: Rename to your Class name
	void Template::DrawSprite()
	{

	}



} // olc

#endif
