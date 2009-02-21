
// CODESTYLE: v2.0

// GameBattle_Initialize.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: Implementation of the Initialization methods for the BattleSingleton class

#include "GameLibrary.h"

namespace GAME
{
	bool BattleEngineSingleton::Initialize()
	{
		// init the shared resource pointers
		smallFont_ 		= GameSingleton::GetInstance()->GetSmallFont();
		largeFont_ 		= GameSingleton::GetInstance()->GetLargeFont();
		microDisplay_ 	= GameSingleton::GetInstance()->GetDisplay();

		// was there any trouble with the shared resource pointers?
		if (!smallFont_ || !largeFont_ || !microDisplay_)
		{
			// log the error
			LogError("Battle System Initializing:\n"
			"Small Font %s found.\n"
			"Large Font %s found.\n"
			"Micro Display %s found.\n"
			"There were errors initializing the battle system!\n",
			(smallFont_)?"was":"was not",
			(largeFont_)?"was":"was not",
			(microDisplay_)?"was":"was not");

			// return failure
			return false;
		}

		// load the battle system configuration file
		// LoadBattleFile("data/battles/game.battle");

		// initially the battle system is not running, so we set the state to finished
		state_ = BATTLE::Finished;

		// return success
		return true;
	}

	/**************************************************************************/

	void BattleEngineSingleton::InitializeBattleResources()
	{
		// if this is set to false, then at the end of this function, the
		// battle system will jump straight to ending the battle
		bool everythingWentOkay = true;

		// is any resources are already init (should never be the case)
		// then they are destroyed first so that creation does not leak resources
		DestroyBattleResources();


		// init the pointers
		battleSceneImage_ 	= new ImageResource();
		windowOverlay_ 		= new ImageResource();
		cursorOverlay_ 		= new ImageResource();
		battleGaugeOverlay_ = new ImageResource();


		/*#
			the battle system background scene is based off of the zone ID for the tile at the
			player's current map world location.

			there is no zone information setup in the game, so its hard coded for now.
		#*/

		// load up the proper background scenery


		// grab a pointer to the player's sprite
		GameMapSprite* playerSprite = GameSingleton::GetInstance()->GetMapSpriteManager()->Get(GameSingleton::GetInstance()->GetPlayerSpriteIndex());

		// get the player's world position
		int pwx, pwy;
		playerSprite->GetWorldPosition(pwx, pwy);

		// get the value of the base layer tile at the player's world position
		int tileValue = GameSingleton::GetInstance()->GetMap()->GetGameMapLayer(0)->GetTileIndexAt((pwx + 4) / 8, (pwy + 4) / 8);

		// the zone id stuff I mentioned
		int zoneID = 0;
		switch(tileValue)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 11: { zoneID = 0; } break;
			case 12: { zoneID = 1; } break;
			case 20: { zoneID = 1; } break;
			default: break;
		}

		// load the scene imagery already!
		char buffer[256];
		sprintf(buffer, "data/graphics/battle/scenes/%04d.png", zoneID);

		if (!battleSceneImage_->Load(buffer))
		{
			everythingWentOkay = false;
		}



		// load in the overlay images
		if (!windowOverlay_->Load("data/graphics/battle/overlays/window.png"))
		{
			everythingWentOkay = false;
		}

		if (!cursorOverlay_->Load("data/graphics/battle/overlays/cursor.png"))
		{
			everythingWentOkay = false;
		}

		if (!battleGaugeOverlay_->Load("data/graphics/battle/overlays/gauge.png"))
		{
			everythingWentOkay = false;
		}










		// if there were no problems initialzing things
		if (everythingWentOkay)
		{
			// set the state to inbattle to begin the fun
			SetState(BATTLE::InBattle);
		}
		else
		{
			// set the state to endbattle because something went wrong!
			SetState(BATTLE::EndBattle);
		}
	}

	/**************************************************************************/

	void BattleEngineSingleton::DestroyBattleResources()
	{
		#define _TMP_DELRES(resName) if (resName) { delete resName; resName = 0; }

		_TMP_DELRES(battleSceneImage_)
		_TMP_DELRES(windowOverlay_)
		_TMP_DELRES(cursorOverlay_)
		_TMP_DELRES(battleGaugeOverlay_)

		_TMP_DELRES(battleSprites_)
		_TMP_DELRES(battleScenes_)

		#undef _TMP_DELRES
	}

} // end namespace
