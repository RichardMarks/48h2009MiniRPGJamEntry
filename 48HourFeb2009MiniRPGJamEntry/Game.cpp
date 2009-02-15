
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

	/**************************************************************************/

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

			// test the battle system by pressing F8
#if 1
			if (InputDevice->KeyPressed(KEY::Key_F8))
			{
				if (BattleEngine->Initialize())
				{
					BattleEngine->Execute();
				}
			}
#endif

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

} // end namespace


