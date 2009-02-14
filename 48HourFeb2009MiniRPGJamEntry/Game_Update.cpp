
// CODESTYLE: v2.0

// Game_Update.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Update method for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	void GameSingleton::Update()
	{
		// GameMapSprite* playerSprite = playerObject_->GetSprite();
		GameMapSprite* playerSprite = gameSprites_->Get(playerSpriteIndex_);
		
		int psx = 0, psy = 0; // player screen X, Y coords
		int pwx = 0, pwy = 0; // player world X, Y coords
		
		int camAnchorX = 0, camAnchorY = 0;
		int camWidth = 0, camHeight = 0;
		int camWorldX = 0, camWorldY = 0;
		
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);
		
		
		int mapWidth 	= baseLayer->GetWidth();
		int mapHeight 	= baseLayer->GetHeight();
		int mapRows 	= baseLayer->GetNumRows();
		int mapColumns 	= baseLayer->GetNumColumns();
		
		
		camera_->GetSize(camWidth, camHeight);
		camera_->GetWorldPosition(camWorldX, camWorldY);
		camera_->GetAnchorPosition(camAnchorX, camAnchorY);

		// get the player screen position 
		playerSprite->GetScreenPosition(psx, psy);
		
		// figure the player's world position
		pwx = psx + camWorldX;
		pwy = psy + camWorldY;
		
		// set the player's world position
		playerSprite->SetWorldPosition(pwx, pwy);
	
	
	
		// get the collision data surrounding the player
		/*
			[0] - left of the player
			[1] - above the player
			[2] - right of the player
			[3] - below the player
		*/
		int playerTileX = (pwx + 4) / 8;
		int playerTileY = (pwy + 4) / 8;
		int collide[] =
		{
			(baseLayer->GetEventAt((playerTileX > 0) ? playerTileX - 1 : 0, playerTileY) == 0xFF) ? true : false,
			(baseLayer->GetEventAt(playerTileX, (playerTileY > 0) ? playerTileY - 1 : 0) == 0xFF) ? true : false,
			(baseLayer->GetEventAt((playerTileX < mapColumns - 1) ? playerTileX + 1 : mapColumns - 1, playerTileY) == 0xFF) ? true : false,
			(baseLayer->GetEventAt(playerTileX, (playerTileY < mapRows - 1) ? playerTileY + 1 : mapRows - 1) == 0xFF) ? true : false
		};
		
		// handle events
		// CheckMapEvents(playerTileX, playerTileY);
	
		#define _TMP_CHECKFORMAPEVENTS \
			camera_->GetWorldPosition(camWorldX, camWorldY); \
			pwx = psx + camWorldX; \
			pwy = psy + camWorldY; \
			playerSprite->SetWorldPosition(pwx, pwy); \
			playerTileX = (pwx + 4) / 8; \
			playerTileY = (pwy + 4) / 8; \
			CheckMapEvents(playerTileX, playerTileY);
	
		// handle scrolling / moving the player
		
		if (InputDevice->KeyDown(KEY::Key_Left))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_WEST_FRAME);
			playerSprite->Animate();
			
			if (psx > camWidth / 2)
			{
				if (!collide[0]/*1==1*/ /* replace with: if tile is not solid left of the player's world position */)
				{
					psx--;
					playerSprite->SetScreenPosition(psx, psy);
					_TMP_CHECKFORMAPEVENTS
				} // end if
			} // end if
			else
			{
				if (camWorldX > 0)
				{
					if (!collide[0]/*1==1*/ /* replace with: if tile is not solid left of the player's world position */)
					{
						camera_->Pan(CAMERA::West);
						_TMP_CHECKFORMAPEVENTS
					} // end if
				} // end if
				else
				{
					if (psx > 0)
					{
						if (!collide[0]/*1==1*/ /* replace with: if tile is not solid left of the player's world position */)
						{
							psx--;
							playerSprite->SetScreenPosition(psx, psy);
							_TMP_CHECKFORMAPEVENTS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end if
		else if (InputDevice->KeyDown(KEY::Key_Right))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_EAST_FRAME);
			playerSprite->Animate();
			
			if (psx < camWidth / 2)
			{
				if (!collide[2]/*1==1*/ /* replace with: if tile is not solid right of the player's world position */)
				{
					psx++;
					playerSprite->SetScreenPosition(psx, psy);
					_TMP_CHECKFORMAPEVENTS
				} // end if
			} // end if
			else
			{
				if (camWorldX < mapWidth - camWidth)
				{
					if (!collide[2]/*1==1*/ /* replace with: if tile is not solid right of the player's world position */)
					{
						camera_->Pan(CAMERA::East);
						_TMP_CHECKFORMAPEVENTS
					} // end if
				} // end if
				else
				{
					if (psx < camWidth-8)
					{
						if (!collide[2]/*1==1*/ /* replace with: if tile is not solid right of the player's world position */)
						{
							psx++;
							playerSprite->SetScreenPosition(psx, psy);
							_TMP_CHECKFORMAPEVENTS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end else if
		else if (InputDevice->KeyDown(KEY::Key_Up))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_NORTH_FRAME);
			playerSprite->Animate();
			
			if (psy > camHeight / 2)
			{
				if (!collide[1]/*1==1*/ /* replace with: if tile is not solid above the player's world position */)
				{
					psy--;
					playerSprite->SetScreenPosition(psx, psy);
					_TMP_CHECKFORMAPEVENTS
				} // end if
			} // end if
			else
			{
				if (camWorldY > 0)
				{
					if (!collide[1]/*1==1*/ /* replace with: if tile is not solid above the player's world position */)
					{
						camera_->Pan(CAMERA::North);
						_TMP_CHECKFORMAPEVENTS
					} // end if
				} // end if
				else
				{
					if (psy > 0)
					{
						if (!collide[1]/*1==1*/ /* replace with: if tile is not solid above the player's world position */)
						{
							psy--;
							playerSprite->SetScreenPosition(psx, psy);
							_TMP_CHECKFORMAPEVENTS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end else if
		else if (InputDevice->KeyDown(KEY::Key_Down))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_SOUTH_FRAME);
			playerSprite->Animate();
			
			if (psy < camHeight / 2)
			{
				if (!collide[3]/*1==1*/ /* replace with: if tile is not solid below the player's world position */)
				{
					psy++;
					playerSprite->SetScreenPosition(psx, psy);
					_TMP_CHECKFORMAPEVENTS
				} // end if
			} // end if
			else
			{
				if (camWorldY < mapHeight - camHeight)
				{
					if (!collide[3]/*1==1*/ /* replace with: if tile is not solid below the player's world position */)
					{
						camera_->Pan(CAMERA::South);
						_TMP_CHECKFORMAPEVENTS
					} // end if
				} // end if
				else
				{
					if (psy < camHeight-8)
					{
						if (!collide[3]/*1==1*/ /* replace with: if tile is not solid below the player's world position */)
						{
							psy++;
							playerSprite->SetScreenPosition(psx, psy);
							_TMP_CHECKFORMAPEVENTS
						} // end if
					} // end if
				} // end else
			}// end else
		} // end else if
		
		#undef _TMP_CHECKFORMAPEVENTS
		
		if (
			InputDevice->KeyUp(KEY::Key_Up) && InputDevice->KeyUp(KEY::Key_Down) &&
			InputDevice->KeyUp(KEY::Key_Left) && InputDevice->KeyUp(KEY::Key_Right)
			)
		{
			// stop animating the player since we are not moving
			playerSprite->Animate(false);
		}
		
		// update player sprite
		playerSprite->Update();
	}
	
} // end namespace

