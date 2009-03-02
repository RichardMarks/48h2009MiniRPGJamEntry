
// CODESTYLE: v2.0

// GameBattle.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: The battle (encounter/fighting) engine

#ifndef __GAMEBATTLE_H__
#define __GAMEBATTLE_H__

namespace GAME
{
	/// the battle system singleton class
	class BattleEngineSingleton
	{
	public:
		static BattleEngineSingleton* GetInstance();
		~BattleEngineSingleton();

		/// initialize the battle system
		bool Initialize();

		/// clean up after the battle system
		void Destroy();

		/// update the battle system
		void Update();

		/// render the battle system
		void Render();

	private:
		BattleEngineSingleton();
		BattleEngineSingleton(const BattleEngineSingleton& rhs);
		const BattleEngineSingleton& operator=(const BattleEngineSingleton& rhs);

	}; // end class

	/// simple define to access the singleton class pointer easily
	#define BattleEngine BattleEngineSingleton::GetInstance()
} // end namespace
#endif


