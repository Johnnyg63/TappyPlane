#pragma once

//////////////////////////////////////////////////////////////////
// Pixel Game Engine Mobile Release 2.2.8                       //
// John Galvin aka Johnngy63: 29-May-2024                       //
// iOS Sensor NOT supported, coming soon                        //
// Please report all bugs to https://discord.com/invite/WhwHUMV //
// Or on Github: https://github.com/Johnnyg63					//
//////////////////////////////////////////////////////////////////


#if defined (__ANDROID__) || (__APPLE__)

#include "olcPixelGameEngine_Mobile.h"
#else

#include "olcPixelGameEngine.h"
#endif // defined (__ANDROID__) || (__APPLE__)
#include <exception>

#ifdef OLC_PGEX_PLAYER_OBJECT

#endif

namespace olc
{
    class PlayerObject : public PGEX
    {

    public:
        /*
        * Default Constructor:
        * The class will create a temp blank sprite 10pxX10px
        * You will need to update the renImage and renSpriteSheet manually from your code
        */
        PlayerObject();

        /*
        * Recommended Constructor:
        * The class will create and setup renImage or renSpriteSheet depending on bIsSpriteSheet flag
        * You can still update the renImage and renSpriteSheet manually from your code
        */
        PlayerObject(std::string ImagePath, bool bIsSpriteSheet = false);

        // virtual Default De-Constructor:
        virtual ~PlayerObject();

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


    public:

        /*
        * Stores the image Source and Size for the sprImage
        * or for use in vecPartialImages
        */
        struct ImageInfo
        {
            olc::vf2d vSource = { 0.0f, 0.0f }; // Source poisition to draw drawing from, default: {0.0f, 0.0f)
            olc::vf2d vSize = { 1.0f, 1.0f };	// The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
            olc::vf2d vScaleSize = { 1.0f, 1.0f }; // Auto generated, stores the scale size of the image in pixels
            olc::vf2d vScale = { 1.0f, 1.0f };	// Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
            olc::Pixel pxTint = olc::WHITE;		// Tint colour for background, set to olc::DARK_GREY for a night time effect
        };

        struct Circle {

            olc::vf2d vfCenterPos = { 0.0f, 0.0f };
            float fRadius = 0.0f;
        };

        /*
        * This Object Properites Struct
        */
        struct ObjectProperites
        {
            std::string strName = "GameObject"; // Object name. Default "GameObject"
            int8_t nPlayerNumber = 0;	        // Object number, Default 0 i.e. Player 1 , Player 2 etc

            int32_t nLives = 3;	        // Lives, Default 3
            uint32_t nMaxFrames = 3;        // Max number of frames, Default 0, this means we are not anitmating this object
            uint32_t nCurrentFrame = 0;     // Stores the number of the last frame displayed
            uint32_t nCurrentFPS = 0;       // Stores the current Engine FPS

            bool bIsGodMode = false;            // Stores if the player is in God Mode, Default: false
            bool bIsVisiable = true;            // Enable/Disable visiabilty 
            float fGodModeTimeOutSeconds = 10.0f;  // Stores the timeout for God mode, Default: 3000ms
            float fGodModeFlashSeconds = 0.2f;     // Flash timer for when the player is in God Mode
 
            float fFrameChangeRate = 0.0f;      // This value will auto calcuate the rate of change depending on the current FPS
            float fFrameElapsedTime = 0.0f;     // Keeps track of the time pass since the last frame change
            float fFramesPerSecound = 1.0f;     // Set the number of times the frame is to change per second, Default: 1.0f

            olc::vf2d vfVelocity = { 0.0f, 0.0f };  // Velocity, vfsd {x, y}, Default: {0.0f, 0.0f}

            olc::vi2d viStartPosition = { 0,0 };	        // Start Position {x,y} (Int32_t), Default {0,0}
           
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

        /*
        * Proprties
        */
        ObjectProperites Properties;

        // This is the collision circle(s)
        Circle collCircle;

        private:
            bool bIsEmptySprite = false;
            bool bisSpriteSheet = false;
            std::string strImagePath = "";
            float fGodModeTimer = 0.0f;         // Use to timeout GodMode when the timeout is reached
            float fGodModeFlashTimer = 0.0f;    // Used to flash the Decal while in God mode
    };

}


#ifdef OLC_PGEX_PLAYER_OBJECT
#undef OLC_PGEX_GAME_OBJECT

namespace olc
{
    

    PlayerObject::PlayerObject() : PGEX(true)
    {
        bIsEmptySprite = true;
    }

    PlayerObject::PlayerObject(std::string ImagePath, bool bIsSpriteSheet) : PGEX(true)
    {
        bisSpriteSheet = bIsSpriteSheet;
        strImagePath = ImagePath;
    }


    PlayerObject::~PlayerObject()
    {
        // new and delete are evil and we should be using Smart Pointers instead of new & delete
        // However in this case, it makes this object to complex to manage, and this object should be
        // instancated using a smart pointer (see main.cpp) therefore the calling of the deconstructor
        // will always happen, hence the delete will ensure no memory leaks.... debug to understand please

        // lets clean up
        if (Properties.decImage != nullptr)
        {
            delete Properties.decImage;
        }

        if (Properties.sprImage != nullptr)
        {
            delete Properties.sprImage;
        }

        if (Properties.decSpriteSheet != nullptr)
        {
            delete Properties.decSpriteSheet;
        }

        if (Properties.sprSpriteSheet != nullptr)
        {
            delete Properties.sprSpriteSheet;
        }

        Properties.vecPartialImages.clear();

    }

    void PlayerObject::OnBeforeUserCreate()
    {
        // Ok let create our player object

        if (bIsEmptySprite)
        {
            Properties.renImage.Create(10, 10);
            Properties.sprImage = Properties.renImage.Sprite();
            Properties.decImage = Properties.renImage.Decal();
            Properties.sprImageInfo.vSource = { 0.0f, 0.0f };
            Properties.sprImageInfo.vSize = { 10.0f, 10.0f };
            return;
        }


        // There is a chance that our developer decided to update the properities after instantiated this class using the default construtor
        // and now has updated the properites before this execution
        // Therefore the below code checks and ensures these "new" properties are applied

        if (bisSpriteSheet)
        {
            // lets create Sprite Sheet
            Properties.strSpriteSheetPath = strImagePath;
            Properties.renSpriteSheet.Load(Properties.strSpriteSheetPath);
            Properties.sprSpriteSheet = Properties.renSpriteSheet.Sprite();
            Properties.decSpriteSheet = Properties.renSpriteSheet.Decal();
            Properties.sprImageInfo.vSize = Properties.renSpriteSheet.Sprite()->Size();

        }
        else
        {
            // Lets create a sprite
            Properties.strSpritePath = strImagePath;
            Properties.renImage.Load(Properties.strSpritePath);
            Properties.sprImage = Properties.renImage.Sprite();
            Properties.decImage = Properties.renImage.Decal();
            Properties.sprImageInfo.vSize = Properties.renImage.Sprite()->Size();

        }


        Properties.vfPosition = Properties.viStartPosition;
        Properties.viPosition = Properties.vfPosition;

    }

    void PlayerObject::OnAfterUserCreate()
    {

    }

    bool PlayerObject::OnBeforeUserUpdate(float& fElapsedTime)
    {
        
        /* Update our postion  */

        // Ok we are falling....
        Properties.vfPosition.y += Properties.vfVelocity.y * fElapsedTime;

        // Have we hit the bottom?
        if (Properties.vfPosition.y > (pge->ScreenHeight() - Properties.sprImageInfo.vSize.y))
            Properties.vfPosition.y = pge->ScreenHeight() - Properties.sprImageInfo.vSize.y;

        if (pge->GetTouch().bReleased)
        {
            // We are tap...tap..tapping
            Properties.vfPosition.y -= Properties.vfVelocity.y * 200.0f * fElapsedTime;
        }

        // Have we hit the top
        if (Properties.vfPosition.y < 1.0f)
            Properties.vfPosition.y = 1.0f;

        // Decals uses vf2f (floats) while Sprites use vi2d (int32_t), let case vfPos to vnPos
        Properties.viPosition = Properties.vfPosition;

        /* END Update our postion  */

        /* Update our frame  */

        // Ok lets check if we are using frames (animation)
        if (Properties.renSpriteSheet.Decal() == nullptr || Properties.nMaxFrames == 0)
        {
            return false; // we are not using animation lets get outta here
        }


        // Calculate the rate of change from frames
        Properties.fFrameChangeRate = 1.0f / Properties.fFramesPerSecound;
        Properties.fFrameElapsedTime += fElapsedTime;

        if (Properties.fFrameElapsedTime > Properties.fFrameChangeRate)
        {
            Properties.fFrameElapsedTime = 0.0f;
            Properties.nCurrentFrame++;
            if (Properties.nCurrentFrame >= Properties.nMaxFrames)
            {
                Properties.nCurrentFrame = 0;
            }
        }

        // Ok now for the fun part, 
        // We need to update the output decal so the main engine knows what to draw
        // To do this we use the public method to drawObject
        /// DrawDecal and DrawSprite methods

        /* End Update our frame  */

        return false;
    }

    void PlayerObject::OnAfterUserUpdate(float fElapsedTime)
    {
        if (Properties.bIsGodMode)
        {
            fGodModeFlashTimer += fElapsedTime;
            if (fGodModeFlashTimer > Properties.fGodModeFlashSeconds)
            {
                Properties.bIsVisiable = !Properties.bIsVisiable;
                fGodModeFlashTimer = 0.0f;
            }

            fGodModeTimer += fElapsedTime;
            if (fGodModeTimer > Properties.fGodModeTimeOutSeconds)
            {
                Properties.bIsVisiable = true;
                Properties.bIsGodMode = false;
            }

        }
        else
        {
            fGodModeFlashTimer = 0.0f;
            fGodModeTimer = 0.0f;
        }
    }

    void PlayerObject::DrawDecal()
    {
        if (Properties.nMaxFrames < 1)
        {
            // Ok we are just drawing a decal
            if(Properties.bIsVisiable) pge->DrawDecal(Properties.vfPosition, Properties.decImage);
        }
        else
        {
            // Ok we are drawing frames 
            // Check 1: is the vector empty
            if (Properties.vecPartialImages.empty())
            {
                return; // there is nothing we can do
            }

            // Check 2: ensure our nCurrentFrame is in the vector
            if (Properties.nCurrentFrame >= Properties.vecPartialImages.size())
                Properties.nCurrentFPS = 0;

            olc::vi2d vfSourcePos = Properties.vecPartialImages[Properties.nCurrentFrame].vSource;
            olc::vi2d vfSize = Properties.vecPartialImages[Properties.nCurrentFrame].vSize;

            Properties.sprImageInfo.vSource = vfSourcePos;
            Properties.sprImageInfo.vSize = vfSize;

            if (Properties.bIsVisiable) pge->DrawPartialDecal(Properties.vfPosition, Properties.decSpriteSheet, vfSourcePos, vfSize, Properties.sprImageInfo.vScale, Properties.sprImageInfo.pxTint);

            // Now that we have our Player position we need to setup our collision circle
            // NOTE: The Decal World and Sprite World do not aline, you made need to play with the values to get it perfect
            // Our collision circle will have an approx center of the decal with an approx radius

            // 1: Lets get our center point
            collCircle.vfCenterPos = Properties.vfPosition + (vfSize / 2);
            collCircle.vfCenterPos *= Properties.sprImageInfo.vScale;

            // 2: Lets get the radius, we want the small circle that can fit within the decal
            collCircle.fRadius = (std::min(vfSize.x, vfSize.y) / 2) * std::min(Properties.sprImageInfo.vScale.x , Properties.sprImageInfo.vScale.y);

            pge->FillCircle(collCircle.vfCenterPos, collCircle.fRadius, olc::GREEN);


        }
    }

    void PlayerObject::DrawSprite()
    {
        if (Properties.nMaxFrames < 1)
        {
            // Ok we are just drawing a decal
            if (Properties.bIsVisiable) pge->DrawSprite(Properties.viPosition, Properties.sprImage);
        }
        else
        {
            // Ok we are drawing frames 
            // Check 1: is the vector empty
            if (Properties.vecPartialImages.empty())
            {
                return; // there is nothing we can do
            }

            // Check 2: ensure our nCurrentFrame is in the vector
            if (Properties.nCurrentFrame >= Properties.vecPartialImages.size())
                Properties.nCurrentFPS = 0;


            olc::vi2d viSourcePos = Properties.vecPartialImages[Properties.nCurrentFrame].vSource;
            olc::vi2d viSize = Properties.vecPartialImages[Properties.nCurrentFrame].vSize;

            Properties.sprImageInfo.vSource = viSourcePos;
            Properties.sprImageInfo.vSize = viSize;

            if (Properties.bIsVisiable) pge->DrawPartialSprite(Properties.viPosition, Properties.sprSpriteSheet, viSourcePos, viSize);

        }
    }

} // olc

#endif
