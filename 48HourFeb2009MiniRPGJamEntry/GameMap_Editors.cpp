
// CODESTYLE: v2.0

// GameMap_Editors.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: singleton class that implements the in-game map editors

#include "GameLibrary.h"

#include "GameMap_Editors.h"

namespace GAME
{
	GameMapEditorsSingleton* GameMapEditorsSingleton::GetInstance()
	{
		static GameMapEditorsSingleton instance;
		return &instance;
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::SetState(MAPEDITORS::EditorState state)
	{
		state_ = state;
	}

	/**************************************************************************/

	MAPEDITORS::EditorState GameMapEditorsSingleton::GetState() const
	{
		return state_;
	}

	/**************************************************************************/

	GameMapEditorsSingleton::GameMapEditorsSingleton()
	{
	}

	/**************************************************************************/

	GameMapEditorsSingleton::~GameMapEditorsSingleton()
	{
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::SetMap(GameMap* theMap)
	{
		// point the map pointer XD
		currentMap_ = theMap;

		// grab the base layer of the map
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

		// get dimensions in pixel and tile sizes
		int mapWidth 	= baseLayer->GetWidth();
		int mapHeight 	= baseLayer->GetHeight();
		int mapRows 	= baseLayer->GetNumRows();
		int mapColumns 	= baseLayer->GetNumColumns();

		// create the new map panel
		if (mapPanel_) { delete mapPanel_; }
		mapPanel_ = new ImageResource(mapWidth, mapHeight);

		// make sure we don't have a camera larger than the map!
		if (cameraW_ > mapColumns) { cameraW_ = mapColumns; }
		if (cameraH_ > mapRows) { cameraH_ = mapRows; }

		// make sure the camera is not positioned off the map!
		if (cameraX_ > mapColumns - cameraW_) { cameraX_ = mapColumns - cameraW_; }
		if (cameraY_ > mapRows - cameraH_) { cameraY_ = mapRows - cameraH_; }

		// update the map panel
		RenderMap();
	}

	/**************************************************************************/

	bool GameMapEditorsSingleton::Initialize()
	{
		GameSingleton* game = GameSingleton::GetInstance();

		// get shared pointers
		microDisplay_ 	= game->GetDisplay();
		camera_ 		= game->GetCamera();
		tiles_ 			= game->GetTilesetManager();
		maps_			= game->GetMapManager();

		mapPanel_ 		= new ImageResource();

		cameraX_ = 0;
		cameraY_ = 0;
		cameraW_ = 20;
		cameraH_ = 15;

		return true;
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Destroy()
	{
		if (mapPanel_) { delete mapPanel_; }

		InputDevice->MouseDisplayOnScreen(false);
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Update()
	{
		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();

		int displayWidth = display->GetWidth();
		int displayHeight = display->GetHeight();

/*#**************************************************************************#*/

		if (InputDevice->KeyPressed(KEY::Key_Tab))
		{
			switch(state_)
			{
				case MAPEDITORS::EditingBaseLayer: 		{ state_ = MAPEDITORS::EditingCollisionLayer; } break;
				case MAPEDITORS::EditingCollisionLayer: { state_ = MAPEDITORS::EditingEvents; } break;
				case MAPEDITORS::EditingEvents: 		{ state_ = MAPEDITORS::SelectingTile; } break;
				case MAPEDITORS::SelectingTile: 		{ state_ = MAPEDITORS::Previewing; } break;
				case MAPEDITORS::Previewing: 			{ state_ = MAPEDITORS::EditingBaseLayer; } break;
				default:break;
			}
		}

		if (MAPEDITORS::SelectingTile != state_)
		{
			// keyboard controls for map

			GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

			int mapRows 	= baseLayer->GetNumRows();
			int mapColumns 	= baseLayer->GetNumColumns();

			if (InputDevice->KeyDown(KEY::Key_Up))
			{
				// if the camera has not reached the top of the map, move it north
				if (cameraY_ > 0)
				{
					cameraY_--;
					// update the map panel
					RenderMap();
				}
			}
			else if (InputDevice->KeyDown(KEY::Key_Down))
			{
				// if the camera has not reached the bottom of the map, move it south
				if (cameraY_ + cameraH_ < mapRows)
				{
					cameraY_++;
					// update the map panel
					RenderMap();
				}
			}
			else if (InputDevice->KeyDown(KEY::Key_Left))
			{
				// if the camera has not reached the left edge of the map, move it west
				if (cameraX_ > 0)
				{
					cameraX_--;
					// update the map panel
					RenderMap();
				}
			}
			else if (InputDevice->KeyDown(KEY::Key_Right))
			{
				// if the camera has not reached the right edge of the map, move it east
				if (cameraX_ + cameraW_ < mapColumns)
				{
					cameraX_++;

					// update the map panel
					RenderMap();
				}
			}
		}
		else
		{
			// keyboard controls for tile browser
		}
/*#**************************************************************************#*/

		mouseX_ = InputDevice->MouseX();
		mouseY_ = InputDevice->MouseY();

		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer: {} break;

/*#**************************************************************************#*/

			// we are editing the collision layer of the map
			case MAPEDITORS::EditingCollisionLayer:
			{
				// only process clicks on the inside of the map panel
				if (mouseX_ > 3 && mouseX_ < (cameraW_ * 16) - 3 && mouseY_ > 3 && mouseY_ < (cameraH_ * 16) - 3)
				{
					// if we have not clicked already
					if (!mouseClicked_)
					{
						// check for left mouse button being down
						if (InputDevice->MouseButtonDown(1))
						{
							// its down
							mouseDown_ = true;
						}

						if (InputDevice->MouseButtonUp(1))
						{
							// its up
							if (mouseDown_)
							{
								//fprintf(stderr, "Clicked!\n");
								// we clicked
								mouseClicked_ = true;
								mouseDown_ = false;
							}
						}
					}
					else
					{
						// we have clicked
						mouseClicked_ = false;

						// where did we click?
						int tileX = cameraX_ + (mouseX_ / 16);
						int tileY = cameraY_ + (mouseY_ / 16);

						// toggle the solidness of the tile
						currentMap_->SetTileSolid(tileX, tileY, !currentMap_->IsSolid(tileX, tileY));

						// update the map panel
						RenderMap();
					}
				}

			} break;

/*#**************************************************************************#*/

			// we are editing the events on the map
			case MAPEDITORS::EditingEvents: {} break;

/*#**************************************************************************#*/

			// we are selecting a tile from the tileset browser
			case MAPEDITORS::SelectingTile: {} break;

/*#**************************************************************************#*/

			// we are previewing the map
			case MAPEDITORS::Previewing: {} break;

/*#**************************************************************************#*/

			default: break;
		} // end switch
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::RenderMap()
	{
		// render the map to the mapPanel_

		// get the tileset of the current map
		GameTileset* tileset = currentMap_->GetTileset();

		// get the base layer
		GameMapLayer* baseLayer = currentMap_->GetGameMapLayer(0);

		int mapRows 	= baseLayer->GetNumRows();
		int mapColumns 	= baseLayer->GetNumColumns();

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

				if (currentMap_->IsSolid(tileX, tileY))
				{
					ColorRGB colorRed(255, 0, 0);
					mapPanel_->Rect(tilePixelX, tilePixelY, tilePixelX + 7, tilePixelY + 7, colorRed.Get());
				}
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

		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Base Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
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


