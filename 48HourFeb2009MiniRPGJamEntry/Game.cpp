
// CODESTYLE: v2.0

// Game.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the primary game singleton class

#include "GameLibrary.h"

// comment this out to turn off the debugging display
#define ENABLE_DEBUGGING_DISPLAY_INFORMATION

// comment these out to disable in-game editing of maps and events
#define DEV_ENABLE_MAP_EDITING
#define DEV_ENABLE_EVENT_EDITING

#include "GameMap_Editors.h"

namespace GAME
{
	// this is the simplest way to get the game running the same speed across systems
	// so I am using it. fuck that its C, its simple, and works.
	volatile int allegroTimerSpeedCounter = 0;
	void allegroTimerSpeedController(){allegroTimerSpeedCounter++;}END_OF_FUNCTION(allegroTimerSpeedController)

	/**************************************************************************/

	namespace GAMESTATE
	{
		GameStateManager::GameStateManager() :
			state_(World)
		{
		}

		/**********************************************************************/

		GameStateManager::~GameStateManager()
		{
		}

		/**********************************************************************/

		void GameStateManager::SetState(StateType state)
		{
			state_ = state;
		}

		/**********************************************************************/

		StateType GameStateManager::GetState() const
		{
			return state_;
		}

	} // end namespace

	/**************************************************************************/




	GameMenuManager::GameMenuManager() {}
	GameMenuManager::~GameMenuManager() {}
	void GameMenuManager::Update() {}
	void GameMenuManager::Render() {}







	void GameSingleton::SetState(GAMESTATE::StateType state)
	{
		gameStateManager_->SetState(state);
	}

	/**************************************************************************/

	GAMESTATE::StateType GameSingleton::GetState() const
	{
		return gameStateManager_->GetState();
	}

	/**************************************************************************/
















	GameSingleton::GameSingleton() { /* do not put code here */ }

	/**************************************************************************/

	GameSingleton::~GameSingleton() { /* do not put code here */ }

	/**************************************************************************/

	GameSingleton* GameSingleton::GetInstance()
	{
		static GameSingleton instance;
		return &instance;
	}

	/**************************************************************************/

	void GameSingleton::CheckMapEvents(int scanX, int scanY)
	{
		// code for checking and executing map events

		// grab the base layer
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

		// get the event at the tile to scan
		int eventCode = baseLayer->GetEventAt(scanX, scanY);

		// if its an event and not an empty tile or a solid tile
		if (eventCode > 0x0 && eventCode < 0xFF)
		{
			//LogMessage("Found Event %d At %d, %d", eventCode, scanX, scanY);
			// get the event from the map
			GameMapEvent* event = currentMap_->GetMapEvent(eventCode - 1);
			//event->DebugList();

			// if its valid
			if (0 != event)
			{
				//LogMessage("Event is Valid");
				// event->SetLocation(currentMap_->GetID(), scanX, scanY);

				// if its armed
				if (event->IsArmed())
				{
					//LogMessage("Event is Armed");
					// get the event handler
					GameMapEventHandler* eventHandler = event->GetEventHandler();

					// if its valid
					if (0 != eventHandler)
					{
						//LogMessage("Event has a Valid Handler");
						// if it initializes
						if (eventHandler->Initialize(event))
						{
							//LogMessage("Event Handler Initialized");
							// execute the event handler
							eventHandler->Execute();
							//LogMessage("Event Executed");
						}
					}

					// if the event is not a persistent event,
					if (!event->IsPersistent())
					{
						//LogMessage("Disarming the Event");
						// disarm the event
						event->Disarm();
					}
				}
			}
		}
	}

	/**************************************************************************/

	static volatile bool running = true;
	static void CloseButtonHandler()
	{
		running = false;
	}
	END_OF_FUNCTION(CloseButtonHandler)

	/**************************************************************************/

	void GameSingleton::Execute()
	{
		LOCK_FUNCTION(CloseButtonHandler);
		set_close_button_callback(CloseButtonHandler);

		while(running)
		{
			// update the input device info
			InputDevice->Update(UPDATE_KEYBOARD | UPDATE_MOUSE);

			// exit when the user presses ESC
			if (InputDevice->KeyPressed(KEY::Key_Escape))
			{
				running = false;
			}

// test the dialogue system by pressing SPACE
#if 1
			if (InputDevice->KeyPressed(KEY::Key_Space))
			{
				if (GAMESTATE::World == GetState())
				{
					Dialogue("");
				}
			}
#endif


#if defined(DEV_ENABLE_MAP_EDITING)
			if ("true" == gameSettings_->Get("enable_game_editors"))
			{
				if (InputDevice->KeyPressed(KEY::Key_F4))
				{
					if (GAMESTATE::MapEditor != GetState())
					{
						MapEditors->SetMap(currentMap_);
						MapEditors->SetState(MAPEDITORS::EditingCollisionLayer);
						SetState(GAMESTATE::MapEditor);
					}
					else
					{
						SetState(GAMESTATE::World);
					}
				}
			}
#endif



			while (allegroTimerSpeedCounter > 0)
			{
				// depending on the current state of the game, we update different things
				switch(GetState())
				{
#if defined(DEV_ENABLE_MAP_EDITING)
					case GAMESTATE::MapEditor:
					{
						MapEditors->Update();
					} break;
#endif

					case GAMESTATE::World:
					{
						// game world processing takes place

						// if there is game dialogue to process
						if (DIALOGUE::Undefined != dialogueMessage_->GetState())
						{
							// update the dialogue system
							dialogueMessage_->Update(false /* set to true if you want delayed output printing */ );
						}
						else
						{
							// update the main game logic
							Update();

							// handle the 'random' monster ambushes

#if !defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)
#if 1
							stepsUntilAmbush_ -= stepsTaken_;
#endif

							stepsTaken_ = 0;
							if (stepsUntilAmbush_ <= 0)
							{
								// tell the battle system to initialize
								BattleEngine->SetState(BATTLE::Initialize);

								// tell the game to process the battle system on the next update
								SetState(GAMESTATE::Battle);

								// reset ambush step counter
								/** TODO:
								// this should become a random range value based off of
								// the player's current level (higher player's level, the less monsters ambush you)
								// this will make leveling up take longer as the player gets stronger
								// helping to balance out the game
								*/
								stepsUntilAmbush_ 	= 160;

							}
#endif
						}

						// update the game's NPCs
						gameNPCs_->Update(currentMap_->GetName().c_str());
					} break;

					case GAMESTATE::Battle:
					{
						// the external battle system processing takes place

						// update the battle system
						BattleEngine->Update();

						// has the battle finished?
						if (BATTLE::Finished == BattleEngine->GetState())
						{
							// tell the game to process the game world
							SetState(GAMESTATE::World);
						}

					} break;

					case GAMESTATE::Menu:
					{
						// the menu system processing takes place

						// update the menu system
						gameMenu_->Update();

					} break;

					default: break;

				} // end switch

				// decrement the allegro timing var
				allegroTimerSpeedCounter--;

			} // end the update while loop


			// begin rendering
			GraphicsDevice->BeginScene(0);
			microDisplay_->Clear();

			// depending on the current state of the game, we render different things
			switch(GetState())
			{
#if defined(DEV_ENABLE_MAP_EDITING)
				case GAMESTATE::MapEditor:
				{
					MapEditors->Render();
				} break;
#endif
				case GAMESTATE::World:
				{
					// render the main game
					InputDevice->MouseDisplayOnScreen(false);
					Render();
				} break;

				case GAMESTATE::Battle:
				{
					// render the battle system
					BattleEngine->Render();
				} break;

				case GAMESTATE::Menu:
				{
					// render the menu system
					gameMenu_->Render();
				} break;

				default: break;

			} // end switch


			// we are done rendering
			// end the scene using special 4x scaling
			ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();

#if defined(DEV_ENABLE_MAP_EDITING)
			GAMESTATE::StateType state = GetState();

			if (GAMESTATE::MapEditor != state)
			{
#endif
				microDisplay_->Blit(
					display,
					0, 0,
					200, 150,
					0, 0,
					display->GetWidth(), display->GetHeight());

				// debugging display
#if defined(ENABLE_DEBUGGING_DISPLAY_INFORMATION)
				if ("true" == gameSettings_->Get("enable_general_debugging"))
				{
					int camAnchorX 	= 0, camAnchorY = 0;
					int camWidth 	= 0, camHeight 	= 0;
					int camWorldX 	= 0, camWorldY 	= 0;

					camera_->GetSize(camWidth, camHeight);
					camera_->GetWorldPosition(camWorldX, camWorldY);
					camera_->GetAnchorPosition(camAnchorX, camAnchorY);

					int psx = 0, psy = 0; // player screen X, Y coords
					int pwx = 0, pwy = 0; // player world X, Y coords

					// get the player screen position
					gameSprites_->Get(playerSpriteIndex_)->GetScreenPosition(psx, psy);

					// figure the player's world position
					pwx = psx + camWorldX;
					pwy = psy + camWorldY;

					int playerTileX = (pwx + 4) / 8;
					int playerTileY = (pwy + 4) / 8;

					// get the event at the tile to scan

					GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);


					int eventCode = baseLayer->GetEventAt(playerTileX, playerTileY);

					BitmapFont debugFont;
					debugFont.Print(display, 4, 80,     "Current Map: (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
					debugFont.Print(display, 4, 80+8,   "Camera World Pos: %d, %d", camWorldX, camWorldY);
					debugFont.Print(display, 4, 80+8*2, "Player World Pos: %d, %d", pwx, pwy);
					debugFont.Print(display, 4, 80+8*3, "Player Screen Pos: %d, %d", psx, psx);
					debugFont.Print(display, 4, 80+8*4, "Player Tile Pos: %d, %d", playerTileX, playerTileY);
					debugFont.Print(display, 4, 80+8*5, "Event Code @ Player Tile Pos: %d", eventCode);

					debugFont.Print(display, 4, 80+8*6, "Steps Taken: %d", stepsTaken_);
					debugFont.Print(display, 4, 80+8*7, "Steps Until Ambush: %d", stepsUntilAmbush_);
				}
#endif

#if defined(DEV_ENABLE_MAP_EDITING)
			}
#endif




			GraphicsDevice->EndScene();



			// let the cpu rest to keep from using 100% CPU for no damn reason
			rest(10);
		}


		// clean up the battle system
		BattleEngine->Destroy();

		// clean up the map editors
		MapEditors->Destroy();

		// the game is over, cleanup after our game class -- release pointers, etc
		Destroy();

	}

	/**************************************************************************/

	void GameSingleton::ChangeMap(GameMap* map)
	{
		currentMap_ = map;
	}

	/**************************************************************************/

	GameTilesetManager* GameSingleton::GetTilesetManager() const
	{
		return gameTiles_;
	}

	/**************************************************************************/

	GameMapManager* GameSingleton::GetMapManager() const
	{
		return gameMaps_;
	}

	/**************************************************************************/

	GameMapSpriteManager* GameSingleton::GetMapSpriteManager() const
	{
		return gameSprites_;
	}

	/**************************************************************************/

	GameNPCManager* GameSingleton::GetNPCManager() const
	{
		return gameNPCs_;
	}

	/**************************************************************************/

	int GameSingleton::GetPlayerSpriteIndex() const
	{
		return playerSpriteIndex_;
	}

	/**************************************************************************/

	GameMap* GameSingleton::GetMap() const
	{
		return currentMap_;
	}

	/**************************************************************************/

	GameCamera* GameSingleton::GetCamera() const
	{
		return camera_;
	}

	/**************************************************************************/

	BitmapFont* GameSingleton::GetSmallFont() const
	{
		return smallFont_;
	}

	/**************************************************************************/

	BitmapFont* GameSingleton::GetLargeFont() const
	{
		return largeFont_;
	}

	/**************************************************************************/

	ImageResource* GameSingleton::GetDisplay() const
	{
		return microDisplay_;
	}

	/**************************************************************************/

	void GameSingleton::SetSetting(const char* setting, const char* value)
	{
		gameSettings_->Set(setting, value);
	}

	/**************************************************************************/

	std::string GameSingleton::GetSetting(const char* setting)
	{
		return gameSettings_->Get(setting);
	}

	/**************************************************************************/

	Settings* GameSingleton::GetSettings() const
	{
		return gameSettings_;
	}

	/**************************************************************************/

} // end namespace
