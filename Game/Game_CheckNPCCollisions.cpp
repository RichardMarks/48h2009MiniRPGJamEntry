
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

	/**************************************************************************/

	int GameSingleton::CheckNPCCollisions(int tileX, int tileY) const
	{
		unsigned int npcCount = gameNPCs_->GetNumNPCs();

		for (unsigned int index = 0; index < npcCount; index++)
		{
			// if the npc is not on this map, we skip it
			if (!gameNPCs_->IsNPCOnMap(index, currentMap_->GetName().c_str())) { continue; }

			// create an invisible sprite to test collisions with at tileX, tileY
			GameMapSprite* dummy = new GameMapSprite();
			dummy->SetSize(8, 8);
			dummy->SetWorldPosition(tileX * 8, tileY * 8);

			// collision?
			if (dummy->CollidesWith(gameNPCs_->Get(index)->GetSprite()))
			{
				if (dummy) { delete dummy; }
				return index;
			}
			if (dummy) { delete dummy; }
		}

		return -1;
	}

} // end namespace

