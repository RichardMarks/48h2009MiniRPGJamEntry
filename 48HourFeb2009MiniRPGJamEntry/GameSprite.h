
// CODESTYLE: v2.0

// GameSprite.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a game sprite, its animation abilities, and anything else I think of

#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	class GameSprite
	{
	public:
		GameSprite();
		
		GameSprite(const char* pathName);
		
		GameSprite(
			const char* frameImagePathName, 
			int frameWidth, int frameHeight,
			int frameCount,
			int frameDelay,
			int initialFrame = 0);
		
		virtual ~GameSprite();
		
		void SetScreenPosition(int x, int y);
		void GetScreenPosition(int& storageX, int& storageY);
		
		void Animate(bool enabled = true);

		virtual void Update();
		virtual void Render(ENGINE::ImageResource* target);
		
		void SetFrame(int frame = 0);
		void GetFrame(int& storageFrame);
		
		void SetFrameCount(int frameCount);
		void GetFrameCount(int& storageFrameCount);
		
		void SetFrameDelay(int frameDelay);
		void GetFrameDelay(int& storageFrameDelay);
		
		bool IsAnimating() const;
		
		void SetSize(int width, int height);
		void GetSize(int& storageWidth, int& storageHeight);
		
		bool LoadFromFile(const char* pathName);
		
	private:
		GameSprite(const GameSprite& rhs);
		const GameSprite& operator=(const GameSprite& rhs);
		
	protected:
		void Destroy();
		
		int width_;						// width of sprite frame in pixels
		int height_;					// height of sprite frame in pixels
		
		int screenX_; 					// screen rendering coordinate in pixels
		int screenY_;					// ..
		int frame_; 					// which frame of animation 0,1,2,3
		int frameCount_; 				// max number of frames
		int frameCounter_; 				// animation delay counter
		int frameDelay_; 				// animation delay
		bool animating_; 				// are we supposed to animate?
		
		ENGINE::ImageResource* frames_;	// the graphics for the sprite animation frames
	}; // end class

} // end namespace
#endif


