
// CODESTYLE: v2.0

// GameBattle.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: The battle (encounter/fighting) engine

#include "GameLibrary.h"

namespace GAME
{
	BattleEngineSingleton* BattleEngineSingleton::GetInstance()
	{
		static BattleEngineSingleton instance;
		return &instance;
	}

	/**************************************************************************/

	BattleEngineSingleton::BattleEngineSingleton() { /* do not put code here */ }

	/**************************************************************************/

	BattleEngineSingleton::~BattleEngineSingleton() { /* do not put code here */ }

	/**************************************************************************/

	void BattleEngineSingleton::Destroy()
	{
		// clean up
	}


} // end namespace



