
// CODESTYLE: v2.0

// Game_Initialize.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Initialize method for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	bool GameSingleton::Initialize(int argc, char* argv[])
	{
		// initialize Allegro
		if (0 != allegro_init())
		{
			LogFatal("Could not initialize Allegro!");
		}

		// install the Allegro timer driver
		if (0 != install_timer())
		{
			LogFatal("Could not install Allegro Timer driver!");
		}
		
		// setup the 60fps allegro speed controller
		LOCK_VARIABLE(allegroTimerSpeedCounter);
		LOCK_FUNCTION(allegroTimerSpeedController);
		install_int_ex(allegroTimerSpeedController, BPS_TO_TIMER(60));
		
		// setup the graphics device
		GraphicsDevice->SetDisplay(
			800, 
			600, 
			GraphicsDevice_24bit, 
			GraphicsDevice_Windowed);
			
		// setup the input device
		InputDevice->Initialize(INIT_KEYBOARD);
		
		// init the timing class
		GameTimer;
		
		// set the window title
		set_window_title("48h Contest LO-Fi Mini-RPG Game Project");
		
		// load fonts
		smallFont_ = new BitmapFont();
		if (!smallFont_->Load("data/graphics/fonts/font5x5white.png", 5, 5, 1)) { return false; }
	
		largeFont_ = new BitmapFont();
		if (!largeFont_->Load("data/graphics/fonts/font8x8white.png", 8, 8, 1)) { return false; }

		// create the display surface we draw on for implementing the cool 400% scale 
		microDisplay_ = new ImageResource(200, 150);
	
		playerSpriteIndex_ = 0;
		
		LoadWorldFile("data/worlds/game.world");
		
	
	
		// create the camera
		camera_ = new GameCamera(
			1, 1, 		// rendering anchor x, y
			0, 0, 		// world x, y
			17 * 8, 	// width in pixels
			11 * 8, 	// height in pixels
			1, 			// camera pan speed
			currentMap_->GetGameMapLayer(0)->GetWidth(),
			currentMap_->GetGameMapLayer(0)->GetHeight()
			);
		
		
		// center the camera on the player
		camera_->CenterOnSprite(gameSprites_->Get(playerSpriteIndex_));
		
		// create the player object
		// playerObject_ = new GameObject("data/graphics/game/sprites/player.png", 8, 8, 3, 5);
	
		// center the player in the camera
		//int camWidth = 0, camHeight = 0;
		//int camWorldX = 0, camWorldY = 0;
		
		//camera_->GetSize(camWidth, camHeight);
		//camera_->GetWorldPosition(camWorldX, camWorldY);
		
		// playerObject_->GetSprite()->SetScreenPosition(camWorldX + camWidth / 2, camWorldY + camHeight / 2);
		
		windowOverlay_ 	= new ImageResource("data/graphics/game/overlays/mainscreen.png");
		lofiOverlay_ 	= new ImageResource("data/graphics/game/overlays/lofi.png");
		playerPortrait_ = new ImageResource("data/graphics/portraits/player.png");

		return true;
	}
	
} // end namespace

