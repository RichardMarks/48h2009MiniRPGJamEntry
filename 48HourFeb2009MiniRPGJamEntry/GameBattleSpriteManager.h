
// CODESTYLE: v2.0

// GameBattleSpriteManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of battle sprite pointers

#ifndef __GAMEBATTLESPRITEMANAGER_H__
#define __GAMEBATTLESPRITEMANAGER_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;

	class GameBattleSprite;

	class GameBattleSpriteManager
	{
	public:
		GameBattleSpriteManager();
		~GameBattleSpriteManager();

		void Clear();

		unsigned int Add(GameBattleSprite* sprite);

		GameBattleSprite* Get(unsigned int index);

		const unsigned int GetNumSprites();

		void Update();
		void Render(ImageResource* target);

	private:
		GameBattleSpriteManager(const GameBattleSpriteManager& rhs);
		const GameBattleSpriteManager& operator=(const GameBattleSpriteManager& rhs);

	private:
		std::vector<GameBattleSprite*> sprites_;
	};

} // end namespace
#endif


