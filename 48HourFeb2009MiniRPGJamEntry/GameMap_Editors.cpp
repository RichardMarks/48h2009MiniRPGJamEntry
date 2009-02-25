
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

		SetMap(game->GetMap());

		cameraX_ = 0;
		cameraY_ = 0;
		cameraW_ = 20;
		cameraH_ = 15;

		mouseLBDown_ 		= false;
		mouseLBClicked_ 	= false;
		mouseRBDown_ 		= false;
		mouseRBClicked_ 	= false;
		placedFirst_ 		= false;

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
		//ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();

		//int displayWidth = display->GetWidth();
		//int displayHeight = display->GetHeight();

/*#**************************************************************************#*/

		if (InputDevice->KeyPressed(KEY::Key_Tab))
		{
			switch(state_)
			{
				case MAPEDITORS::EditingBaseLayer: 		{ state_ = MAPEDITORS::EditingCollisionLayer; RenderMap();} break;
				case MAPEDITORS::EditingCollisionLayer: { state_ = MAPEDITORS::EditingMapWarps; RenderMap();} break;
				case MAPEDITORS::EditingMapWarps:		{ state_ = MAPEDITORS::EditingEvents; RenderMap();} break;
				case MAPEDITORS::EditingEvents: 		{ state_ = MAPEDITORS::SelectingTile; } break;
				case MAPEDITORS::SelectingTile: 		{ state_ = MAPEDITORS::Previewing; RenderMap();} break;
				case MAPEDITORS::Previewing: 			{ state_ = MAPEDITORS::EditingBaseLayer; RenderMap();} break;
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
#if 1 /// enable saving collision changes
				if (InputDevice->KeyPressed(KEY::Key_S))
				{
					bool yesNo = DEBUG::DebugAllegroGUI::YesNo("Save Changes?", "Collision Map Editor");

					if (yesNo)
					{
						currentMap_->SaveCollisionData(GameSingleton::GetInstance()->GetMapsDirectory().c_str());
						std::string message = "Saved Collision Data for " + currentMap_->GetName() + ".map";
						DEBUG::DebugAllegroGUI::MessageBox(message.c_str(), "Collision Map Editor");
					}
					else
					{
						DEBUG::DebugAllegroGUI::MessageBox("Changes were not saved.", "Collision Map Editor");
					}
				}
#endif

				// only process clicks on the inside of the map panel
				if (mouseX_ > 0 && mouseX_ < (cameraW_ * 16) && mouseY_ > 0 && mouseY_ < (cameraH_ * 16))
				{

#if 1 /// enable the mouse-click requirment for placing collision tiles
					// if we have not clicked already
					if (!mouseLBClicked_)
					{
						// check for left mouse button being down
						if (InputDevice->MouseButtonDown(1))
						{
							// its down
							mouseLBDown_ = true;
						}

						if (InputDevice->MouseButtonUp(1))
						{
							// its up
							if (mouseLBDown_)
							{
								//fprintf(stderr, "Clicked!\n");
								// we clicked
								mouseLBClicked_ = true;
								mouseLBDown_ = false;
							}
						}
					}
					else
					{
						// we have clicked
						mouseLBClicked_ = false;

						// where did we click?
						int tileX = cameraX_ + (mouseX_ / 16);
						int tileY = cameraY_ + (mouseY_ / 16);

						// toggle the solidness of the tile
						currentMap_->SetTileSolid(tileX, tileY, !currentMap_->IsSolid(tileX, tileY));

						// update the map panel
						RenderMap();
					}
#endif
				}

			} break;

/*#**************************************************************************#*/

			// we are editing the map warps of the game
			case MAPEDITORS::EditingMapWarps:
			{
#if 1 /// enable saving map warp changes
				if (InputDevice->KeyPressed(KEY::Key_S))
				{
					bool yesNo = DEBUG::DebugAllegroGUI::YesNo("Save Changes?", "Map Warp Editor");

					if (yesNo)
					{
						currentMap_->SaveMapWarpData(GameSingleton::GetInstance()->GetMapsDirectory().c_str());
						std::string message = "Saved Map Warp Data for " + currentMap_->GetName() + ".map";
						DEBUG::DebugAllegroGUI::MessageBox(message.c_str(), "Map Warp Editor");
					}
					else
					{
						DEBUG::DebugAllegroGUI::MessageBox("Changes were not saved.", "Map Warp Editor");
					}
				}
#endif
				// only process clicks on the inside of the map panel
				if (mouseX_ > 0 && mouseX_ < (cameraW_ * 16) && mouseY_ > 0 && mouseY_ < (cameraH_ * 16))
				{
#if 1 /// handle the left mouse button code
					// if we have not clicked already
					if (!mouseLBClicked_)
					{
						// check for left mouse button being down
						if (InputDevice->MouseButtonDown(1))
						{
							// its down
							mouseLBDown_ = true;
						}

						if (InputDevice->MouseButtonUp(1))
						{
							// its up
							if (mouseLBDown_)
							{
								//fprintf(stderr, "Clicked!\n");
								// we clicked
								mouseLBClicked_ = true;
								mouseLBDown_ = false;
							}
						}
					}
					else
					{
						// we have clicked
						mouseLBClicked_ = false;

						// where did we click?
						int tileX = cameraX_ + (mouseX_ / 16);
						int tileY = cameraY_ + (mouseY_ / 16);

						// we do not place a new warp if there is already a warp at this tile
						if (!currentMap_->IsWarp(tileX, tileY))
						{
							// are we placing the "from" or the "to" warp target?
							if (!placedFirst_)
							{
								// first warp
								WarpTarget temp(tileX, tileY, currentMap_->GetID());
								warpFrom_.Copy(temp);

								placedFirst_ = true;

								// show a list of available maps
								GUIListBox mapList;
								unsigned int numMaps = maps_->GetNumMaps();
								for (unsigned int index = 0; index < numMaps; index++)
								{
									mapList.Add(maps_->Get(index)->GetName().c_str());
								}
								mapList.Show();
								DEBUG::DebugAllegroGUI::MessageBox("Starting point marked. Please place ending point now.", "Map Warp Editor");

								// save a pointer to the current map
								previousMap_ = currentMap_;

								// save the camera position
								previousCameraX_ = cameraX_;
								previousCameraY_ = cameraY_;

								// load the target map in
								SetMap(maps_->Get(mapList.GetSelection().c_str()));
							}
							else
							{
								// second
								WarpTarget temp(tileX, tileY, currentMap_->GetID());
								warpTo_.Copy(temp);

								char msgbuffer[0x400] = {0};
								sprintf(msgbuffer, "Added Warp from %s @ (%d, %d) to %s @ (%d, %d)",
								maps_->Get(warpFrom_.targetMapID_)->GetName().c_str(),
								warpFrom_.worldX_, warpFrom_.worldY_,
								maps_->Get(warpTo_.targetMapID_)->GetName().c_str(),
								warpTo_.worldX_, warpTo_.worldY_);
								DEBUG::DebugAllegroGUI::MessageBox(msgbuffer, "Map Warp Editor");

								// create the warp bridge
								previousMap_->SetWarp(warpFrom_.worldX_, warpFrom_.worldY_, previousMap_->GetNumWarpTargetPairs());
								previousMap_->AddWarpTargetPair(
									warpFrom_.worldX_,
									warpFrom_.worldY_,
									warpFrom_.targetMapID_,
									warpTo_.worldX_,
									warpTo_.worldY_,
									warpTo_.targetMapID_);

								currentMap_->SetWarp(warpTo_.worldX_, warpTo_.worldY_, currentMap_->GetNumWarpTargetPairs());
								currentMap_->AddWarpTargetPair(
									warpTo_.worldX_,
									warpTo_.worldY_,
									warpTo_.targetMapID_,
									warpFrom_.worldX_,
									warpFrom_.worldY_,
									warpFrom_.targetMapID_);

								// restore the previous map we were on
								SetMap(previousMap_);

								// restore camera position
								cameraX_ = previousCameraX_;
								cameraY_ = previousCameraY_;

								placedFirst_ = false;
							}
						}
						else
						{
							// tell the user that they cannot place a warp on another warp
							DEBUG::DebugAllegroGUI::MessageBox("You cannot place warps on top of existing warps!", "Map Warp Editor");
						}

						// update the map panel
						RenderMap();
					}
#endif /// end handle left mouse button

#if 1 /// handle the right mouse button code
					// if we have not clicked already
					if (!mouseRBClicked_)
					{
						// check for left mouse button being down
						if (InputDevice->MouseButtonDown(2))
						{
							// its down
							mouseRBDown_ = true;
						}

						if (InputDevice->MouseButtonUp(2))
						{
							// its up
							if (mouseRBDown_)
							{
								//fprintf(stderr, "Clicked!\n");
								// we clicked
								mouseRBClicked_ = true;
								mouseRBDown_ = false;
							}
						}
					}
					else
					{
						// we have clicked
						mouseRBClicked_ = false;

						// where did we click?
						int tileX = cameraX_ + (mouseX_ / 16);
						int tileY = cameraY_ + (mouseY_ / 16);

						// if there is a warp here, we edit it
						if (currentMap_->IsWarp(tileX, tileY))
						{
							// display the warp editor

							GUITextEntryDialog editWarpTextBox(
								"Please enter a new warp definition in the format:\n"
								"[from-map-name]:[from-x]:[from-y]:[to-map-name]:[to-x]:[to-y]\n"
								"For example: Town:4:4:World:19:16",
								"Map Warp Editor");

							editWarpTextBox.Show();

							// did the user enter a value?
							if ("" == editWarpTextBox.GetText())
							{
								// remove the warp
								DEBUG::DebugAllegroGUI::MessageBox("TODO: remove the Map Warp", "Map Warp Editor");
							}
							else
							{
								// update the warp
								DEBUG::DebugAllegroGUI::MessageBox("TODO: Update the Map Warp", "Map Warp Editor");
							}
						}

						// update the map panel
						RenderMap();
					}
#endif /// end handle right mouse button


				} // end mouse is on map panel
			} break;

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

} // end namespace


