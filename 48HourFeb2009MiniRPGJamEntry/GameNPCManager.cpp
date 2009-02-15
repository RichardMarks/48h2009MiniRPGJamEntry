
// CODESTYLE: v2.0

// GameNPCManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of NPC pointers

#include "GameLibrary.h"

namespace GAME
{
	GameNPCManager::GameNPCManager()
	{
	}

	/**************************************************************************/

	GameNPCManager::~GameNPCManager()
	{
		Clear();
	}

	/**************************************************************************/

	void GameNPCManager::Clear()
	{
		npcs_.clear();
		maps_.clear();
	}

	/**************************************************************************/

	unsigned int GameNPCManager::Add(GameNPC* npc, const char* parentMapName)
	{
		unsigned int index = npcs_.size();
		npcs_.push_back(npc);
		maps_.push_back(parentMapName);
		return index;
	}

	/**************************************************************************/

	bool GameNPCManager::IsNPCOnMap(unsigned int index, const char* mapName) const
	{
		return static_cast<bool>(mapName == maps_.at(index));
	}

	/**************************************************************************/

	GameNPC* GameNPCManager::Get(unsigned int index)
	{
		return (index >= npcs_.size()) ? 0 : npcs_.at(index);
	}

	/**************************************************************************/

	const unsigned int GameNPCManager::GetNumNPCs()
	{
		return npcs_.size();
	}

	/**************************************************************************/

	void GameNPCManager::Update(const char* mapName)
	{
		unsigned int spriteCount = npcs_.size();
		for (unsigned int index = 0; index < spriteCount; index++)
		{
			if (!IsNPCOnMap(index, mapName)) { continue; }
			npcs_.at(index)->Update();
		}
	}

	/**************************************************************************/

	void GameNPCManager::Render(const char* mapName, ImageResource* target, int* fov)
	{
		// are we using a camera field of view to limit rendering ?
		if (0 != fov)
		{
			unsigned int spriteCount = npcs_.size();
			for (unsigned int index = 0; index < spriteCount; index++)
			{
				if (!IsNPCOnMap(index, mapName)) { continue; }

				// get the current sprite
				GameMapSprite* sprite = npcs_.at(index)->GetSprite();

				int sx = 0, sy = 0; // sprite screen position
				int wx = 0, wy = 0; // sprite world position

				// get the world position
				sprite->GetWorldPosition(wx, wy);

				// if the sprite is within the field of view
				if (wx >= fov[0] && wx <= fov[2] && wy >= fov[1] && wy <= fov[3])
				{
					// figure the screen position
					sx = wx - fov[0];
					sy = wy - fov[1];

					// set the screen position
					sprite->SetScreenPosition(sx, sy);

					// draw the sprite
					sprite->Render(target);

					int spriteWidth = 0, spriteHeight = 0;
					sprite->GetSize(spriteWidth, spriteHeight);

					ColorRGB redColor(255, 0, 0);
					target->Rect(sx, sy, sx + spriteWidth, sy + spriteHeight, redColor.Get());

				}
			}
		}
		else
		{
			// just draw them all
			std::vector<GameNPC*>::iterator iter;

			for (iter = npcs_.begin(); iter != npcs_.end(); iter++)
			{
				(*iter)->Render(target);
			}
		}
	}

} // end namespace


