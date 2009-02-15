
// CODESTYLE: v2.0

// GameSprite.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a game sprite, its animation abilities, and anything else I think of

#include "GameLibrary.h"

namespace GAME
{
	GameSprite::GameSprite() :
		width_(0),
		height_(0),
		screenX_(0),
		screenY_(0),
		frame_(0),
		frameCount_(0),
		frameCounter_(0),
		frameDelay_(0),
		animating_(false),
		frames_(0)
	{
	}

	/**************************************************************************/

	GameSprite::GameSprite(const char* pathName) :
		width_(0),
		height_(0),
		screenX_(0),
		screenY_(0),
		frame_(0),
		frameCount_(0),
		frameCounter_(0),
		frameDelay_(0),
		animating_(false),
		frames_(0)
	{
		if (!LoadFromFile(pathName))
		{
			LogFatal("could not load the requested sprite file: %s", pathName);
		}
	}

	/**************************************************************************/

	GameSprite::GameSprite(
		const char* frameImagePathName,
		int frameWidth, int frameHeight,
		int frameCount,
		int frameDelay,
		int initialFrame) :
		width_(frameWidth),
		height_(frameHeight),
		screenX_(0),
		screenY_(0),
		frame_(initialFrame),
		frameCount_(frameCount),
		frameCounter_(0),
		frameDelay_(frameDelay),
		animating_(false),
		frames_(0)
	{
		frames_ = new ImageResource(frameImagePathName);
	}

	/**************************************************************************/

	GameSprite::~GameSprite()
	{
		Destroy();
	}

	/**************************************************************************/

	void GameSprite::Destroy()
	{
		if (frames_)
		{
			delete frames_;
			frames_ = 0;
		}
	}

	/**************************************************************************/

	void GameSprite::SetScreenPosition(int x, int y)
	{
		screenX_ = x;
		screenY_ = y;
	}

	/**************************************************************************/

	void GameSprite::GetScreenPosition(int& storageX, int& storageY)
	{
		storageX = screenX_;
		storageY = screenY_;
	}

	/**************************************************************************/

	void GameSprite::Animate(bool enabled)
	{
		animating_ = enabled;
	}

	/**************************************************************************/

	void GameSprite::Update()
	{
		// only animate if we should be
		if (!animating_) return;
		if (++frameCounter_ >= frameDelay_)
		{
			frame_++;
			if (frame_ > frameCount_)
			{
				frame_ = 0;
			}

			frameCounter_ = 0;
		}
	}

	/**************************************************************************/

	void GameSprite::Render(ENGINE::ImageResource* target)
	{
		frames_->BlitMasked(
			target,
			1 + (frame_ * width_) + (frame_), 1,
			screenX_, screenY_,
			width_, height_);
	}

	/**************************************************************************/

	void GameSprite::SetFrame(int frame)
	{
		frame_ = frame;
	}

	/**************************************************************************/

	void GameSprite::GetFrame(int& storageFrame)
	{
		storageFrame = frame_;
	}

	/**************************************************************************/

	void GameSprite::SetFrameCount(int frameCount)
	{
		frameCount_ = frameCount;
	}

	/**************************************************************************/

	void GameSprite::GetFrameCount(int& storageFrameCount)
	{
		storageFrameCount = frameCount_;
	}

	/**************************************************************************/

	void GameSprite::SetFrameDelay(int frameDelay)
	{
		frameDelay_ = frameDelay;
	}

	/**************************************************************************/

	void GameSprite::GetFrameDelay(int& storageFrameDelay)
	{
		storageFrameDelay = frameDelay_;
	}

	/**************************************************************************/

	bool GameSprite::IsAnimating() const
	{
		return animating_;
	}

	/**************************************************************************/

	void GameSprite::SetSize(int width, int height)
	{
		width_ = width;
		height_ = height;
	}

	/**************************************************************************/

	void GameSprite::GetSize(int& storageWidth, int& storageHeight)
	{
		storageWidth = width_;
		storageHeight = height_;
	}

	/**************************************************************************/

	bool GameSprite::LoadFromFile(const char* pathName)
	{
		// write a sprite loader here later...
		return false;
	}

} // end namespace


