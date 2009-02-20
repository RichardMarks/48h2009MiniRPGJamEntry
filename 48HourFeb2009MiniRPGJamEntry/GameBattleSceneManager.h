
// CODESTYLE: v2.0

// GameBattleSceneManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of battle scene pointers

#ifndef __GAMEBATTLESCENEMANAGER_H__
#define __GAMEBATTLESCENEMANAGER_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;

	class GameBattleScene;

	class GameBattleSceneManager
	{
	public:
		GameBattleSceneManager();
		~GameBattleSceneManager();

		void Clear();

		unsigned int Add(GameBattleScene* scene);

		GameBattleScene* Get(unsigned int index);

		const unsigned int GetNumScenes();

	private:
		GameBattleSceneManager(const GameBattleSceneManager& rhs);
		const GameBattleSceneManager& operator=(const GameBattleSceneManager& rhs);

	private:
		std::vector<GameBattleScene*> scenes_;
	};

} // end namespace
#endif


