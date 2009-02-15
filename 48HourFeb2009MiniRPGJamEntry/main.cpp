
// CODESTYLE: v2.0

// main.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

#include "GameLibrary.h"

int main(int argc, char* argv[])
{
	if (GAME::GameSingleton::GetInstance()->Initialize(argc, argv))
	{
		GAME::GameSingleton::GetInstance()->Execute();
	}

	return 0;
}
END_OF_MAIN();

