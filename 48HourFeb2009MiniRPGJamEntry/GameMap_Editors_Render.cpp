
// CODESTYLE: v2.0

// GameMap_Editors_Render.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: implements the GameMapEditorsSingleton Render* methods

#include "GameLibrary.h"

#include "GameMap_Editors.h"

namespace GAME
{
	void GameMapEditorsSingleton::RenderMap()
	{
		// render the map to the mapPanel_

		// get the tileset of the current map
		GameTileset* tileset = currentMap_->GetTileset();

		// get the base layer
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

		int mapRows 	= baseLayer->GetNumRows();
		int mapColumns 	= baseLayer->GetNumColumns();

#if 0 /// disabled debugging text
		fprintf(stderr,
			"RenderMap()\n\n"
			"Map Panel Size in Pixels: %d, %d\n"
			"Map Size in Tiles: %d, %d\n"
			"Camera Pos %d, %d\n"
			"Camera Size %d, %d\n\n",
			mapPanel_->GetWidth(), mapPanel_->GetHeight(),
			mapColumns, mapRows,
			cameraX_, cameraY_,
			cameraW_, cameraH_);
#endif

		InputDevice->MouseEnableCursorDisplay(false);

		// re-draw the view that the editor's camera can see
		for (int tileY = cameraY_; tileY < cameraY_ + cameraH_; tileY++)
		{
			for (int tileX = cameraX_; tileX < cameraX_ + cameraW_; tileX++)
			{
				// get the tile
				if (tileX >= mapColumns && tileY >= mapRows)
				{
					continue;
				}

				int tileIndex = baseLayer->GetTileIndexAt(tileX, tileY);

				// draw the tile
				int tilePixelX = (tileX - cameraX_) * 8;
				int tilePixelY = (tileY - cameraY_) * 8;

				tileset->GetImageResourceAt(tileIndex)->Blit(
					mapPanel_,
					0, 0,
					tilePixelX,
					tilePixelY,
					8, 8);

				// shade the solid tiles red and add a red line to them as well
				if (MAPEDITORS::EditingCollisionLayer == state_)
				{
					if (currentMap_->IsSolid(tileX, tileY))
					{
						ColorRGB colorRed(255, 0, 0);
						ImageResource hilight(8, 8, colorRed.Get());
						hilight.BlitAlpha(mapPanel_, tilePixelX, tilePixelY, 0.5f);
						mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX + 7, tilePixelY, colorRed.Get());
						mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX, tilePixelY + 7, colorRed.Get());
					}
				}

				// draw a little blue circle on warp tiles
				if (MAPEDITORS::EditingMapWarps == state_)
				{
					if (currentMap_->IsWarp(tileX, tileY))
					{
						ColorRGB colorBlue(0, 0, 255);
						ColorRGB colorWhite(255, 255, 255);

						ImageResource hilight(8, 8, colorWhite.Get());
						hilight.Circle(4, 4, 4, colorBlue.Get(), true);

						hilight.BlitAlpha(mapPanel_, tilePixelX, tilePixelY, 0.5f);

						BitmapFont t; t.Print(mapPanel_,
						tilePixelX, tilePixelY, "%d", currentMap_->GetWarp(tileX, tileY));

					}
				}
			}
		}

		InputDevice->MouseEnableCursorDisplay(true);
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Render()
	{
		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();

		int mapPanelW = mapPanel_->GetWidth();
		int mapPanelH = mapPanel_->GetHeight();

		int mouseTileX = cameraX_ + (mouseX_ / 16);
		int mouseTileY = cameraY_ + (mouseY_ / 16);

		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Base Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
			} break;

			case MAPEDITORS::EditingMapWarps:
			{
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);
				InputDevice->MouseEnableCursorDisplay(true);

				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Map Warps on \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
				debugFont.Print(display, 1, 12, "Mouse Pos Pixel: (%3d, %3d) Tile:(%3d, %3d)", mouseX_, mouseY_, mouseTileX, mouseTileY);

			} break;

			// we are editing the collision layer of the map
			case MAPEDITORS::EditingCollisionLayer:
			{
				// draw the map
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);
				InputDevice->MouseEnableCursorDisplay(true);

				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Collision Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
				debugFont.Print(display, 1, 12, "Mouse Pos Pixel: (%3d, %3d) Tile:(%3d, %3d)", mouseX_, mouseY_, mouseTileX, mouseTileY);


			} break;

			// we are editing the events on the map
			case MAPEDITORS::EditingEvents:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Events Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
			} break;

			// we are selecting a tile from the tileset browser
			case MAPEDITORS::SelectingTile:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Select a Tile");
			} break;

			// we are previewing the map
			case MAPEDITORS::Previewing:
			{
				// draw the map
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);

				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Previewing Map (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());

			} break;

			default: break;
		} // end switch

		InputDevice->MouseDisplayOnSurface(display);
	}

} // end namespace
