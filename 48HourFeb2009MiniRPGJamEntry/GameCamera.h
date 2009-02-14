
// CODESTYLE: v2.0

// GameCamera.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: handles a pixel*pixel smooth scrolling camera

#ifndef __GAMECAMERA_H__
#define __GAMECAMERA_H__

namespace GAME
{
	namespace CAMERA
	{
		enum CameraDirection
		{
			North,
			South,
			East,
			West
		};
	} // end namespace
	
	class GameMapSprite;
	
	class GameCamera
	{
	public:
		GameCamera();
		GameCamera(int anchorX, int anchorY, int worldX, int worldY, int width, int height, int speed, int worldW, int worldH);
		~GameCamera();
		
		void Pan(CAMERA::CameraDirection camDir, int speed = -1);
		
		void SetSize(int width, int height);
		void SetWorldPosition(int x, int y);
		void SetAnchorPosition(int x, int y);
		void SetSpeed(int speed = 1);
		
		void GetSize(int& storageX, int& storageY);
		void GetWorldPosition(int& storageX, int& storageY);
		void GetAnchorPosition(int& storageX, int& storageY);
		void GetSpeed(int& storageSpeed);
		
		void SetWorldSize(int width, int height);
		void GetWorldSize(int& storageWidth, int& storageHeight);
		
		void CenterOnSprite(GameMapSprite* sprite);
		
	private:
		GameCamera(const GameCamera& rhs);
		const GameCamera& operator=(const GameCamera& rhs);
	
	private:
		int anchorX_; 	// the screen anchor coordinate of the display
		int anchorY_;	// ..
		int worldX_; 	// the camera's upper-left-corner world position in pixels
		int worldY_;	// ..
		int width_; 	// the size of the camera in pixels
		int height_;	// ..
		int speed_; 	// number of pixels to move each pan
		
		int worldW_;	// width of the world in pixels
		int worldH_;	// height of the world in pixels
		
	};

} // end namespace
#endif


