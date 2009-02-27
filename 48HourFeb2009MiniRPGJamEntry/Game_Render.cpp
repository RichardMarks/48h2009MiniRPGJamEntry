
// CODESTYLE: v2.0

// Game_Render.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Render method for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	void GameSingleton::Render()
	{
		int camAnchorX 	= 0, camAnchorY = 0;
		int camWidth 	= 0, camHeight 	= 0;
		int camWorldX 	= 0, camWorldY 	= 0;

		camera_->GetSize(camWidth, camHeight);
		camera_->GetWorldPosition(camWorldX, camWorldY);
		camera_->GetAnchorPosition(camAnchorX, camAnchorY);

		int fov[] = /* left, top, right, and bottom collision planes */
		{
			camWorldX,
			camWorldY,
			camWorldX + camWidth,
			camWorldY + camHeight
		};

		// draw the map
		currentMap_->DrawMap(
			microDisplay_,
			camWorldX, camWorldY,
			camAnchorX, camAnchorY,
			camWidth, camHeight);

		// draw the NPCs
		gameNPCs_->Render(currentMap_->GetName().c_str(), microDisplay_, fov);

		// draw the player
		gameSprites_->Get(playerSpriteIndex_)->Render(microDisplay_);

		// draw the hud
		playerPortrait_->Blit(microDisplay_, 0, 0, 143, 16, 16, 16);

// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		lofiOverlay_->BlitMasked(microDisplay_, 0, 0, 141, 58, 55, 60);
#endif

		// draw the overlays
		windowOverlay_->BlitMasked(microDisplay_, 0, 0, 0, 0, 200, 150);

		// handle any region clear requests
		if (requestToClearDialogueRegion_)
		{
			microDisplay_->Rect(4, 93, 133, 145, 0, true);
			requestToClearDialogueRegion_ = false;
		}

		// render dialogues if there are any
		if (DIALOGUE::Undefined != dialogueMessage_->GetState())
		{
			// update the dialogue system
			dialogueMessage_->Render(microDisplay_);
		}

	}

} // end namespace

