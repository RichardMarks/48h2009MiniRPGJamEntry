
// CODESTYLE: v2.0

// GameMapSprite.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a sprite that walks on the world map

// so we don't need to prefix the ENGINE:: namespace scope to everything
#define GED101_USE_SIMPLE_NAMESPACES

// windows build bug patch
#if defined(WIN_32)
#define DWORD signed long long
#endif

// include the ged101 minimal header
#include "ged101minimal.h"

#include "GameMapSprite.h"

namespace GAME
{
	GameMapSprite::GameMapSprite() : 
		GameSprite(),
		face_(0),
		worldX_(0),
		worldY_(0)
	{
	}
	
	/**************************************************************************/
	
	GameMapSprite::GameMapSprite(const char* pathName) : 
		GameSprite(pathName),
		face_(0),
		worldX_(0),
		worldY_(0)
	{
	}
	
	/**************************************************************************/
	
	GameMapSprite::GameMapSprite(
		const char* frameImagePathName, 
		int frameWidth, int frameHeight,
		int frameCount,
		int frameDelay,
		int initialFrame) : 
		GameSprite(frameImagePathName, frameWidth, frameHeight, frameCount, frameDelay, initialFrame),
		face_(0),
		worldX_(0),
		worldY_(0)
	{
	}
	
	/**************************************************************************/
	
	GameMapSprite::~GameMapSprite()
	{
		GameSprite::Destroy();
	}
	
	/**************************************************************************/
	
	void GameMapSprite::SetFaceDirection(int facing)
	{
		face_ = facing;
	}
	
	/**************************************************************************/
	
	void GameMapSprite::GetFaceDirection(int& storageFacing)
	{
		storageFacing = face_;
	}
	
	/**************************************************************************/
	
	void GameMapSprite::SetWorldPosition(int x, int y)
	{
		worldX_ = x;
		worldY_ = y;
	}
	
	/**************************************************************************/
	
	void GameMapSprite::GetWorldPosition(int& storageX, int& storageY)
	{
		storageX = worldX_;
		storageY = worldY_;
	}
	
	/**************************************************************************/
	
	void GameMapSprite::Update()
	{
		GameSprite::Update();
	}
	
	/**************************************************************************/
	
	void GameMapSprite::Render(ImageResource* target)
	{
		int currentFrame = frame_ + face_;
		frames_->BlitMasked(
			target,
			1 + (currentFrame * width_) + (currentFrame), 1,
			screenX_, screenY_, 
			width_, height_);
	}
	
} // end namespace


