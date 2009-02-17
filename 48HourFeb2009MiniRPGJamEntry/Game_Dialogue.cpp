
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
			/*
			dialogueMessage_->Initialize(
				"Castle Guard:\n"
				"Halt! Who goes there?\n"
				"You are not authorized\n"
				"to be here! Leave now!\n"
				"\n"
				"...I said Leave NOW!!!\n"
				"\n", smallFont_, false);
			*/

			dialogueMessage_->Initialize(
				"A young woman's body falls limp as some figure steps away"
				"from her. A man races towards the figure from behind you,"
				"nearly running into you as he passes.\n\n"
				"Raising his hands to the heavens he speaks several words "
				"in a foreign tongue and a white light emanates from "
				"his palms and seem to pierce through the dark figure, "
				"temporarily stunning it. You feel the hairs on the back of "
				"your neck raise as the figure reaches out to the man quickly"
				"advancing on him, lets out a violent scream that shatters "
				"the unusally silent night and viciously attacks the man.\n\n"
				"Your presence does not go unnoticed and the dark figure "
				"begins to move towards you. You feel as if in a trance;"
				"Unable to move, you feel the figure staring at you, his "
				"dark gaze seems to have power over you.\n\n",
				smallFont_, false);
		}
	}

	/**************************************************************************/

	void GameSingleton::ClearDialogue()
	{
		requestToClearDialogueRegion_ = true;
	}

} // end namespace

