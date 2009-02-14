
// CODESTYLE: v2.0

// GameCamera.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: handles a pixel*pixel smooth scrolling camera

#include "GameLibrary.h"

namespace GAME
{
	GameCamera::GameCamera() :
		anchorX_(0),
		anchorY_(0),
		worldX_(0),
		worldY_(0),
		width_(0),
		height_(0),
		speed_(1),
		worldW_(0),
		worldH_(0)
	{
	}
	
	/**************************************************************************/
	
	GameCamera::GameCamera(int anchorX, int anchorY, int worldX, int worldY, int width, int height, int speed, int worldW, int worldH) :
		anchorX_(anchorX),
		anchorY_(anchorY),
		worldX_(worldX),
		worldY_(worldY),
		width_(width),
		height_(height),
		speed_(speed),
		worldW_(worldW),
		worldH_(worldH)
	{
	}
	/**************************************************************************/
	
	GameCamera::~GameCamera()
	{
	}
	
	/**************************************************************************/
	
	void GameCamera::Pan(CAMERA::CameraDirection camDir, int speed)
	{
		int panSpeed = (speed == -1) ? speed_ : speed;
		switch(camDir)
		{
			case CAMERA::North: { worldY_ -= panSpeed; } break;
			case CAMERA::South: { worldY_ += panSpeed; } break;
			case CAMERA::East:  { worldX_ += panSpeed; } break;
			case CAMERA::West:  { worldX_ -= panSpeed; } break;
			default: break;
		}
	}
	
	/**************************************************************************/
	
	void GameCamera::SetSize(int width, int height)
	{
		width_ = width;
		height_ = height;
	}
	
	/**************************************************************************/
	
	void GameCamera::SetWorldPosition(int x, int y)
	{
		worldX_ = x;
		worldY_ = y;
	}
	
	/**************************************************************************/
	
	void GameCamera::SetAnchorPosition(int x, int y)
	{
		anchorX_ = x;
		anchorY_ = y;
	}
	
	/**************************************************************************/
	
	void GameCamera::SetSpeed(int speed)
	{
		speed_ = speed;
	}
	
	/**************************************************************************/
	
	void GameCamera::GetSize(int& storageX, int& storageY)
	{
		storageX = width_;
		storageY = height_;
	}
	
	/**************************************************************************/
	
	void GameCamera::GetWorldPosition(int& storageX, int& storageY)
	{
		storageX = worldX_;
		storageY = worldY_;
	}
	
	/**************************************************************************/
	
	void GameCamera::GetAnchorPosition(int& storageX, int& storageY)
	{
		storageX = anchorX_;
		storageY = anchorY_;
	}
	
	/**************************************************************************/
	
	void GameCamera::GetSpeed(int& storageSpeed)
	{
		storageSpeed = speed_;
	}
	
	/**************************************************************************/
	
	void GameCamera::SetWorldSize(int width, int height)
	{
		worldW_ = width;
		worldH_ = height;
	}
	
	/**************************************************************************/
	
	void GameCamera::GetWorldSize(int& storageWidth, int& storageHeight)
	{
		storageWidth = worldW_;
		storageHeight = worldH_;
	}
	
	/**************************************************************************/
	
	void GameCamera::CenterOnSprite(GameMapSprite* sprite)
	{
		int spriteWorldX = 0, spriteWorldY = 0;
		int spriteScreenX = 0, spriteScreenY = 0;
		
		// grab the sprite's world position
		sprite->GetWorldPosition(spriteWorldX, spriteWorldY);
		
		// center the sprite in the camera's view
		sprite->SetScreenPosition(width_ / 2, height_ / 2);
		
		// pre-calculate the new camera position
		// with the sprite centered in the middle
		int camToX = spriteWorldX - width_ / 2;
		int camToY = spriteWorldY - height_ / 2;
	
		// handle the camera going off the left edge of the map
		if (camToX < 0)
		{
			int offsetX = abs(camToX);
	
			// get the sprite screen position 
			sprite->GetScreenPosition(spriteScreenX, spriteScreenY);

			// offset sprite's screen position
			spriteScreenX = spriteScreenX - offsetX;

			// set the sprite's screen position
			sprite->SetScreenPosition(spriteScreenX, spriteScreenY);
		
			// set the camera on the edge
			camToX = 0;
		}
	
		// handle the camera going off the top edge of the map
		if (camToY < 0)
		{
			int offsetY = abs(camToY);
			
			// get the sprite screen position 
			sprite->GetScreenPosition(spriteScreenX, spriteScreenY);

			// offset sprite's screen position
			spriteScreenY = spriteScreenY - offsetY;

			// set the sprite's screen position
			sprite->SetScreenPosition(spriteScreenX, spriteScreenY);
		
			// set the camera on the edge
			camToY = 0;
		}
	
		// handle the camera going off the right edge of the map
		if (camToX + width_ > worldW_)
		{
			int offsetX = abs(worldW_ - (camToX + width_));
	
			// get the sprite screen position 
			sprite->GetScreenPosition(spriteScreenX, spriteScreenY);

			// offset sprite's screen position
			spriteScreenX = spriteScreenX + offsetX;

			// set the sprite's screen position
			sprite->SetScreenPosition(spriteScreenX, spriteScreenY);
		
			// set the camera on the edge
			camToX = worldW_ - width_;
		}
		
		// handle the camera going off the bottom edge of the map
		if (camToY + height_ > worldH_)
		{
			int offsetY = abs(worldH_ - (camToY + height_));
			
			// get the sprite screen position 
			sprite->GetScreenPosition(spriteScreenX, spriteScreenY);

			// offset sprite's screen position
			spriteScreenY = spriteScreenY + offsetY;

			// set the sprite's screen position
			sprite->SetScreenPosition(spriteScreenX, spriteScreenY);
		
			// set the camera on the edge
			camToY = worldH_ - height_;
		}
	
		// finally move the camera to its proper place
		worldX_ = camToX;
		worldY_ = camToY;
	}
	

} // end namespace


