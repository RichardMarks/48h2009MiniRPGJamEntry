
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
		if (DIALOGUE::Undefined == dialogueMessage_->GetState())
		{

			dialogueMessage_->Initialize(
				UTILITY::PARSING::TextFileParser::Execute("data/dialogue/test.dialogue", "#"),
				smallFont_, false);
		}
	}

	/**************************************************************************/

	void GameSingleton::ClearDialogue()
	{
		requestToClearDialogueRegion_ = true;
	}

} // end namespace

