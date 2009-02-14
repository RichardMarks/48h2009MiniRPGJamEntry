
// CODESTYLE: v2.0

// GameNPC.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines an NPC

#ifndef __GAMENPC_H__
#define __GAMENPC_H__

#include <string>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;
	
	class GameMapSprite;
	
	class GameNPC
	{
	public:
		GameNPC(GameMapSprite* sprite, int speed = 1, const char* motionData = "");
		
		~GameNPC();
		
		void Update();
		void Render(ImageResource* target);
		
		GameMapSprite* GetSprite() const;
		
		void SetMotionData(const char* motionData = "");
		void SetSpeed(int speed = 1);
		
		void GetSpeed(int& storageSpeed);
		void GetMotionData(std::string& storageMotionData);
		
		void ResetMotionDataCounter(int position = 0);
		
	private:
		GameNPC(const GameNPC& rhs);
		const GameNPC& operator=(const GameNPC& rhs);
		
	private:
		GameMapSprite* sprite_; // the animated sprite of the NPC
		
		int speed_;
		std::string motionData_;
		int motionDataCounter_;
		bool walking_;
		int walkingDirection_;
		int distanceWalked_;
		int framesIdle_;
		bool handlingMotion_;
		int logicTimer_;
	};
	
} // end namespace
#endif


