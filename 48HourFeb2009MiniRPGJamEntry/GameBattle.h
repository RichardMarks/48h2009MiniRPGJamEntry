
// CODESTYLE: v2.0

// GameBattle.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: The battle (encounter/fighting) engine

#ifndef __GAMEBATTLE_H__
#define __GAMEBATTLE_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
	class BitmapFont;
}

namespace GAME
{
	class BattleReward
	{
	public:
		BattleReward();
		~BattleReward();
		
	private:
		BattleReward(const BattleReward& rhs);
		const BattleReward& operator=(const BattleReward& rhs);
	};
	
	class BattleEngineSingleton
	{
	public:
		static BattleEngineSingleton* GetInstance();
		~BattleEngineSingleton();
		
		bool Initialize();
		void Execute();
		
	private:
	
		void SetupMonsterParty();
		void SetupFightingOrder();
		void SetupScene();
		
		void RenderScene();
		
		void PlayerTurn();
		void EnemyTurn();
		
		void GiveRewards();
		
		void Destroy();
	
		BattleEngineSingleton();
		BattleEngineSingleton(const BattleEngineSingleton& rhs);
		const BattleEngineSingleton& operator=(const BattleEngineSingleton& rhs);
	private:
	
		std::vector<BattleReward*> rewards_;
		
		ENGINE::ImageResource* battleSceneImage_;
		ENGINE::ImageResource* microDisplay_; // the 200x150 display buffer that we scale x4
		
		ENGINE::BitmapFont* smallFont_;
		ENGINE::BitmapFont* largeFont_;
	}; // end class

	#define BattleEngine BattleEngineSingleton::GetInstance()
} // end namespace
#endif


