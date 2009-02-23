
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

		bool collide[] =
		{
			currentMap_->IsSolid((playerTileX > 0) ? playerTileX - 1 : 0, playerTileY),
			currentMap_->IsSolid(playerTileX, (playerTileY > 0) ? playerTileY - 1 : 0),
			currentMap_->IsSolid((playerTileX < mapColumns - 1) ? playerTileX + 1 : mapColumns - 1, playerTileY),
			currentMap_->IsSolid(playerTileX, (playerTileY < mapRows - 1) ? playerTileY + 1 : mapRows - 1),
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

		#define _TMP_CHECKFORWARPS \
			if (currentMap_->IsWarp(playerTileX, playerTileY)) { WarpTargetPair* pair = \
			currentMap_->GetWarpTargetPair(currentMap_->GetWarp(playerTileX, playerTileY)); \
			GameMapEvent warpEvent(pair->startX_, pair->startY_, pair->startMap_, \
			GameBasicMapWarp::GetInstance(), true, true, "", pair->endX_, pair->endY_, \
			pair->endMap_); if (GameBasicMapWarp::GetInstance()->Initialize(&warpEvent)) { \
			GameBasicMapWarp::GetInstance()->Execute(); }}

#if 0
//##############################################################################
/// the check for warps code explained
			// if the tile that the player is standing on is a warp tile
			if (currentMap_->IsWarp(playerTileX, playerTileY))
			{
				// get the warp target pair data
				WarpTargetPair* pair = currentMap_->GetWarpTargetPair(currentMap_->GetWarp(playerTileX, playerTileY));

				// create a map warp event init with the warp target pair data
				GameMapEvent warpEvent(
					pair->startX_, pair->startY_, pair->startMap_,
					GameBasicMapWarp::GetInstance(), true, true, "",
					pair->endX_, pair->endY_, pair->endMap_);

				// if we initialize the event handler with the event successfully
				if (GameBasicMapWarp::GetInstance()->Initialize(&warpEvent))
				{
					// then we execute the event handler to perform our map warp
					GameBasicMapWarp::GetInstance()->Execute();
				}
			}
//##############################################################################

#endif


		// handle player / NPC interactions

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
		// reset the NPC data if it exists
		if (npcInteractionData_.npcBusy_)
		{
			gameNPCs_->Get(npcInteractionData_.npcIndex_)->GetSprite()->SetFaceDirection(npcInteractionData_.npcFacing_);
			gameNPCs_->Resume(npcInteractionData_.npcIndex_);

			npcInteractionData_.npcBusy_ 	= false;
			npcInteractionData_.npcIndex_ 	= 0;
			npcInteractionData_.npcFacing_ 	= MAPSPRITE::WALK_NORTH_FRAME;

		}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
		if (InputDevice->KeyPressed(KEY::Key_Enter))
		{
			// based on the direction the player is facing
			int playerFacing;
			playerSprite->GetFaceDirection(playerFacing);
			switch(playerFacing)
			{
				case MAPSPRITE::WALK_NORTH_FRAME:
				{
					// check for an NPC above the player
					if (playerTileY - 1 >= 0)
					{
						int npc = CheckNPCCollisions(playerTileX, playerTileY - 1);
						if (-1 != npc)
						{
							// there is an NPC here
							fprintf(stderr, "Found NPC #%d at tile %d, %d\n", npc, playerTileX, playerTileY - 1);

							npcInteractionData_.npcIndex_ = static_cast<unsigned int>(npc);
							npcInteractionData_.npcBusy_ = true;

							// pause it so that its not running away from us!
							gameNPCs_->Pause(npc);

							// make it face the player (south)
							gameNPCs_->Get(npc)->GetSprite()->GetFaceDirection(npcInteractionData_.npcFacing_);
							gameNPCs_->Get(npc)->GetSprite()->SetFaceDirection(MAPSPRITE::WALK_SOUTH_FRAME);

							// do dialogue

						}
					}
				} break;

				case MAPSPRITE::WALK_SOUTH_FRAME:
				{
					// check for an NPC below the player
					if (playerTileY + 1 < mapColumns)
					{
						int npc = CheckNPCCollisions(playerTileX, playerTileY + 1);
						if (-1 != npc)
						{
							// there is an NPC here
							fprintf(stderr, "Found NPC #%d at tile %d, %d\n", npc, playerTileX, playerTileY + 1);

							npcInteractionData_.npcIndex_ = static_cast<unsigned int>(npc);
							npcInteractionData_.npcBusy_ = true;

							// pause it so that its not running away from us!
							gameNPCs_->Pause(npc);

							// make it face the player (north)
							gameNPCs_->Get(npc)->GetSprite()->GetFaceDirection(npcInteractionData_.npcFacing_);
							gameNPCs_->Get(npc)->GetSprite()->SetFaceDirection(MAPSPRITE::WALK_NORTH_FRAME);

							// do dialogue

						}
					}
				} break;

				case MAPSPRITE::WALK_EAST_FRAME:
				{
					// check for an NPC east of the player
					if (playerTileX + 1 < mapColumns)
					{
						int npc = CheckNPCCollisions(playerTileX + 1, playerTileY);
						if (-1 != npc)
						{
							// there is an NPC here
							fprintf(stderr, "Found NPC #%d at tile %d, %d\n", npc, playerTileX + 1, playerTileY);

							npcInteractionData_.npcIndex_ = static_cast<unsigned int>(npc);
							npcInteractionData_.npcBusy_ = true;

							// pause it so that its not running away from us!
							gameNPCs_->Pause(npc);

							// make it face the player (west)
							gameNPCs_->Get(npc)->GetSprite()->GetFaceDirection(npcInteractionData_.npcFacing_);
							gameNPCs_->Get(npc)->GetSprite()->SetFaceDirection(MAPSPRITE::WALK_WEST_FRAME);

							// do dialogue

						}
					}
				} break;

				case MAPSPRITE::WALK_WEST_FRAME:
				{
					// check for an NPC west of the player
					if (playerTileX - 1 >= 0)
					{
						int npc = CheckNPCCollisions(playerTileX - 1, playerTileY);
						if (-1 != npc)
						{
							// there is an NPC here
							fprintf(stderr, "Found NPC #%d at tile %d, %d\n", npc, playerTileX - 1, playerTileY);

							npcInteractionData_.npcIndex_ = static_cast<unsigned int>(npc);
							npcInteractionData_.npcBusy_ = true;

							// pause it so that its not running away from us!
							gameNPCs_->Pause(npc);

							// make it face the player (east)
							gameNPCs_->Get(npc)->GetSprite()->GetFaceDirection(npcInteractionData_.npcFacing_);
							gameNPCs_->Get(npc)->GetSprite()->SetFaceDirection(MAPSPRITE::WALK_EAST_FRAME);

							// do dialogue

						}
					}
				} break;

				default: break;
			}// end switch
		}

		// handle scrolling / moving the player

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


		if (InputDevice->KeyDown(KEY::Key_Left))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_WEST_FRAME);
			playerSprite->Animate();

			if (psx > camWidth / 2)
			{
				if (!collide[0])
				{
					psx--;
					playerSprite->SetScreenPosition(psx, psy);
					stepsTaken_++;

					if (CheckNPCCollisions())
					{
						psx++;
						playerSprite->SetScreenPosition(psx, psy);
					}

					_TMP_CHECKFORMAPEVENTS
					_TMP_CHECKFORWARPS
				} // end if
			} // end if
			else
			{
				if (camWorldX > 0)
				{
					if (!collide[0])
					{
						camera_->Pan(CAMERA::West);
						stepsTaken_++;

						if (CheckNPCCollisions())
						{
							camera_->Pan(CAMERA::East);
						}

						_TMP_CHECKFORMAPEVENTS
						_TMP_CHECKFORWARPS
					} // end if
				} // end if
				else
				{
					if (psx > 0)
					{
						if (!collide[0])
						{
							psx--;
							playerSprite->SetScreenPosition(psx, psy);
							stepsTaken_++;

							if (CheckNPCCollisions())
							{
								psx++;
								playerSprite->SetScreenPosition(psx, psy);
							}

							_TMP_CHECKFORMAPEVENTS
							_TMP_CHECKFORWARPS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end if

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		else if (InputDevice->KeyDown(KEY::Key_Right))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_EAST_FRAME);
			playerSprite->Animate();

			if (psx < camWidth / 2)
			{
				if (!collide[2])
				{
					psx++;
					playerSprite->SetScreenPosition(psx, psy);
					stepsTaken_++;

					if (CheckNPCCollisions())
					{
						psx--;
						playerSprite->SetScreenPosition(psx, psy);
					}

					_TMP_CHECKFORMAPEVENTS
					_TMP_CHECKFORWARPS
				} // end if
			} // end if
			else
			{
				if (camWorldX < mapWidth - camWidth)
				{
					if (!collide[2])
					{
						camera_->Pan(CAMERA::East);
						stepsTaken_++;

						if (CheckNPCCollisions())
						{
							camera_->Pan(CAMERA::West);
						}

						_TMP_CHECKFORMAPEVENTS
						_TMP_CHECKFORWARPS
					} // end if
				} // end if
				else
				{
					if (psx < camWidth-8)
					{
						if (!collide[2])
						{
							psx++;
							playerSprite->SetScreenPosition(psx, psy);
							stepsTaken_++;

							if (CheckNPCCollisions())
							{
								psx--;
								playerSprite->SetScreenPosition(psx, psy);
							}

							_TMP_CHECKFORMAPEVENTS
							_TMP_CHECKFORWARPS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end else if

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		else if (InputDevice->KeyDown(KEY::Key_Up))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_NORTH_FRAME);
			playerSprite->Animate();

			if (psy > camHeight / 2)
			{
				if (!collide[1])
				{
					psy--;
					playerSprite->SetScreenPosition(psx, psy);
					stepsTaken_++;

					if (CheckNPCCollisions())
					{
						psy++;
						playerSprite->SetScreenPosition(psx, psy);
					}

					_TMP_CHECKFORMAPEVENTS
					_TMP_CHECKFORWARPS
				} // end if
			} // end if
			else
			{
				if (camWorldY > 0)
				{
					if (!collide[1])
					{
						camera_->Pan(CAMERA::North);
						stepsTaken_++;

						if (CheckNPCCollisions())
						{
							camera_->Pan(CAMERA::South);
						}

						_TMP_CHECKFORMAPEVENTS
						_TMP_CHECKFORWARPS
					} // end if
				} // end if
				else
				{
					if (psy > 0)
					{
						if (!collide[1])
						{
							psy--;
							playerSprite->SetScreenPosition(psx, psy);
							stepsTaken_++;

							if (CheckNPCCollisions())
							{
								psy++;
								playerSprite->SetScreenPosition(psx, psy);
							}

							_TMP_CHECKFORMAPEVENTS
							_TMP_CHECKFORWARPS
						} // end if
					} // end if
				} // end else
			} // end else
		} // end else if

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		else if (InputDevice->KeyDown(KEY::Key_Down))
		{
			playerSprite->SetFaceDirection(MAPSPRITE::WALK_SOUTH_FRAME);
			playerSprite->Animate();

			if (psy < camHeight / 2)
			{
				if (!collide[3])
				{
					psy++;
					playerSprite->SetScreenPosition(psx, psy);
					stepsTaken_++;

					if (CheckNPCCollisions())
					{
						psy--;
						playerSprite->SetScreenPosition(psx, psy);
					}

					_TMP_CHECKFORMAPEVENTS
					_TMP_CHECKFORWARPS
				} // end if
			} // end if
			else
			{
				if (camWorldY < mapHeight - camHeight)
				{
					if (!collide[3])
					{
						camera_->Pan(CAMERA::South);
						stepsTaken_++;

						if (CheckNPCCollisions())
						{
							camera_->Pan(CAMERA::North);
						}

						_TMP_CHECKFORMAPEVENTS
						_TMP_CHECKFORWARPS
					} // end if
				} // end if
				else
				{
					if (psy < camHeight-8)
					{
						if (!collide[3])
						{
							psy++;
							playerSprite->SetScreenPosition(psx, psy);
							stepsTaken_++;

							if (CheckNPCCollisions())
							{
								psy--;
								playerSprite->SetScreenPosition(psx, psy);
							}

							_TMP_CHECKFORMAPEVENTS
							_TMP_CHECKFORWARPS
						} // end if
					} // end if
				} // end else
			}// end else
		} // end else if

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

		#undef _TMP_CHECKFORMAPEVENTS
		#undef _TMP_CHECKFORWARPS

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

