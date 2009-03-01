
// CODESTYLE: v2.0

// GameBattleSceneManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of battle scene pointers

#include "GameLibrary.h"

namespace GAME
{
	GameBattleSceneManager::GameBattleSceneManager()
	{
	}

	/**************************************************************************/

	GameBattleSceneManager::~GameBattleSceneManager()
	{
		Clear();
	}

	/**************************************************************************/

	void GameBattleSceneManager::Clear()
	{
		scenes_.clear();
	}

	/**************************************************************************/

	unsigned int GameBattleSceneManager::Add(GameBattleScene* scene)
	{
		unsigned int index = scenes_.size();
		scenes_.push_back(scene);
		return index;
	}

	/**************************************************************************/

	GameBattleScene* GameBattleSceneManager::Get(unsigned int index)
	{
		return (index >= scenes_.size()) ? 0 : scenes_.at(index);
	}

	/**************************************************************************/

	const unsigned int GameBattleSceneManager::GetNumScenes()
	{
		return scenes_.size();
	}

} // end namespace


