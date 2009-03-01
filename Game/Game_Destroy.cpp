
// CODESTYLE: v2.0

// Game_Destroy.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Destroy method for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	void GameSingleton::Destroy()
	{
		// clean up here
		#define _TMP_DELRES(resName) if (resName) { delete resName; resName = 0; }

		_TMP_DELRES(gameSettings_)
		_TMP_DELRES(smallFont_)
		_TMP_DELRES(largeFont_)
		_TMP_DELRES(microDisplay_)

		_TMP_DELRES(playerPortrait_)
		_TMP_DELRES(windowOverlay_)

		_TMP_DELRES(camera_)
		_TMP_DELRES(gameTiles_)
		_TMP_DELRES(gameMaps_)
		_TMP_DELRES(gameSprites_)
		_TMP_DELRES(gameNPCs_)
		_TMP_DELRES(gameStateManager_)
		_TMP_DELRES(dialogueMessage_)
		_TMP_DELRES(gameMenu_);


// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		_TMP_DELRES(lofiOverlay_)
#endif


		#undef _TMP_DELRES
	}

} // end namespace

