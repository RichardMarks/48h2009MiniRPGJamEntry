
// CODESTYLE: v2.0

// GameNPC.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines an NPC

#include "GameLibrary.h"

namespace GAME
{
	GameNPC::GameNPC(GameMapSprite* sprite, int speed, const char* motionData) :
		sprite_(sprite),
		speed_(speed),
		motionData_(motionData),
		motionDataCounter_(0),
		walking_(false),
		walkingDirection_(0),
		distanceWalked_(0),
		framesIdle_(0),
		handlingMotion_(false),
		logicTimer_(1)
	{
	}
	
	/**************************************************************************/
	
	GameNPC::~GameNPC()
	{
		motionData_ = "";
	}
	
	/**************************************************************************/
	
	void GameNPC::Update()
	{
		sprite_->Update();
		
		if (0 == --logicTimer_)
		{
			sprite_->GetFrameDelay(logicTimer_); logicTimer_ /= 2;
		
			// do we have special motion data?
			if (motionData_.size())
			{
				// handle it
			
				// if we are not already handling a motion, then lets get the next motion
				if (!handlingMotion_)
				{
					// run through the motion data counter
					if (static_cast<unsigned int>(++motionDataCounter_) > motionData_.size()-1)
					{
						motionDataCounter_ = 0;
					}
			
					// handle the current motion data
					char currentMotion = motionData_.at(motionDataCounter_);
			
					//fprintf(stderr, "Current Motion: %c\n", currentMotion);
			
					switch(currentMotion)
					{
					// face direction changes
					case 'N': { sprite_->SetFaceDirection(MAPSPRITE::WALK_NORTH_FRAME); } break;
					case 'S': { sprite_->SetFaceDirection(MAPSPRITE::WALK_SOUTH_FRAME); } break;
					case 'W': { sprite_->SetFaceDirection(MAPSPRITE::WALK_EAST_FRAME); } break;
					case 'E': { sprite_->SetFaceDirection(MAPSPRITE::WALK_WEST_FRAME); } break;
			
					// walking directions
					case 'U': { distanceWalked_ = 0; walking_ = true; walkingDirection_ = 0; handlingMotion_ = true; } break;
					case 'D': { distanceWalked_ = 0; walking_ = true; walkingDirection_ = 1; handlingMotion_ = true; } break;
					case 'L': { distanceWalked_ = 0; walking_ = true; walkingDirection_ = 2; handlingMotion_ = true; } break;
					case 'R': { distanceWalked_ = 0; walking_ = true; walkingDirection_ = 3; handlingMotion_ = true; } break;
			
					// animation handlers
					case '@': { sprite_->Animate(true); } break;
					case '_': { sprite_->Animate(false); } break;
					case '!': { framesIdle_ = 0; walking_ = false; handlingMotion_ = true; } break;
			
					default: break;
					}
				}
				else
				{
					// we are handling a motion
				
					if (walking_)
					{
						// we are walking
						if (++distanceWalked_ > 8)
						{
							walking_ = 0;
							distanceWalked_ = 0;
							handlingMotion_ = false;
						}
					
						int worldX = 0, worldY = 0;
						sprite_->GetWorldPosition(worldX, worldY);
						
						int spriteTileX = (worldX + 4) / 8;
						int spriteTileY = (worldY + 4) / 8;
						
						GameMapLayer* baseLayer = GameSingleton::GetInstance()->GetMap()->GetGameMapLayer(0);
		
						int mapRows 	= baseLayer->GetNumRows();
						int mapColumns 	= baseLayer->GetNumColumns();
						
						switch(walkingDirection_)
						{
						case 0: 
						{
							bool canMoveNorth = (baseLayer->GetEventAt(spriteTileX, (spriteTileY > 0) ? spriteTileY - 1 : 0) == 0xFF) ? false : true;
							
							if (canMoveNorth) { worldY--; }
							else
							{
								walking_ = 0;
								distanceWalked_ = 0;
								handlingMotion_ = false;
							}
						} break; // North
						
						
						case 1: 
						{ 
							bool canMoveSouth = (baseLayer->GetEventAt(spriteTileX, (spriteTileY < mapRows - 1) ? spriteTileY + 1 : mapRows - 1) == 0xFF) ? false : true;
							
							if (canMoveSouth) { worldY++; }
							else
							{
								walking_ = 0;
								distanceWalked_ = 0;
								handlingMotion_ = false;
							}
						} break; // South
						
						case 2: 
						{
							bool canMoveEast = (baseLayer->GetEventAt((spriteTileX < mapColumns - 1) ? spriteTileX + 1 : mapColumns - 1, spriteTileY) == 0xFF) ? false : true;
							
							if (canMoveEast) { worldX++; }
							else
							{
								walking_ = 0;
								distanceWalked_ = 0;
								handlingMotion_ = false;
							}
							
						} break; // East
						
						case 3: 
						{
							bool canMoveEast = (baseLayer->GetEventAt((spriteTileX > 0) ? spriteTileX - 1 : 0, spriteTileY) == 0xFF) ? false : true;
							
							if (canMoveEast) { worldX--; }
							else
							{
								walking_ = 0;
								distanceWalked_ = 0;
								handlingMotion_ = false;
							}
							
						} break; // West
					
						default: break;
						}
						
						sprite_->SetWorldPosition(worldX, worldY);
					}
					else
					{
						// we are idling
						if (++framesIdle_ > (4 * speed_))
						{
							framesIdle_ = 0;
							handlingMotion_ = false;
						}

					}
				}
			}
		}
	}
	
	/**************************************************************************/
	
	void GameNPC::Render(ImageResource* target)
	{
		sprite_->Render(target);
	}
	
	/**************************************************************************/
	
	GameMapSprite* GameNPC::GetSprite() const
	{
		return sprite_;
	}
	
	/**************************************************************************/
	
	void GameNPC::SetMotionData(const char* motionData)
	{
		motionData_ = motionData;
	}
	
	/**************************************************************************/
	
	void GameNPC::SetSpeed(int speed)
	{
		speed_ = speed;
	}
	
	/**************************************************************************/
	
	void GameNPC::GetSpeed(int& storageSpeed)
	{
		storageSpeed = speed_;
	}
	
	/**************************************************************************/
	
	void GameNPC::GetMotionData(std::string& storageMotionData)
	{
		storageMotionData = motionData_;
	}
	
	/**************************************************************************/
	
	void GameNPC::ResetMotionDataCounter(int position)
	{
		position = (position < 0) ? 0 : position;
		motionDataCounter_ = (static_cast<unsigned int>(position) < motionData_.size()) ? position : 0;
	}
	
} // end namespace


