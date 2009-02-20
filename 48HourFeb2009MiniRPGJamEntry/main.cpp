
// CODESTYLE: v2.0

// main.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

#include "GameLibrary.h"

#include "GameMap_Editors.h"

int main(int argc, char* argv[])
{
	// if the game engine initializes
	if (GAME::GameSingleton::GetInstance()->Initialize(argc, argv))
	{
		// if the map editors initialize
		if (GAME::GameMapEditorsSingleton::GetInstance()->Initialize())
		{
			// if the battle system initializes
			if (GAME::BattleEngineSingleton::GetInstance()->Initialize())
			{
				// start the game processing
				GAME::GameSingleton::GetInstance()->Execute();
			}
		}
	}

	return 0;
}
END_OF_MAIN();

