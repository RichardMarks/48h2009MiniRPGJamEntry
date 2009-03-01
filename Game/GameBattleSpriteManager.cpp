
// CODESTYLE: v2.0

// GameBattleSpriteManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of map sprite pointers

#include "GameLibrary.h"

namespace GAME
{
	GameBattleSpriteManager::GameBattleSpriteManager()
	{
	}

	/**************************************************************************/

	GameBattleSpriteManager::~GameBattleSpriteManager()
	{
		Clear();
	}

	/**************************************************************************/

	void GameBattleSpriteManager::Clear()
	{
		sprites_.clear();
	}

	/**************************************************************************/

	unsigned int GameBattleSpriteManager::Add(GameBattleSprite* event)
	{
		unsigned int index = sprites_.size();
		sprites_.push_back(event);
		return index;
	}

	/**************************************************************************/

	GameBattleSprite* GameBattleSpriteManager::Get(unsigned int index)
	{
		return (index >= sprites_.size()) ? 0 : sprites_.at(index);
	}

	/**************************************************************************/

	const unsigned int GameBattleSpriteManager::GetNumSprites()
	{
		return sprites_.size();
	}

	/**************************************************************************/

	void GameBattleSpriteManager::Update()
	{
		std::vector<GameBattleSprite*>::iterator iter;

		for (iter = sprites_.begin(); iter != sprites_.end(); iter++)
		{
			(*iter)->Update();
		}
	}

	/**************************************************************************/

	void GameBattleSpriteManager::Render(ImageResource* target)
	{
		std::vector<GameBattleSprite*>::iterator iter;

		for (iter = sprites_.begin(); iter != sprites_.end(); iter++)
		{
			(*iter)->Render(target);
		}
	}

} // end namespace


