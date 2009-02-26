
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
		LOCK_FUNCTION(allegroTimer_RenderController);

		install_int_ex(allegroTimerSpeedController, BPS_TO_TIMER(60));

		// setup the graphics device
		GraphicsDevice->SetDisplay(
			800,
			600,
			GraphicsDevice_24bit,
			GraphicsDevice_Windowed);

		// setup the input device
		InputDevice->Initialize(INIT_KEYBOARD | INIT_MOUSE);

		// init the timing class
		GameTimer;

		// set the window title
		set_window_title("48h Contest LO-Fi Mini-RPG Game Project");

		// load fonts
		smallFont_ = new BitmapFont();
		if (!smallFont_->Load("data/graphics/fonts/font5x5white.png", 5, 5, 1)) { return false; }

		largeFont_ = new BitmapFont();
		if (!largeFont_->Load("data/graphics/fonts/font8x8white.png", 8, 8, 1)) { return false; }

		// create the dialogue message
		dialogueMessage_ = new GameDialogueMessage();

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

		windowOverlay_ 	= new ImageResource("data/graphics/game/overlays/mainscreen.png");

		playerPortrait_ = new ImageResource("data/graphics/portraits/player.png");

		// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		lofiOverlay_ 	= new ImageResource("data/graphics/game/overlays/lofi.png");
#endif

		stepsTaken_ 		= 0;
		stepsUntilAmbush_ 	= 160;

		gameStateManager_ = new GAMESTATE::GameStateManager();

		// we will start with the map editor if the argv[] is -edit
		if (argc>1)
		{
			std::string argvStr = argv[1];
			if ("-edit" == argvStr)
			{
				SetState(GAMESTATE::MapEditor);
			}
		}

		gameMenu_ = new GameMenuManager();

		return true;
	}

} // end namespace

