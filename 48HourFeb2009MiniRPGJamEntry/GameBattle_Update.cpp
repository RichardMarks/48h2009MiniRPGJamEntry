
// CODESTYLE: v2.0

// GameBattle_Update.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: Implementation of the Update methods for the BattleSingleton class

#include "GameLibrary.h"

namespace GAME
{
	void BattleEngineSingleton::Update()
	{
		// depending on the current state of the battle system, we update different things
		switch(GetState())
		{
			case BATTLE::Initialize:
			{
				// initialize battle system resources
				InitializeBattleResources();

			} break;

			case BATTLE::InBattle:
			{
				// panic key is enter for debugging
				if (InputDevice->KeyPressed(KEY::Key_Enter))
				{
					// set state to endbattle so we can exit cleanly
					SetState(BATTLE::EndBattle);
				}

			} break;

			case BATTLE::EndBattle:
			{
				// release battle system resources
				DestroyBattleResources();

				// set state to finished so we can get out of here
				SetState(BATTLE::Finished);
			} break;

			default: break;
		} // end switch
	}

} // end namespace
