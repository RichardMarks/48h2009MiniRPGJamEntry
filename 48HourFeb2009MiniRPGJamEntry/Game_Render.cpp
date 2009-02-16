
// CODESTYLE: v2.0

// Game_Render.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Render method for the game singleton class

#include "GameLibrary.h"

// comment this out to turn off the debugging display
#define ENABLE_DEBUGGING_DISPLAY_INFORMATION

namespace GAME
{
	void GameSingleton::Render()
	{
		// begin rendering
		GraphicsDevice->BeginScene(0);
		microDisplay_->Clear();


		int camAnchorX = 0, camAnchorY = 0;
		int camWidth = 0, camHeight = 0;
		int camWorldX = 0, camWorldY = 0;

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

		// draw the starfield
		starfield_->Render(microDisplay_);

		// draw the overlays
		windowOverlay_->BlitMasked(microDisplay_, 0, 0, 0, 0, 200, 150);
		lofiOverlay_->BlitMasked(microDisplay_, 0, 0, 141, 58, 55, 60);


		// handle any region clear requests
		if (requestToClearDialogueRegion_)
		{
			microDisplay_->Rect(4, 93, 133, 145, 0, true);
			requestToClearDialogueRegion_ = false;
		}

		// we are done rendering
		// end the scene using special 4x scaling
		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();
		microDisplay_->Blit(
			display,
			0, 0,
			200, 150,
			0, 0,
			display->GetWidth(), display->GetHeight());

		// debugging display
#if defined(ENABLE_DEBUGGING_DISPLAY_INFORMATION)
		{
			int psx = 0, psy = 0; // player screen X, Y coords
			int pwx = 0, pwy = 0; // player world X, Y coords

			// get the player screen position
			gameSprites_->Get(playerSpriteIndex_)->GetScreenPosition(psx, psy);

			// figure the player's world position
			pwx = psx + camWorldX;
			pwy = psy + camWorldY;

			int playerTileX = (pwx + 4) / 8;
			int playerTileY = (pwy + 4) / 8;

			// get the event at the tile to scan

			GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);


			int eventCode = baseLayer->GetEventAt(playerTileX, playerTileY);

			BitmapFont debugFont;
			debugFont.Print(display, 4, 80,     "Current Map: (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
			debugFont.Print(display, 4, 80+8,   "Camera World Pos: %d, %d", camWorldX, camWorldY);
			debugFont.Print(display, 4, 80+8*2, "Player World Pos: %d, %d", pwx, pwy);
			debugFont.Print(display, 4, 80+8*3, "Player Screen Pos: %d, %d", psx, psx);
			debugFont.Print(display, 4, 80+8*4, "Player Tile Pos: %d, %d", playerTileX, playerTileY);
			debugFont.Print(display, 4, 80+8*5, "Event Code @ Player Tile Pos: %d", eventCode);

			debugFont.Print(display, 4, 80+8*6, "Steps Taken: %d", stepsTaken_);
			debugFont.Print(display, 4, 80+8*7, "Steps Until Ambush: %d", stepsUntilAmbush_);
		}
#endif
		GraphicsDevice->EndScene();
	}

} // end namespace

