
// CODESTYLE: v2.0

// Game_CheckNPCCollisions.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the CheckNPCCollisions method for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	bool GameSingleton::CheckNPCCollisions() const
	{
		unsigned int spriteCount = gameNPCs_->GetNumNPCs();

		for (unsigned int index = 0; index < spriteCount; index++)
		{
			// if the npc is not on this map, we skip it
			if (!gameNPCs_->IsNPCOnMap(index, currentMap_->GetName().c_str())) { continue; }

			// get the current sprite
			GameMapSprite* sprite = gameNPCs_->Get(index)->GetSprite();

			// collision?
			if (gameSprites_->Get(playerSpriteIndex_)->CollidesWith(sprite))
			{
				return true;
			}
		}

		return false;
	}

} // end namespace

