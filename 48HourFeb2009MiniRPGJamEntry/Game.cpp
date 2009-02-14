
// CODESTYLE: v2.0

// Game.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the primary game singleton class

#include "GameLibrary.h"

namespace GAME
{
	// this is the simplest way to get the game running the same speed across systems
	// so I am using it. fuck that its C, its simple, and works.
	volatile int allegroTimerSpeedCounter = 0;
	void allegroTimerSpeedController(){allegroTimerSpeedCounter++;}END_OF_FUNCTION(allegroTimerSpeedController)

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

	void GameSingleton::Execute()
	{
		LOCK_FUNCTION(CloseButtonHandler);
		set_close_button_callback(CloseButtonHandler);

		while(running)
		{
			// update the input device info
			InputDevice->Update(UPDATE_KEYBOARD);

			// exit when the user presses ESC
			if (InputDevice->KeyPressed(KEY::Key_Escape))
			{
				running = false;
			}

			if (InputDevice->KeyPressed(KEY::Key_F8))
			{
				if (BattleEngine->Initialize())
				{
					BattleEngine->Execute();
				}
			}

			while (allegroTimerSpeedCounter > 0)
			{
				Update();

				gameNPCs_->Update(currentMap_->GetName().c_str());

				allegroTimerSpeedCounter--;
			}

			Render();

			// let the cpu rest to keep from using 100% CPU for no damn reason
			rest(10);
		}

		Destroy();
	}

	/**************************************************************************/

	GameTilesetManager* GameSingleton::GetTilesetManager() const { return gameTiles_; } // get a pointer to the tileset manager
	GameMapManager* GameSingleton::GetMapManager() const { return gameMaps_; } // get a pointer to the map manager

	GameMapSpriteManager* GameSingleton::GetMapSpriteManager() const { return gameSprites_; } // get a pointer to the map sprite manager
	GameNPCManager* GameSingleton::GetNPCManager() const { return gameNPCs_; } // get a pointer to the npc manager

	int GameSingleton::GetPlayerSpriteIndex() const { return playerSpriteIndex_; } // get the player sprite index value

	GameMap* GameSingleton::GetMap() const { return currentMap_; } // get a pointer to the current map
	// GameObject* GameSingleton::GetPlayerObject() const { return playerObject_; } // get a pointer to the player
	GameCamera* GameSingleton::GetCamera() const { return camera_; } // get a pointer to the camera
	BitmapFont* GameSingleton::GetSmallFont() const { return smallFont_; } // get a pointer to the small font
	BitmapFont* GameSingleton::GetLargeFont() const { return largeFont_; } // get a pointer to the large font
	ImageResource* GameSingleton::GetDisplay() const { return microDisplay_; } // get a pointer to the micro display

	void GameSingleton::ChangeMap(GameMap* map)
	{
		currentMap_ = map;
	}

	/**************************************************************************/

	#if 1

	void GameSingleton::InitializeNPCs()
	{
		npcObject_ = new GameObject("data/graphics/game/sprites/npc_adultm.png", 8, 8, 3, 15);

		GameMapSprite* npcSprite = npcObject_->GetSprite();

		npcSprite->SetWorldPosition(8 * 18, 8 * 6);
		npcSprite->SetFaceDirection(MAPSPRITE::WALK_SOUTH_FRAME);
		npcSprite->Animate();

	}

	/**************************************************************************/

	void GameSingleton::UpdateNPCs()
	{
		npcObject_->Update();
	}

	/**************************************************************************/

	void GameSingleton::RenderNPCs()
	{

		// get player's field of view (camera)
		int camWidth = 0, camHeight = 0;
		int camWorldX = 0, camWorldY = 0;

		camera_->GetSize(camWidth, camHeight);
		camera_->GetWorldPosition(camWorldX, camWorldY);

		int fov[] = /* left, top, right, and bottom collision planes */
		{
			camWorldX,
			camWorldY,
			camWorldX + camWidth,
			camWorldY + camHeight
		};

		// if the npc is within the field of view
		int npcsx = 0, npcsy = 0;
		int npcwx = 0, npcwy = 0;
		GameMapSprite* npcSprite = npcObject_->GetSprite();

		npcSprite->GetWorldPosition(npcwx, npcwy);

		if (npcwx >= fov[0] && npcwx <= fov[2] && npcwy >= fov[1] && npcwy <= fov[3])
		{
			// figure the screen position
			npcsx = npcwx - fov[0];
			npcsy = npcwy - fov[1];
			// set the screen position
			npcSprite->SetScreenPosition(npcsx, npcsy);
			// draw the npc
			npcObject_->Render(microDisplay_);
		}

	}

	#endif

} // end namespace


