
// CODESTYLE: v2.0

// main.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

// so we don't need to prefix the ENGINE:: namespace scope to everything
#define GED101_USE_SIMPLE_NAMESPACES

// windows build bug patch
#if defined(WIN_32)
#define DWORD signed long long
#endif

// include the ged101 minimal header
#include "ged101minimal.h"

#include "Game.h"

int main(int argc, char* argv[])
{
	#if 0
	// debugging the world loader
	GAME::GameSingleton::GetInstance()->LoadWorldFile("game.world");
	return 0;
	#endif
	
	if (GAME::GameSingleton::GetInstance()->Initialize(argc, argv))
	{
		GAME::GameSingleton::GetInstance()->Execute();
	}
	
	return 0;
}
END_OF_MAIN();

