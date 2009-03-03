
// CODESTYLE: v2.0

// main.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

#include "GameLibrary.h"

#include "GameMap_Editors.h"

namespace GAME
{
	GameSingleton* globalGameInstance = 0;
} // end namespace

int main(int argc, char* argv[])
{
	// Windows Singleton Fix
	GAME::GameSingleton localGameInstance;
	GAME::globalGameInstance = &localGameInstance;

	// if the game engine initializes
	LogSimpleMessage("Startup Sequence Starting...");

	LogSimpleMessage("Initializing Game Engine...");

	if (GAME::GameSingleton::GetInstance()->Initialize(argc, argv))
	{
		LogSimpleMessage("Initializing Map Editors...");

		// if the map editors initialize
		if (GAME::GameMapEditorsSingleton::GetInstance()->Initialize())
		{
			LogSimpleMessage("Initializing Battle Engine...");

			// if the battle system initializes
			if (GAME::BattleEngineSingleton::GetInstance()->Initialize())
			{
				LogSimpleMessage("Executing Game...");

				// start the game processing
				GAME::GameSingleton::GetInstance()->Execute();
			}
		}
	}

	// ensure that we shutdown the graphics system before exiting
	GraphicsDevice->Destroy();

	LogSimpleMessage("Shutdown Sequence Complete.");
	return 0;
}
END_OF_MAIN();

