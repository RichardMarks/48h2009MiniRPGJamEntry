
// CODESTYLE: v2.0

// Game_Dialogue.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Dialogue methods for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	void GameSingleton::Dialogue(const char* message)
	{

	}

	/**************************************************************************/

	void GameSingleton::ClearDialogue()
	{
		requestToClearDialogueRegion_ = true;
	}

} // end namespace

