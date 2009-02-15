
// CODESTYLE: v2.0

// GameMapSpriteManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of map sprite pointers

#include "GameLibrary.h"

namespace GAME
{
	GameMapSpriteManager::GameMapSpriteManager()
	{
	}

	/**************************************************************************/

	GameMapSpriteManager::~GameMapSpriteManager()
	{
		Clear();
	}

	/**************************************************************************/

	void GameMapSpriteManager::Clear()
	{
		sprites_.clear();
	}

	/**************************************************************************/

	unsigned int GameMapSpriteManager::Add(GameMapSprite* event)
	{
		unsigned int index = sprites_.size();
		sprites_.push_back(event);
		return index;
	}

	/**************************************************************************/

	GameMapSprite* GameMapSpriteManager::Get(unsigned int index)
	{
		return (index >= sprites_.size()) ? 0 : sprites_.at(index);
	}

	/**************************************************************************/

	const unsigned int GameMapSpriteManager::GetNumSprites()
	{
		return sprites_.size();
	}

	/**************************************************************************/

	void GameMapSpriteManager::Update()
	{
		std::vector<GameMapSprite*>::iterator iter;

		for (iter = sprites_.begin(); iter != sprites_.end(); iter++)
		{
			(*iter)->Update();
		}
	}

	/**************************************************************************/

	void GameMapSpriteManager::Render(ImageResource* target, int* fov)
	{
		// are we using a camera field of view to limit rendering ?
		if (0 != fov)
		{
			unsigned int spriteCount = sprites_.size();
			for (unsigned int index = 0; index < spriteCount; index++)
			{
				// get the current sprite
				GameMapSprite* sprite = sprites_.at(index);

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
				}
			}
		}
		else
		{
			// just draw them all
			std::vector<GameMapSprite*>::iterator iter;

			for (iter = sprites_.begin(); iter != sprites_.end(); iter++)
			{
				(*iter)->Render(target);
			}
		}
	}

} // end namespace


