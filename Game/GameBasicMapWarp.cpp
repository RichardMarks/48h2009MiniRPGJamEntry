
// CODESTYLE: v2.0

// GameBasicMapWarp.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: a simple map to map transition event handler singleton class

#include "GameLibrary.h"

namespace GAME
{

	GameBasicMapWarp* GameBasicMapWarp::GetInstance()
	{
		static GameBasicMapWarp instance;
		return &instance;
	}

	/**************************************************************************/

	GameBasicMapWarp::GameBasicMapWarp()
	{
	}

	/**************************************************************************/

	GameBasicMapWarp::~GameBasicMapWarp()
	{
	}

	/**************************************************************************/

	bool GameBasicMapWarp::Initialize(GameMapEvent* event)
	{
		if (!event) { return false; }
		event_ = new GameMapEvent();
		if (!event_) { return false; }
		event_->CloneFrom(event);
		return true;
	}

	/**************************************************************************/

	void GameBasicMapWarp::Execute()
	{
		GameSingleton* game 		= GameSingleton::GetInstance();
		GameMap* currentMap 		= game->GetMap();
		GameMapSprite* playerSprite = game->GetMapSpriteManager()->Get(game->GetPlayerSpriteIndex());
		GameCamera* camera 			= game->GetCamera();

		int camWidth = 0, camHeight = 0;
		int fromX = 0, fromY = 0, fromMap = 0, toX = 0, toY = 0, toMap = 0;

		event_->GetLocation(fromMap, fromX, fromY);
		event_->GetWarpTargetLocation(toMap, toX, toY);

		#if 0
		fprintf(stderr,"warp from map %d @ position %d, %d to map %d @ position %d, %d\n",
			fromMap, fromX, fromY, toMap, toX, toY);
		#endif

		int playerFace;
		playerSprite->GetFaceDirection(playerFace);
		switch(playerFace)
		{
			case MAPSPRITE::WALK_NORTH_FRAME: { toY--; } break;
			case MAPSPRITE::WALK_SOUTH_FRAME: { toY++; } break;
			case MAPSPRITE::WALK_EAST_FRAME:  { toX++; } break;
			case MAPSPRITE::WALK_WEST_FRAME:  { toX--; } break;
		}

		// change the map if its valid
		GameMap* newMap = game->GetMapManager()->Get(toMap);

		if (0 != newMap)
		{
			#if 0
			currentMap->DebugList();
			fprintf(stderr, "changing map *****************************************************************\n");
			game->ChangeMap(newMap);
			currentMap = game->GetMap();
			currentMap->DebugList();
			#endif

			// fade to black
#if 0
			{
				ImageResource* scrBuf = GraphicsDevice->GetPrimaryDisplayBuffer();
				ImageResource fadeScreen(scrBuf->GetWidth(), scrBuf->GetHeight());
				int fade = 0;
				while(fade < 100)
				{
					fadeScreen.BlitAlpha(scrBuf, 0, 0, 0.05f);
					fade++;
				}
			}
#endif


			// change the map
			game->ChangeMap(newMap);
			currentMap = game->GetMap();

			// update the camera world size
			camera->SetWorldSize(
				currentMap->GetGameMapLayer(0)->GetWidth(),
				currentMap->GetGameMapLayer(0)->GetHeight());

			// get the size of the camera
			camera->GetSize(camWidth, camHeight);

			// move the player's world and screen position
			playerSprite->SetWorldPosition(toX * 8, toY * 8);

			// center the camera on the player
			camera->CenterOnSprite(playerSprite);
		}

		// we are done with this event
		if (event_)
		{
			delete event_;
		}

		#if 0
		else
		{
			fprintf(stderr, "invalid map id %d\n", toMap);
		}
		#endif
	}

	/**************************************************************************/

	void GameBasicMapWarp::Destroy()
	{
		if (event_) { delete event_; }
	}

} // end namespace


