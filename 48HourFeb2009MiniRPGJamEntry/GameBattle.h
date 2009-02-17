
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
	using ENGINE::ImageResource;
	using ENGINE::BitmapFont;

	/// a class for handling battle rewards
	class BattleReward
	{
	public:
		BattleReward();
		~BattleReward();

	private:
		BattleReward(const BattleReward& rhs);
		const BattleReward& operator=(const BattleReward& rhs);
	};

	class BattleCombatant
	{
	public:
	private:
	private:
	};

	/// the battle system singleton class
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
		/// shared resource pointers -- the game singleton class is responsible for init/destroy these pointers
		BitmapFont* smallFont_;
		BitmapFont* largeFont_;
		ImageResource* microDisplay_;

		/// the battle rewards that are awarded to the player if the player is alive when the battle ends
		std::vector<BattleReward*> rewards_;

		/// the background scene image of the battle
		ImageResource* battleSceneImage_;

	}; // end class

	/// simple define to access the singleton class pointer easily
	#define BattleEngine BattleEngineSingleton::GetInstance()
} // end namespace
#endif


