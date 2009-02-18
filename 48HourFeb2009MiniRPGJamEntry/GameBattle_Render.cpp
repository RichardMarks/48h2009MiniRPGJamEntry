
// CODESTYLE: v2.0

// GameBattle_Render.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: Implementation of the Rendering methods for the BattleSingleton class

#include "GameLibrary.h"

namespace GAME
{
	void BattleEngineSingleton::Render()
	{
		// depending on the current state of the battle system, we render different things
		switch(GetState())
		{
			case BATTLE::InBattle:
			{
				// render the actual battle

				// render the scene background
				battleSceneImage_->Blit(microDisplay_, 0, 0, 4, 4, battleSceneImage_->GetWidth(), battleSceneImage_->GetHeight());

				// render the battle gauge overlay
				battleGaugeOverlay_->BlitMasked(microDisplay_, 0, 0, 46, 5, 109, 26);

				// render the borders overlay
				windowOverlay_->BlitMasked(microDisplay_, 0, 0, 0, 0, 200, 150);

			} break;

			case BATTLE::EndBattle:
			{
				// render any rewards if there are rewards to render
			} break;

			default: break;
		} // end switch
	}

} // end namespace
