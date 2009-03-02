
// CODESTYLE: v2.0

// Game_Initialize.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Initialize method for the game singleton class

#include "GameLibrary.h"

#include "GameMap_Editors.h"

namespace GAME
{

	bool GameSingleton::Initialize(int argc, char* argv[])
	{
		gameSettings_ = new UTILITY::CONFIGURATION::Settings();

		if (gameSettings_)
		{
			// default settings
			gameSettings_->Set("screen_width", "800");
			gameSettings_->Set("screen_width", "600");
			gameSettings_->Set("screen_bpp", "24");
			gameSettings_->Set("use_fullscreen", "false");
			gameSettings_->Set("lock_fps_to", "60");

			// the micro display
			gameSettings_->Set("micro_screen_width", "200");
			gameSettings_->Set("micro_screen_height", "150");

			/*
				data file paths

				ROOT OF ALL DATA FILES
				data_path - relative to executable

				.WORLD FILES DIRECTORY
				world_path - relative to data_path

				MAPS DIRECTORY
				map_path - relative to data_path

				GRAPHICS DIRECTORY
				graphics_path - relative to data_path

				.DIALOGUE FILES DIRECTORY
				dialogue_path - relative to data_path

				.BATTLE FILES DIRECTORY
				battle_path - relative to data_path


				TILESET DIRECTORY
				tiles_path - relative to graphics_path

				GAME SPRITES DIRECTORY
				game_sprite_path - relative to graphics_path

				BATTLE SPRITES DIRECTORY
				battle_sprite_path - relative to graphics_path

				BATTLE SCENE IMAGERY DIRECTORY
				battle_scene_path - relative to graphics_path

				FONT DIRECTORY
				font_path - relative to graphics_path

				GAME OVERLAY IMAGES DIRECTORY
				game_overlay_path - relative to graphics_path

				BATTLE OVERLAY IMAGES DIRECTORY
				battle_overlay_path - relative to graphcis_path

				CHARACTER PORTRAIT IMAGES DIRECTORY
				portrait_path - relative to graphics_path



			*/

			gameSettings_->Set("data_path", "data/");
			gameSettings_->Set("world_path", "worlds/");
			gameSettings_->Set("map_path", "maps/");
			gameSettings_->Set("graphics_path", "graphics/");
			gameSettings_->Set("dialogue_path", "dialogue/");
			gameSettings_->Set("battle_path", "battles/");
			gameSettings_->Set("tiles_path", "game/tiles/");
			gameSettings_->Set("game_sprite_path", "game/sprites/");
			gameSettings_->Set("battle_sprite_path", "battle/sprites/");
			gameSettings_->Set("battle_scene_path", "battle/scenes/");
			gameSettings_->Set("font_path", "fonts/");
			gameSettings_->Set("game_overlay_path", "game/overlays/");
			gameSettings_->Set("battle_overlay_path", "battle/overlays/");
			gameSettings_->Set("portrait_path", "portraits/");



			// fonts
			gameSettings_->Set("small_font", "font5x5white.png");
			gameSettings_->Set("small_font_w", "5");
			gameSettings_->Set("small_font_h", "5");
			gameSettings_->Set("large_font", "font8x8white.png");
			gameSettings_->Set("large_font_w", "8");
			gameSettings_->Set("large_font_h", "8");





			gameSettings_->Set("start_world", "game.world");

			gameSettings_->Set("camera_anchor_x", "1");
			gameSettings_->Set("camera_anchor_y", "1");
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

			// cheats haha
			gameSettings_->Set("enable_cheats", "false");
			gameSettings_->Set("enable_cheat_infinite_gold", "false");
			gameSettings_->Set("enable_cheat_infinite_health", "false");
			gameSettings_->Set("enable_cheat_infinite_magic", "false");
			gameSettings_->Set("enable_cheat_max_attack", "false");
			gameSettings_->Set("enable_cheat_max_defense", "false");
			gameSettings_->Set("enable_cheat_max_strength", "false");
			gameSettings_->Set("enable_cheat_no_battles", "false");


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
		if (!smallFont_->Load(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("font_path") +
							gameSettings_->Get("small_font")).c_str(),
				static_cast<int>(atoi(gameSettings_->Get("small_font_w").c_str())),
				static_cast<int>(atoi(gameSettings_->Get("small_font_h").c_str())),
				1))
		{
			return false;
		}

		largeFont_ = new BitmapFont();
		if (!largeFont_->Load(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("font_path") +
							gameSettings_->Get("large_font")).c_str(),
				static_cast<int>(atoi(gameSettings_->Get("large_font_w").c_str())),
				static_cast<int>(atoi(gameSettings_->Get("large_font_h").c_str())),
				1))
		{
			return false;
		}

		// load the over lays
		//windowOverlay_ 	= new ImageResource("data/graphics/game/overlays/mainscreen.png");
		// playerPortrait_ = new ImageResource("data/graphics/portraits/player.png");

		windowOverlay_ = new ImageResource();
		if (!windowOverlay_->Load(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("game_overlay_path") +
							"mainscreen.png").c_str()
			))
		{
			return false;
		}

		playerPortrait_ = new ImageResource();
		if (!playerPortrait_->Load(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("portrait_path") +
							"player.png").c_str()
			))
		{
			return false;
		}

		// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		// lofiOverlay_ 	= new ImageResource("data/graphics/game/overlays/lofi.png");
		lofiOverlay_ = new ImageResource();
		if (!lofiOverlay_->Load(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("game_overlay_path") +
							"lofi.png").c_str()
			))
		{
			return false;
		}
#endif


		// create the dialogue message
		dialogueMessage_ = new GameDialogueMessage();

		// create the display surface we draw on for implementing the cool 400% scale
		int msw  = static_cast<int>(atoi(gameSettings_->Get("micro_screen_width").c_str()));
		int msh  = static_cast<int>(atoi(gameSettings_->Get("micro_screen_height").c_str()));
		microDisplay_ = new ImageResource(
			(msw > 0) ? (msw < sw) ? msw : 200 : 200,
			(msh > 0) ? (msh < sh) ? msh : 150 : 150);

		playerSpriteIndex_ = 0;

		//LoadWorldFile("data/worlds/game.world");
		LoadWorldFile(
			static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("world_path") +
						gameSettings_->Get("start_world")).c_str());


		// create the camera

		int camAX = static_cast<int>(atoi(gameSettings_->Get("camera_anchor_x").c_str()));
		int camAY = static_cast<int>(atoi(gameSettings_->Get("camera_anchor_y").c_str()));

		int camW = static_cast<int>(atoi(gameSettings_->Get("camera_w").c_str()));
		int camH = static_cast<int>(atoi(gameSettings_->Get("camera_h").c_str()));
		int tileW = static_cast<int>(atoi(gameSettings_->Get("tile_w").c_str()));
		int tileH = static_cast<int>(atoi(gameSettings_->Get("tile_h").c_str()));

		// keep the camera size in check
		camW = (camW < 1) ? 17 : (camW > (200 / tileW)) ? 17 : camW;
		camH = (camH < 1) ? 11 : (camH > (150 / tileH)) ? 11 : camH;

		camera_ = new GameCamera(
			camAX, camAY, 	// rendering anchor x, y
			0, 0, 			// world x, y
			camW * tileW, 	// width in pixels
			camH * tileH, 	// height in pixels
			1, 				// camera pan speed
			currentMap_->GetGameMapLayer(0)->GetWidth(),
			currentMap_->GetGameMapLayer(0)->GetHeight()
			);


		// center the camera on the player
		camera_->CenterOnSprite(gameSprites_->Get(playerSpriteIndex_));

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
					MapEditors->SetState(MAPEDITORS::EditingCollisionLayer);
				}
				else
				{
					LogSimpleMessage("Not allowed!");
				}
			}
		}

		gameMenu_ = new GameMenuManager();

		if ("true" == gameSettings_->Get("enable_verbose_startup"))
		{
			gameNPCs_->List(stderr);
		}

		return true;
	}

} // end namespace

