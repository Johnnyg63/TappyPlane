
//////////////////////////////////////////////////////////////////
// Pixel Game Engine Mobile Release 2.2.8                      //
// John Galvin aka Johnngy63: 18-Jun-2024                       //
// iOS Sensor NOT supported, coming soon                        //
// Please report all bugs to https://discord.com/invite/WhwHUMV //
// Or on Github: https://github.com/Johnnyg63					//
//////////////////////////////////////////////////////////////////

//
// Base Project
//


// Set up headers for the different platforms
#if defined (__ANDROID__)

#include "../TappyPlane/pch.h"
//#include "pch.h"

#endif

#if defined (__APPLE__)

#include "ios_native_app_glue.h"
#include <memory>

#endif

//#define STBI_NO_SIMD // Removes SIMD Support
// SIMD greatly improves the speed of your game
#if defined(__arm__)||(__aarch64__)

// Use Advance SIMD NEON when loading images for STB Default is SSE2 (x86)
#define STBI_NEON

#endif

#define OLC_IGNORE_VEC2D
#include "olcUTIL_Geometry2D.h"

#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine_Mobile.h"  // Main engine using STB Image hangler

#define OLC_PGEX_MINIAUDIO
#include "olcPGEX_MiniAudio.h"          // Checkout https://github.com/Moros1138/olcPGEX_MiniAudio Thanks Moros1138

#define OLC_PGEX_PLAYER_OBJECT
#include "olcPGEX_PlayerObject.h"       // Main Player object (The Tappy Plane)

#define OLC_PGEX_BACKGROUND_OBJECT
#include "olcPGEX_Background.h"         // Background Image handler, yep handles it all using easy properties 

#define OLC_PGEX_GROUND_OBJECT
#include "olcPGEX_Ground_Object.h"      // Ground object handler

#define OLC_PGEX_ROCK_OBJECT
#include "olcPGEX_Rock_Object.h"

#include <fstream> // Used for saving the savestate to a file

/// <summary>
/// To ensure proper cross platform support keep the class name as PGE_Mobile
/// This will ensure the iOS can launch the engine correctly
/// If you change it make the required changes in GameViewController.mm in the iOS app to suit
/// </summary>
class PGE_Mobile : public olc::PixelGameEngine {

public:

    // The instance of the audio engine, no fancy config required.
    olc::MiniAudio ma;

    // We use unique pointers to ensure each object is unique and can be cleaned up automatically 
    std::unique_ptr<olc::PlayerObject> pPlayer;
    std::unique_ptr<olc::BackgroundObject> pBackground;
    std::unique_ptr<olc::GroundObject> pGround;
    std::unique_ptr<olc::RockObject> pRock;

    PGE_Mobile() {
        sAppName = "Tappy Plane";

        pPlayer = std::make_unique<olc::PlayerObject>("images/planes_spritesheet.png", true);
        pBackground = std::make_unique<olc::BackgroundObject>("images/spritesheet.png", true);
        pGround = std::make_unique<olc::GroundObject>("images/spritesheet.png", true);
        pRock = std::make_unique<olc::RockObject>("images/spritesheet.png", true);

        pPlayer->Properties.strName = "Johnnyg63";    // Set our player name
        pPlayer->Properties.nPlayerNumber = 0;        // Set our player numbner

        pBackground->Properties.strName = "Master_Background";  // Set our Background name
        pBackground->Properties.nObjectNumber = 0;              // Set our Object number to 0 as it is our Master Background

        //<SubTexture name = "background.png" x = "0" y = "355" width = "800" height = "480" / >
        pBackground->Properties.sImageInfo.vSource = { 0, 355 };
        pBackground->Properties.sImageInfo.vSize = { 800, 480 };
        pBackground->Properties.sImageInfo.pxTint = olc::WHITE;


        pGround->Properties.strName = "Ground_Level";
        pGround->Properties.nObjectNumber = 0;

       //<SubTexture name = "groundDirt.png" x = "0" y = "0" width = "808" height = "71" / >
        pGround->Properties.sImageInfo.vSource = { 0, 0 };
        pGround->Properties.sImageInfo.vSize = { 808, 71 };
        pGround->Properties.sImageInfo.pxTint = olc::WHITE;
        pGround->Properties.SCALE = olc::GroundObject::SCALE_WIDTH;  // We only want to scale the width, the height does not matter


        // Rocks, lots of rocks
        pRock->Properties.strName = "BasicRock";
        pRock->Properties.nObjectNumber = 0;

        //<SubTexture name = "rock.png" x = "114" y = "1400" width = "108" height = "239" / >
        pRock->Properties.sImageInfo.vSource = { 114.0f, 1400.0f };
        pRock->Properties.sImageInfo.vSize = { 108.0f, 238.0f };
        pRock->Properties.sImageInfo.vScale = { 1.0f, 0.8f }; 
        pRock->Properties.SCALE = olc::RockObject::NONE;
        pRock->Properties.nRockCount = 100;
    }

    /* Vectors */
    std::vector<std::string> vecMessages;
    /* END Vectors*/

    int nFrameCount = 0;
    int nStep = 20;

    /* Sprites */
    olc::Sprite* sprOLCPGEMobLogo = nullptr; // OLC PGE Mobile logo pprite   

    /* END Sprites*/

    /* Decals */
    olc::Decal* decOLCPGEMobLogo = nullptr; // OLC PGE Mobile logo decal
    /* End Decals */

    // To keep track of our background music
    int32_t nBackgroundMusic = -1;
    int32_t nPlaneEngineSound = -1;

    // For demonstration controls, with sensible default values
    float pan = 0.0f;
    float pitch = 1.0f;
    float seek = 0.0f;
    float volume = 1.0f;

public:
    //Example Save State Struct and Vector for when your app is paused
    struct MySaveState {
        std::string key;
        int value;
    };

    std::vector<MySaveState> vecLastState;

    std::string strPlaneEngineSound;    // Holds the path to the planes engine sound
    std::string strBackgroundMusic;     // Holds the path to the background music

    bool bStartUpCompleted = false;

public:
    bool OnUserCreate() override {
        //NOTE: To access the features with your phone device use:
        
        strBackgroundMusic = (std::string)app_GetInternalAppStorage() + "/sounds/ItalianMom.mp3";
        olc::rcode fileRes = olc::filehandler->ExtractFileFromAssets("sounds/ItalianMom.mp3", strBackgroundMusic);

        switch (fileRes) {

            case olc::rcode::NO_FILE:
            { break; }
            case olc::rcode::FAIL:
            { break; }
            case olc::rcode::OK:
            {
                // only load the song if it is not already loaded
                nBackgroundMusic = ma.LoadSound(strBackgroundMusic);
                break;
            }
        }

        strPlaneEngineSound = (std::string)app_GetInternalAppStorage() + "/sounds/engineCircular_001.mp3";
        olc::filehandler->ExtractFileFromAssets("sounds/engineCircular_001.mp3", strPlaneEngineSound);

        nPlaneEngineSound = ma.LoadSound(strPlaneEngineSound);

        
        sprOLCPGEMobLogo = new olc::Sprite("images/olcpgemobilelogo.png");
        decOLCPGEMobLogo = new olc::Decal(sprOLCPGEMobLogo);


        // Load player details
        pPlayer->Properties.nMaxFrames = 3;
        pPlayer->Properties.nLives = 3;
        pPlayer->Properties.vfVelocity = { 10.0f, 10.0f };
        pPlayer->Properties.fFramesPerSecound = 20.0f;

        olc::PlayerObject::ImageInfo sInfo;
        sInfo.vSource = { 0.0f, 292.0f };
        sInfo.vSize = { 88.0f, 73.0f };
        pPlayer->Properties.vecPartialImages.push_back(sInfo);

        sInfo.vSource = { 0.0f, 219.0f };
        sInfo.vSize = { 88.0f, 73.0f };
        pPlayer->Properties.vecPartialImages.push_back(sInfo);

        sInfo.vSource = { 0.0f, 146.0f };
        sInfo.vSize = { 88.0f, 73.0f };
        pPlayer->Properties.vecPartialImages.push_back(sInfo);


        // Move the player to half way down screen, and 5% in from the right
        pPlayer->Properties.viStartPosition.x = (ScreenWidth() / 100) * 10;
        pPlayer->Properties.viStartPosition.y = (ScreenHeight() / 2);
        
        pGround->Properties.vfPosition.x = 0.0f;
        pGround->Properties.vfPosition.y = ScreenHeight() - pGround->Properties.sImageInfo.vSize.y;
        pGround->Properties.vfVelocity = { 20.0f, 0.0f }; // Set the x velocity only
        pPlayer->Properties.fFramesPerSecound = 20.0f;    // Set our FPS so that it is same as our plane

        
        pRock->Properties.vfVelocity = { 20.0f, 0.0f }; // Set the x velocity only
        pRock->Properties.fFramesPerSecound = 20.0f;    // Set our FPS so that it is same as our plane
        
        return true;
    }

   
    bool OnUserUpdate(float fElapsedTime) override {

        if (!bStartUpCompleted)
        {
            bStartUpCompleted = true;
            
            ma.Play(nPlaneEngineSound, true);
        }

        SetDrawTarget(nullptr);

        Clear(olc::BLUE);

        nFrameCount = GetFPS();

        // Order is important

        pBackground->DrawDecal();

        pPlayer->DrawDecal();

        pRock->DrawDecal();

        pGround->DrawDecal();

        CollisionManager();

        return true;
    }

    // Collision Maqnager
    void CollisionManager()
    {
        using namespace olc::utils::geom2d;

        bool bResult = false;

        for (auto& tri : pRock->vecCollisionTriangles)
        {
            bResult = overlaps(
                circle<float>{pPlayer->collCircle.vfCenterPos, pPlayer->collCircle.fRadius},
                triangle<float>{tri.vfPosition1, tri.vfPosition2, tri.vfPosition3}
            );

            if (bResult) break;
        }

        if (bResult == true)
        {
            // Ok we have hit the triangle
            pPlayer->Properties.nLives--;
            if (pPlayer->Properties.nLives < 0)
            {
                // Game over!!!
            }

        }

    }

    bool OnUserDestroy() override {

        // We are done with the Player Object, lets ensure we clean up 
        ma.~MiniAudio();

        return true;
    }

    void OnSaveStateRequested() override
    {
        // Fires when the OS is about to put your game into pause mode
        // You have, at best 30 Seconds before your game will be fully shutdown
        // It depends on why the OS is pausing your game tho, Phone call, etc
        // It is best to save a simple Struct of your settings, i.e. current level, player position etc
        // NOTE: The OS can terminate all of your data, pointers, sprites, layers can be freed
        // Therefore do not save sprites, pointers etc 

        // Example 1: vector
        vecLastState.clear();
        vecLastState.push_back({ "MouseX", 55 });
        vecLastState.push_back({ "MouseY", 25 });
        vecLastState.push_back({ "GameLevel", 5 });

#if defined(__ANDROID__)
        // You can save files in the android Internal app storage
        const char* internalPath = app_GetInternalAppStorage(); //Android protected storage
#endif
#if defined(__APPLE__)
        // For iOS the internal app storage is read only, therefore we use External App Storage
        const char* internalPath = app_GetExternalAppStorage(); // iOS protected storage AKA /Library
#endif

        std::string dataPath(internalPath);

        // internalDataPath points directly to the files/ directory                                  
        std::string lastStateFile = dataPath + "/lastStateFile.bin";

        std::ofstream file(lastStateFile, std::ios::out | std::ios::binary);

        if (file)
        {
            float fVecSize = vecLastState.size();
            file.write((char*)&fVecSize, sizeof(long));
            for (auto& vSS : vecLastState)
            {
                file.write((char*)&vSS, sizeof(MySaveState));
            }

            file.close();
        }


    }

    void OnRestoreStateRequested() override
    {
        // This will fire every time your game launches 
        // OnUserCreate will be fired again as the OS may have terminated all your data

#if defined(__ANDROID__)
        // You can save files in the android Internal app storage
        const char* internalPath = app_GetInternalAppStorage(); //Android protected storage
#endif
#if defined(__APPLE__)
        // For iOS the internal app storage is read only, therefore we use External App Storage
        const char* internalPath = app_GetExternalAppStorage(); // iOS protected storage AKA /Library
#endif

        std::string dataPath(internalPath);
        std::string lastStateFile = dataPath + "/lastStateFile.bin";

        vecLastState.clear();

        std::ifstream file(lastStateFile, std::ios::in | std::ios::binary);

        MySaveState saveState;

        if (file)
        {
            float fVecSize = 0.0f;
            file.read((char*)&fVecSize, sizeof(long));
            for (long i = 0; i < fVecSize; i++)
            {
                file.read((char*)&saveState, sizeof(MySaveState));
                vecLastState.push_back(saveState);
            }

            file.close();
            // Note this is a temp file, we must delete it
            std::remove(lastStateFile.c_str());

        }


    }

};


#if defined (__ANDROID__)
/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
* This is now what drives the engine, the thread is controlled from the OS
*/
void android_main(struct android_app* initialstate) {

    /*
        initalstate allows you to make some more edits
        to your app before the PGE Engine starts
        Recommended just to leave it at its defaults
        but change it at your own risk
        to access the Android/iOS directly in your code
        android_app* pMyAndroid = this->pOsEngine.app;;

    */

    PGE_Mobile demo;

    /*
        Note it is best to use HD(1280, 720, ? X ? pixel, Fullscreen = true) the engine can scale this best for all screen sizes,
        without affecting performance... well it will have a very small affect, it will depend on your pixel size
        Note: cohesion is currently not working
    */
    demo.Construct(1280, 720, 2, 2, true, false, false);

    demo.Start(); // Lets get the party started


}

#endif

#if defined(__APPLE__)

/*
* The is the calling point from the iOS Objective C, called during the startup of your application
* Use the objects definded in IOSNativeApp to pass data to the Objective C
* By Default you must at minmum pass the game construct vars, pIOSNatvieApp->SetPGEConstruct
*
* iOS runs in its own threads, with its own
* event loop for receiving input events and doing other things.
* This is now what drives the engine, the thread is controlled from the OS
*/
int ios_main(IOSNativeApp* pIOSNatvieApp)
{
    // The iOS will instance your app differnetly to how Android does it
    // In the iOS it will automatically create the required classes and pointers
    // to get the PGE up and running successfull.

    // IMPORTANT: You must set your class name to PGE_Mobile (see above) always for iOS
    // Don't worry it will not conflict with any other apps that use the same base class name of PGE_Mobile
    // I got your back

    // Finally just like the Android you can access any avialble OS options using pIOSNatvieApp
    // Please note options will NOT be the same across both platforms
    // It is best to use the build in functions for File handling, Mouse/Touch events, Key events, Joypad etc

    //
    // To access the iOS directly in your code
    // auto* pMyApple = this->pOsEngine.app;
    //

    /*
        Note it is best to use HD(0, 0, ? X ? pixel, Fullscreen = true) the engine can scale this best for all screen sizes,
        without affecting performance... well it will have a very small affect, it will depend on your pixel size
        Note: cohesion is currently not working
        Note: It is best to set maintain_aspect_ratio to false, Fullscreen to true and use the olcPGEX_TransformView.h to manage your world-view
        in short iOS does not want to play nice, the screen ratios and renta displays make maintaining a full screen with aspect radio a pain to manage
    */
    pIOSNatvieApp->SetPGEConstruct(0, 0, 2, 2, true, true, false);


    // We now need to return SUCCESS or FAILURE to get the party stated!!!!
    return EXIT_SUCCESS;
}

#endif


