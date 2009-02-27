
// CODESTYLE: v2.0

// GameMapSprite.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a sprite that walks on the world map

#ifndef __GAMEMAPSPRITE_H__
#define __GAMEMAPSPRITE_H__

namespace ENGINE
{
	class ImageResource;
}

#include "GameSprite.h"

namespace GAME
{
	namespace MAPSPRITE
	{
		const int WALK_SOUTH_FRAME 		= 0;
		const int WALK_NORTH_FRAME 		= 4;
		const int WALK_EAST_FRAME  		= 8;
		const int WALK_WEST_FRAME  		= 12;
	} // end namespace

	class GameMapSprite : public GameSprite
	{
	public:
		GameMapSprite();

		GameMapSprite(const char* pathName);

		GameMapSprite(
			const char* frameImagePathName,
			int frameWidth, int frameHeight,
			int frameCount,
			int frameDelay,
			int initialFrame = 0);

		~GameMapSprite();

		void SetFaceDirection(int facing = MAPSPRITE::WALK_SOUTH_FRAME);
		void GetFaceDirection(int& storageFacing);

		void SetWorldPosition(int x, int y);
		void GetWorldPosition(int& storageX, int& storageY);

		void Update();
		void Render(ImageResource* target);


		/// returns true if the bounding rectangle of this sprite intersects with the passed sprite
		bool CollidesWith(GameMapSprite* sprite) const;

		/// clones the properties of @sprite into @this sprite.
		void Clone(GameMapSprite* sprite);

	private:
		GameMapSprite(const GameMapSprite& rhs);
		const GameMapSprite& operator=(const GameMapSprite& rhs);

	private:
		int face_; 				// which direction is the sprite facing
		int worldX_; 			// game world coordinate in pixels
		int worldY_;			// ..
	};

} // end namespace
#endif


