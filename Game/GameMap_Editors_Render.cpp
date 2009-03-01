
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

		bool doCollision = (MAPEDITORS::EditingCollisionLayer == state_ || MAPEDITORS::EditingMapWarps == state_) ? true : false;

		bool doCollisionOnion = (MAPEDITORS::EditingMapWarps == state_) ? true : false;

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

				// shading XD
				if (doCollision)
				{
					if (currentMap_->IsSolid(tileX, tileY))
					{
						if (doCollisionOnion)
						{
							ColorRGB colorRed(128, 0, 0);
							ImageResource hilight(8, 8, colorRed.Get());

							hilight.BlitAlpha(mapPanel_, tilePixelX, tilePixelY, 0.4f);

							mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX + 7, tilePixelY, colorRed.Get());
							mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX, tilePixelY + 7, colorRed.Get());
						}
						else
						{
							ColorRGB colorRed(255, 0, 0);
							ImageResource hilight(8, 8, colorRed.Get());

							hilight.BlitAlpha(mapPanel_, tilePixelX, tilePixelY, 0.5f);

							mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX + 7, tilePixelY, colorRed.Get());
							mapPanel_->Line(tilePixelX, tilePixelY, tilePixelX, tilePixelY + 7, colorRed.Get());
						}
					}
				}
			}
		}


		InputDevice->MouseEnableCursorDisplay(true);
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::RenderMapWarpIDs()
	{
		// get the base layer
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

		int mapRows 	= baseLayer->GetNumRows();
		int mapColumns 	= baseLayer->GetNumColumns();

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

				// draw the tile
				int tilePixelX = (tileX - cameraX_) * 16;
				int tilePixelY = (tileY - cameraY_) * 16;

				// draw a little blue circle on warp tiles

				if (currentMap_->IsWarp(tileX, tileY))
				{
					ColorRGB colorBlue(0, 0, 255);
					ColorRGB magicPink(255, 0, 255);

					ImageResource hilight(16, 16, magicPink.Get());

					hilight.Circle(8, 8, 3, colorBlue.Get(), true);

					hilight.BlitSprite(mapPanelOverlay_, tilePixelX, tilePixelY);

					BitmapFont t; t.Print(mapPanelOverlay_,
					tilePixelX, tilePixelY, "%d", currentMap_->GetWarp(tileX, tileY));
				}
			}
		}

		InputDevice->MouseEnableCursorDisplay(true);
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::RenderEventIDs()
	{
	}

	void GameMapEditorsSingleton::RenderGrid()
	{
		if (showGrid_)
		{
			ColorRGB colorGrid(0, 0, 0);
			for (int gridY = 0; gridY < cameraH_; gridY++)
			{
				mapPanelOverlay_->Line(0, gridY * 16, cameraW_ * 16, gridY * 16, colorGrid.Get());
			}

			for (int gridX = 0; gridX < cameraW_; gridX++)
			{
				mapPanelOverlay_->Line(gridX * 16, 0, gridX * 16, cameraH_ * 16, colorGrid.Get());
			}
		}
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Render()
	{
		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();

		int mapPanelW = mapPanel_->GetWidth();
		int mapPanelH = mapPanel_->GetHeight();

		int mouseTileX = cameraX_ + (mouseX_ / 16);
		int mouseTileY = cameraY_ + (mouseY_ / 16);

		ColorRGB magicPink(255, 0, 255);
		mapPanelOverlay_->Clear(magicPink.Get());

		if (showGrid_)
		{
			RenderGrid();
		}


		int hudy = (mapPanelH * 2) + 10;

		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer:
			{
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);

				InputDevice->MouseEnableCursorDisplay(true);

				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Editing Base Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());

			} break;

			case MAPEDITORS::EditingMapWarps:
			{
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);

				RenderMapWarpIDs();
				mapPanelOverlay_->BlitMasked(display, 0, 0, 0, 0, mapPanelOverlay_->GetWidth(), mapPanelOverlay_->GetHeight());



				InputDevice->MouseEnableCursorDisplay(true);

				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Editing Map Warps on \"%s\"", currentMap_->GetName().c_str());
				debugFont.Print(display, 1, hudy+12, "Mouse Pos Pixel: (%3d, %3d) Tile:(%3d, %3d)", mouseX_, mouseY_, mouseTileX, mouseTileY);

			} break;

			// we are editing the collision layer of the map
			case MAPEDITORS::EditingCollisionLayer:
			{
				// draw the map
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);
				mapPanelOverlay_->BlitMasked(display, 0, 0, 0, 0, mapPanelOverlay_->GetWidth(), mapPanelOverlay_->GetHeight());

				InputDevice->MouseEnableCursorDisplay(true);

				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Editing Collision Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
				debugFont.Print(display, 1, hudy+12, "Mouse Pos Pixel: (%3d, %3d) Tile:(%3d, %3d)", mouseX_, mouseY_, mouseTileX, mouseTileY);


			} break;

			// we are editing the events on the map
			case MAPEDITORS::EditingEvents:
			{
				InputDevice->MouseEnableCursorDisplay(false);
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);
				InputDevice->MouseEnableCursorDisplay(true);

				RenderEventIDs();
				mapPanelOverlay_->BlitMasked(display, 0, 0, 0, 0, mapPanelOverlay_->GetWidth(), mapPanelOverlay_->GetHeight());

				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Editing Events Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
			} break;

			// we are selecting a tile from the tileset browser
			case MAPEDITORS::SelectingTile:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Select a Tile");
			} break;

			// we are previewing the map
			case MAPEDITORS::Previewing:
			{
				// draw the map
				mapPanel_->Blit(display, 0, 0, mapPanelW, mapPanelH, 0, 0, mapPanelW * 2, mapPanelH * 2);

				BitmapFont debugFont;
				debugFont.Print(display, 1, hudy, "Ingame MapED v1.0 - Previewing Map (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());

			} break;

			default: break;
		} // end switch

		InputDevice->MouseDisplayOnSurface(display);
	}

} // end namespace
