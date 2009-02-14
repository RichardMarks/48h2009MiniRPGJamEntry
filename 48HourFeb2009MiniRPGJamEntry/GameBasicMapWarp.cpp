
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
		// GameMapSprite* playerSprite = game->GetPlayerObject()->GetSprite();
		
		GameMapSprite* playerSprite = game->GetMapSpriteManager()->Get(game->GetPlayerSpriteIndex());
		
		GameCamera* camera 			= game->GetCamera();
		
		int camWidth = 0, camHeight = 0;
		int fromX = 0, fromY = 0, fromMap = 0, toX = 0, toY = 0, toMap = 0;
		//int psx = 0, psy = 0; // player screen X, Y coords
		
		//GameMapLayer* baseLayer = currentMap->GetGameMapLayer(0);

		//int mapWidth 	= baseLayer->GetWidth();
		//int mapHeight 	= baseLayer->GetHeight();
		
		event_->GetLocation(fromMap, fromX, fromY);
		event_->GetWarpTargetLocation(toMap, toX, toY);
		
		#if 0
		fprintf(stderr,"warp from map %d @ position %d, %d to map %d @ position %d, %d\n",
			fromMap, fromX, fromY, toMap, toX, toY);
		#endif
		
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


			camera->CenterOnSprite(playerSprite);
			
// disabling to test new camera code
#if 0
			playerSprite->SetScreenPosition(camWidth / 2, camHeight / 2);
				
			// pre-calculate the new camera position
			// with the player centered in the middle
			int camToX = (toX * 8) - camWidth / 2;
			int camToY = (toY * 8) - camHeight / 2;
		
			// handle the camera going off the left edge of the map
			if (camToX < 0)
			{
				int offsetX = abs(camToX);
		
				// get the player screen position 
				playerSprite->GetScreenPosition(psx, psy);
	
				// offset player's screen position
				psx = psx - offsetX;
	
				// set the player's screen position
				playerSprite->SetScreenPosition(psx, psy);
			
				#if 0
				fprintf(stderr, 
					"camera moved off left by %d pixels\n"
					"camToX = %d\npsx = (before)%d (after)%d\n", offsetX,
					camToX, psx + offsetX, psx);
				#endif
				
				// set the camera on the edge
				camToX = 0;
			}
		
			// handle the camera going off the top edge of the map
			if (camToY < 0)
			{
				int offsetY = abs(camToY);
				
				//fprintf(stderr, "camera moved off top by %d pixels\n", offsetY);
		
				// get the player screen position 
				playerSprite->GetScreenPosition(psx, psy);
	
				// offset player's screen position
				psy = psy - offsetY;
	
				// set the player's screen position
				playerSprite->SetScreenPosition(psx, psy);
			
				// set the camera on the edge
				camToY = 0;
			}
		
			// handle the camera going off the right edge of the map
			if (camToX + camWidth > mapWidth)
			{
				int offsetX = abs(mapWidth - (camToX + camWidth));
		
				//fprintf(stderr, "camera moved off right by %d pixels\n", offsetX);
				
				// get the player screen position 
				playerSprite->GetScreenPosition(psx, psy);
	
				// offset player's screen position
				psx = psx + offsetX;
	
				// set the player's screen position
				playerSprite->SetScreenPosition(psx, psy);
			
				// set the camera on the edge
				camToX = mapWidth - camWidth;
			}
			
			// handle the camera going off the bottom edge of the map
			if (camToY + camHeight > mapHeight)
			{
				int offsetY = abs(mapHeight - (camToY + camHeight));
				
			//	fprintf(stderr, "camera moved off bottom by %d pixels\n", offsetY);
		
				// get the player screen position 
				playerSprite->GetScreenPosition(psx, psy);
	
				// offset player's screen position
				psy = psy + offsetY;
	
				// set the player's screen position
				playerSprite->SetScreenPosition(psx, psy);
			
				// set the camera on the edge
				camToY = mapHeight - camHeight;
			}
		
			// finally move the camera
			camera->SetWorldPosition(camToX, camToY);
#endif		
			// update the game
			//game->Update();
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


