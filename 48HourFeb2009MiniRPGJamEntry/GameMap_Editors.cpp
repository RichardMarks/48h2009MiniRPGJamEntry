
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

	void GameMapEditorsSingleton::SetMap(GameMap* theMap)
	{
		currentMap_ = theMap;
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

	bool GameMapEditorsSingleton::Initialize()
	{
		microDisplay_ = GameSingleton::GetInstance()->GetDisplay();
		return true;
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Destroy()
	{
		InputDevice->MouseDisplayOnScreen(false);
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Update()
	{
		mouseX_ = InputDevice->MouseX();
		mouseY_ = InputDevice->MouseY();

		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer: {} break;

			// we are editing the object layer of the map
			case MAPEDITORS::EditingObjectLayer: {} break;

			// we are editing the events on the map
			case MAPEDITORS::EditingEvents: {} break;

			// we are selecting a tile from the tileset browser
			case MAPEDITORS::SelectingTile: {} break;

			// we are previewing the map
			case MAPEDITORS::Previewing: {} break;

			default: break;
		} // end switch
	}

	/**************************************************************************/

	void GameMapEditorsSingleton::Render()
	{
		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();
		switch(state_)
		{
			// we are editing the base layer of the map
			case MAPEDITORS::EditingBaseLayer:
			{
				BitmapFont debugFont;
				debugFont.Print(display, 1, 1, "Ingame MapED v1.0 - Editing Base Layer of (%d) \"%s\"", currentMap_->GetID(), currentMap_->GetName().c_str());
			} break;

			// we are editing the object layer of the map
			case MAPEDITORS::EditingObjectLayer: {} break;

			// we are editing the events on the map
			case MAPEDITORS::EditingEvents: {} break;

			// we are selecting a tile from the tileset browser
			case MAPEDITORS::SelectingTile: {} break;

			// we are previewing the map
			case MAPEDITORS::Previewing: {} break;

			default: break;
		} // end switch

		InputDevice->MouseDisplayOnSurface(display);
	}

} // end namespace


