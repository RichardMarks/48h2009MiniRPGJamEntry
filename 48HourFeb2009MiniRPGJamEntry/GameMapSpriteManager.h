
// CODESTYLE: v2.0

// GameMapSpriteManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of map sprite pointers

#ifndef __GAMEMAPSPRITEMANAGER_H__
#define __GAMEMAPSPRITEMANAGER_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;
	
	class GameMapSprite;
	
	class GameMapSpriteManager
	{
	public:
		GameMapSpriteManager();
		~GameMapSpriteManager();
		
		void Clear();
		
		unsigned int Add(GameMapSprite* sprite);
		
		GameMapSprite* Get(unsigned int index);
		
		const unsigned int GetNumSprites();
		
		void Update();
		void Render(ImageResource* target, int* fov = 0);
		
	private:
		GameMapSpriteManager(const GameMapSpriteManager& rhs);
		const GameMapSpriteManager& operator=(const GameMapSpriteManager& rhs);
		
	private:
		std::vector<GameMapSprite*> sprites_;
	};
	
} // end namespace
#endif


