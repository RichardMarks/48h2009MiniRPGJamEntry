
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
		if (!gameSettings_)
		{
			gameSettings_ = new UTILITY::CONFIGURATION::Settings();

			// default settings
			gameSettings_->Set("screen_width", "800");
			gameSettings_->Set("screen_width", "600");
			gameSettings_->Set("screen_bpp", "24");
			gameSettings_->Set("use_fullscreen", "false");
			gameSettings_->Set("lock_fps_to", "60");

			gameSettings_->Set("data_path", "data/");
			gameSettings_->Set("start_world", "game.world");

			gameSettings_->Set("camera_w", "17");
			gameSettings_->Set("camera_h", "11");
			gameSettings_->Set("tile_w", "8");
			gameSettings_->Set("tile_h", "8");

			gameSettings_->Set("enable_collision_debugging", "false");
			gameSettings_->Set("enable_general_debugging", "false");
			gameSettings_->Set("enable_game_editors", "false");
			gameSettings_->Set("enable_verbose_startup", "false");

			gameSettings_->Set("enable_screen_overlays", "true");

			gameSettings_->Set("window_caption", "48h Contest LO-Fi Mini-RPG Game Project");

			// load the settings file if it exists
			if (UTILITY::FILESYSTEM::FileExists::Execute("game.cfg"))
			{
				gameSettings_->Load("game.cfg", false /* we do not want to clear the settings before loading the file! */);
			}
		}




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

		install_int_ex(allegroTimerSpeedController, BPS_TO_TIMER(static_cast<int>(atoi(gameSettings_->Get("lock_fps_to").c_str()))));

		int sw  = static_cast<int>(atoi(gameSettings_->Get("screen_width").c_str()));
		int sh  = static_cast<int>(atoi(gameSettings_->Get("screen_height").c_str()));
		int bpp = static_cast<int>(atoi(gameSettings_->Get("screen_bpp").c_str()));

		// setup the graphics device
		// we do not allow under 800 x 600 @ 24 bpp res
		GraphicsDevice->SetDisplay(
			(sw < 800) ? 800 : sw,
			(sh < 600) ? 600 : sh,
			(bpp == 32) ? GraphicsDevice_32bit :
			(bpp == 24) ? GraphicsDevice_24bit :
			(bpp < 24) ? GraphicsDevice_24bit : GraphicsDevice_24bit,
			("true" == gameSettings_->Get("use_fullscreen")) ? GraphicsDevice_Fullscreen : GraphicsDevice_Windowed);

		// setup the input device
		InputDevice->Initialize(INIT_KEYBOARD | INIT_MOUSE);

		// init the timing class
		GameTimer;

		// set the window title
		set_window_title(gameSettings_->Get("window_caption").c_str());

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

		//LoadWorldFile("data/worlds/game.world");
		LoadWorldFile(static_cast<std::string>(gameSettings_->Get("data_path") + "worlds/" + gameSettings_->Get("start_world")).c_str());


		// create the camera

		int camW = static_cast<int>(atoi(gameSettings_->Get("camera_w").c_str()));
		int camH = static_cast<int>(atoi(gameSettings_->Get("camera_h").c_str()));
		int tileW = static_cast<int>(atoi(gameSettings_->Get("tile_w").c_str()));
		int tileH = static_cast<int>(atoi(gameSettings_->Get("tile_h").c_str()));

		// keep the camera size in check
		camW = (camW < 1) ? 17 : (camW > (200 / tileW)) ? 17 : camW;
		camH = (camH < 1) ? 11 : (camH > (150 / tileH)) ? 11 : camH;

		camera_ = new GameCamera(
			1, 1, 			// rendering anchor x, y
			0, 0, 			// world x, y
			camW * tileW, 	// width in pixels
			camH * tileH, 	// height in pixels
			1, 				// camera pan speed
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
		if (argc > 1)
		{
			std::string argvStr = argv[1];
			if ("-edit" == argvStr)
			{
				if ("true" == gameSettings_->Get("enable_game_editors"))
				{
					SetState(GAMESTATE::MapEditor);
				}
				else
				{
					LogSimpleMessage("Not allowed!");
				}
			}
		}

		gameMenu_ = new GameMenuManager();

		return true;
	}

} // end namespace

