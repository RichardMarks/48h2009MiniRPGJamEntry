
// CODESTYLE: v2.0

// GameBattle.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: The battle (encounter/fighting) engine

#include "GameLibrary.h"

namespace GAME
{
	BattleEngineSingleton* BattleEngineSingleton::GetInstance()
	{
		static BattleEngineSingleton instance;
		return &instance;
	}

	/**************************************************************************/

	BattleEngineSingleton::~BattleEngineSingleton()
	{

	}

	/**************************************************************************/

	bool BattleEngineSingleton::Initialize()
	{
		//smallFont_ = new BitmapFont();
		//if (!smallFont_->Load("data/graphics/fonts/font5x5white.png", 5, 5,1)) { return false; }

		//largeFont_ = new BitmapFont();
		//if (!largeFont_->Load("data/graphics/fonts/font8x8white.png", 8, 8,1)) { return false; }

		smallFont_ = GameSingleton::GetInstance()->GetSmallFont();
		largeFont_ = GameSingleton::GetInstance()->GetLargeFont();

		battleSceneImage_ = new ImageResource();
		microDisplay_ = new ImageResource(200, 150);

		SetupMonsterParty();
		SetupFightingOrder();
		SetupScene();

		return true;
	}

	/**************************************************************************/

	void BattleEngineSingleton::Execute()
	{
		bool battleOver = false;

		// the color of the scene background
		ColorRGB sceneBackgroundColor(0, 0, 0);
		ColorRGB timeGaugeColor1(0, 170, 133);
		ColorRGB timeGaugeColor2(90, 255, 219);

		ImageResource* display = GraphicsDevice->GetSecondaryDisplayBuffer();
		int dw = display->GetWidth();
		int dh = display->GetHeight();

		ImageResource windowOverlay("data/graphics/battle/overlays/window.png");
		ImageResource cursorOverlay("data/graphics/battle/overlays/cursor.png");
		ImageResource gaugeOverlay("data/graphics/battle/overlays/gauge.png");

		ImageResource playerPortrait("data/graphics/portraits/player.png");

		ImageResource playerFrames("data/graphics/battle/sprites/player.png");

		// test enemy
		ImageResource enemyFrames("data/graphics/battle/sprites/hellheads.png");
		GameGUIGauge enemyAttackAnimTimer(LRHorizontalGUIGauge, 5, 5, 200, 8, 12.0f, 0, 0);
		GameGUIGauge enemyWaitTimer(LRHorizontalGUIGauge, 5, 5, 200, 8, 10.0f, 0, 0);
		bool enemyCanPerform 	= false;
		bool enemyIsAttacking 	= false;
		int enemyFrame 			= 0;
		int enemyFrameCounter 	= 0;
		int enemyFrameDelay 		= 30;
		const int enemyOriginX 			= 44;
		const int enemyOriginY 			= 50;
		const int enemyAtkX 			= 140 - 34;
		const int enemyAtkY 			= 45;
		int enemyPosX 			= 44;
		int enemyPosY 			= 50;
		//

		// timer for the attack animation sequence
		GameGUIGauge playerAttackAnimTimer(LRHorizontalGUIGauge, 5, 5, 200, 8, 8.0f, 0, 0);

		// timer for the player's ability to perform an action
		// when this reaches 0, the player can perform an action
		GameGUIGauge timeGauge(
			LRHorizontalGUIGauge,
			5,
			108 + 17,
			99,
			4,
			4.0f,
			timeGaugeColor1.Get(),
			timeGaugeColor2.Get());

		bool playerCanPerform = false;
		bool playerIsAttacking = false;

		int playerFrame = 0;

		int playerFrameCounter = 0;
		int playerFrameDelay = 15;

		const int playerOriginX = 140;
		const int playerOriginY = 61;

		const int playerAtkX = 70; // some arbitrary position for testing purposes
		const int playerAtkY = 67;

		int playerPosX = 140;
		int playerPosY = 61;


		// action menu stuff
		int actionCursorX = 114;
		int actionCursorY = 109;
		const int cursorYGap = 7;


		// the battle engine main loop
		// the loop ends when the player defeats all enemies or the player dies or successfully escapes
		while(!battleOver)
		{
			// update the input device info
			InputDevice->Update(UPDATE_KEYBOARD | UPDATE_MOUSE);

			// panic key for debugging
			if (InputDevice->KeyPressed(KEY::Key_Escape))
			{
				battleOver = true;
			}

			// RenderScene();



			// begin rendering
			GraphicsDevice->BeginScene(sceneBackgroundColor.Get());
			microDisplay_->Clear();

			// draw the scene background
			battleSceneImage_->Blit(microDisplay_, 0, 0, 4, 4, battleSceneImage_->GetWidth(), battleSceneImage_->GetHeight());

			#if 1
			// test enemy
			// if the enemy is attacking,
			if (enemyIsAttacking)
			{
				// update the attack timer
				enemyAttackAnimTimer.Update(0.09f);

				// if the timer has reached 0
				if (enemyAttackAnimTimer.GetPercentage() == 0.0f)
				{
					// attack finished, return to normal
					enemyFrame = 2;
					enemyAttackAnimTimer.Reset();
					enemyIsAttacking = false;
					enemyWaitTimer.Reset();

					enemyPosX = enemyOriginX;
					enemyPosY = enemyOriginY;
				}
				else
				{
					// still attacking, so animate the attack
					if (++enemyFrameCounter >= enemyFrameDelay)
					{
						playerFrameCounter = 0;
						playerFrame = (playerFrame == 2) ? 1 : 2;
					}
				}
			}
			// if the enemy is not attacking
			else
			{
				// animate the idle animation
				if (++enemyFrameCounter >= enemyFrameDelay)
				{
					enemyFrameCounter = 0;
					enemyFrame = (enemyFrame == 0) ? 1 : 0;
				}
			}

			if (!enemyIsAttacking && !enemyCanPerform)
			{
				// the enemy must wait!
				// update the wait timer
				enemyWaitTimer.Update(0.04f);

				// if the wait timer has reached 0
				if (enemyWaitTimer.GetPercentage() == 0.0f)
				{
					// the enemy can now perform an action
					enemyCanPerform = true;
				}
			}
			//
			#endif


			// if the player is attacking,
			if (playerIsAttacking)
			{
				// update the attack timer
				playerAttackAnimTimer.Update(0.09f);

				// if the timer has reached 0
				if (playerAttackAnimTimer.GetPercentage() == 0.0f)
				{
					// attack finished, return to normal
					playerFrame = 2;
					playerAttackAnimTimer.Reset();
					playerIsAttacking = false;
					timeGauge.Reset();

					playerPosX = playerOriginX;
					playerPosY = playerOriginY;
				}
				else
				{
					// still attacking, so animate the attack
					if (++playerFrameCounter >= playerFrameDelay)
					{
						playerFrameCounter = 0;
						playerFrame = (playerFrame == 2) ? 3 : 2;
					}
				}
			}
			// if the player is not attacking, and can perform an action,
			else if (playerCanPerform)
			{
				// animate the idle animation
				if (++playerFrameCounter >= playerFrameDelay)
				{
					playerFrameCounter = 0;
					playerFrame = (playerFrame == 0) ? 1 : 0;
				}
			}


			// if the player is not attacking, and not able to perform
			if (!playerIsAttacking && !playerCanPerform)
			{
				// the player must wait!
				// update the wait timer
				timeGauge.Update(0.04f);

				// if the wait timer has reached 0
				if (timeGauge.GetPercentage() == 0.0f)
				{
					// the player can now perform an action
					playerCanPerform = true;
				}
				else
				{
					largeFont_->Print(microDisplay_, 132, 110, "WAIT!");
				}
			}

			// draw the player battle sprite and player portrait
			playerPortrait.Blit(microDisplay_, 0, 0, 5, 108, 16, 16);
			playerFrames.BlitMasked(microDisplay_,
				1 + (playerFrame * 16) + (playerFrame), 1,
				playerPosX, playerPosY,
				16, 16);


			// draw the monsters

			// draw the enemy battle sprite
			enemyFrames.BlitMasked(microDisplay_,
				1 + (enemyFrame * 32) + (enemyFrame), 1,
				enemyPosX, enemyPosY,
				32, 32);

			// draw the overlays and other hud details


			int curHp = 220;
			int maxHp = 420;
			smallFont_->Print(microDisplay_, 22, 108, "HP:  %4d/%4d", curHp, maxHp);
			smallFont_->Print(microDisplay_, 22, 108 + 6, "MP:   %3d/ %3d", curHp, maxHp);

			timeGauge.Render(microDisplay_);


			#if 1
			// if the enemy can perform
			if (enemyCanPerform)
			{
				if ( (1 + rand() % (100 - 1)) > 30)
				{
					if (playerIsAttacking)
					{
						enemyCanPerform = false;
						enemyWaitTimer.Reset();
						enemyFrame = 2;
					}
					else
					{
						enemyWaitTimer.Reset();
						enemyIsAttacking = true;
						enemyCanPerform = false;
						enemyPosX = enemyAtkX;
						enemyPosY = enemyAtkY;
					}
				}
				else
				{
					enemyCanPerform = false;
					enemyWaitTimer.Reset();
				}
			}

			//
			#endif


			// if the player can perform then we handle the menu

			if (playerCanPerform && !enemyIsAttacking)
			{
				// handle action menu controls
				if (InputDevice->KeyPressed(KEY::Key_Backspace))
				{
					// forfeit turn
					timeGauge.Reset();
					playerCanPerform = false;
					actionCursorX = 114;
					actionCursorY = 109;
				}

				if (InputDevice->KeyPressed(KEY::Key_Enter))
				{
					// accept selection
					switch(actionCursorY)
					{
						case 109:
						{
							// attack
							timeGauge.Reset();
							playerIsAttacking = true;
							playerCanPerform = false;
							actionCursorX = 114;
							actionCursorY = 109;
							playerPosX = playerAtkX;
							playerPosY = playerAtkY;
						} break;

						case 116:
						{
							// defend
							// defense *= 2;
							timeGauge.Reset();
							playerCanPerform = false;
							actionCursorX = 114;
							actionCursorY = 109;
						} break;

						case 123:
						{
							// magick
							// enter magick selection menu
						} break;

						case 130:
						{
							// use item
							// enter item selection menu
						} break;

						case 137:
						{
							// flee battle
							int chance = 1 + rand() % (5 - 1);
							if (chance == 3 || chance == 4)
							{
								battleOver = true;
							}
							timeGauge.Reset();
							playerCanPerform = false;
							actionCursorX = 114;
							actionCursorY = 109;

						} break;

						default: break;
					}
				}

				if (InputDevice->KeyPressed(KEY::Key_Up))
				{
					if (109 == actionCursorY)
					{
						// at the top of menu, move to bottom
						actionCursorY = 109 + (7 * 4);
					}
					else
					{
						// not at the top, move up
						actionCursorY -= cursorYGap;
					}
				}

				if (InputDevice->KeyPressed(KEY::Key_Down))
				{
					if (109 + (7 * 4) == actionCursorY)
					{
						// at the bottom of menu, move to top
						actionCursorY = 109;
					}
					else
					{
						// not at the bottom, move down
						actionCursorY += cursorYGap;
					}
				}

				// draw action menu
				cursorOverlay.BlitSprite(microDisplay_, actionCursorX, actionCursorY);
				smallFont_->Print(microDisplay_, 114+6, 109, "Attack");
				smallFont_->Print(microDisplay_, 114+6, 109+7, "Defend");
				smallFont_->Print(microDisplay_, 114+6, 109+(7*2), "Magick");
				smallFont_->Print(microDisplay_, 114+6, 109+(7*3), "Use Item");
				smallFont_->Print(microDisplay_, 114+6, 109+(7*4), "Flee Battle");
			}



			gaugeOverlay.BlitMasked(microDisplay_, 0, 0, 46, 5, 109, 26);

			// temporarily draw the player portrait in the gauge window

			windowOverlay.BlitMasked(microDisplay_, 0, 0, 0, 0, 200, 150);

			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4, 10, 16, 16);
			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4 + 17, 10, 16, 16);
			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4 + (17*2), 10, 16, 16);
			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4 + (17*3), 10, 16, 16);
			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4 + (17*4), 10, 16, 16);
			playerPortrait.Blit(microDisplay_, 0, 0, 46 + 4 + (17*5), 10, 16, 16);

			// draw the action menu

			// we are done rendering
			// end the scene using special 4x scaling
			microDisplay_->Blit(display, 0, 0, 200, 150, 0, 0, dw, dh);
			GraphicsDevice->EndScene();




			// let the cpu rest to keep from using 100% CPU for no damn reason
			rest(10);
		}

		if (!rewards_.empty())
		{
			GiveRewards();
		}

		Destroy();
	}

	/**************************************************************************/

	void BattleEngineSingleton::SetupMonsterParty()
	{
		/*
			check tile that player is on to get a list of monsters that can be pooled
			get a random count of monsters to fight between 1 and 5
			create an enemy list with that number of 'slots'
			select a random monster from the 'pool' and add to the enemy party until we have all slots filled
			based on the number of battles the player has won, select a difficulty multiplier and then
			apply that multiplier to the base stats of each monster in the enemy party
		*/

	}

	/**************************************************************************/

	void BattleEngineSingleton::SetupFightingOrder()
	{
		/*
			sort the combatants by their level, then by their speed into the combatant-list
		*/
	}

	/**************************************************************************/

	void BattleEngineSingleton::SetupScene()
	{
		/*
			check the tile that the player is on to get a battle scene bg
		*/


		#if 0

		// grab a pointer to the player entity
		GameEntity* playerEntity = GameEngine->GetPlayer();

		// get the value of the base layer tile at the player's world position
		int tileValue = GameEngine->GetMap()->GetGameMapLayer(0)->GetTileIndexAt(playerEntity->GetWorldX(), playerEntity->GetWorldY());

		// get the tileset information for the tile
		GameTilesetInfo* playerCellInfo = GameEngine->GetTilesetInfo(tileValue);

		// get the id number of the zone
		int zoneID = playerCellInfo->GetZoneID();

		// load the scene image
		char buffer[256];
		sprintf(buffer, "data/graphics/battle/scenes/%04d.png", zoneID);
		battleSceneImage_->Load(buffer);

		#endif


		GameMapSprite* playerSprite = GameSingleton::GetInstance()->GetMapSpriteManager()->Get(GameSingleton::GetInstance()->GetPlayerSpriteIndex());

		int pwx, pwy;
		playerSprite->GetWorldPosition(pwx, pwy);

		// get the value of the base layer tile at the player's world position
		int tileValue = GameSingleton::GetInstance()->GetMap()->GetGameMapLayer(0)->GetTileIndexAt((pwx + 4) / 8, (pwy + 4) / 8);

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

		char buffer[256];
		sprintf(buffer, "data/graphics/battle/scenes/%04d.png", zoneID);
		battleSceneImage_->Load(buffer);

		// battleSceneImage_->Load("data/graphics/battle/scenes/0002.png");

	}

	void UselessDummyFunct()
	{
		#if 0

		GameTilesetInfo* cellInfo = GameEngine->GetTilesetInfo(tileValue);

		std::string zoneName 	= cellInfo->GetZoneName();
		int zoneID 				= cellInfo->GetZoneID();
		bool isSolid 			= cellInfo->IsSolid();


		#endif
	}

	/**************************************************************************/

	void BattleEngineSingleton::RenderScene()
	{

	}

	/**************************************************************************/

	void BattleEngineSingleton::PlayerTurn()
	{
	}

	/**************************************************************************/

	void BattleEngineSingleton::EnemyTurn()
	{
	}

	/**************************************************************************/

	void BattleEngineSingleton::GiveRewards()
	{
	}

	/**************************************************************************/

	void BattleEngineSingleton::Destroy()
	{
		delete battleSceneImage_;
		delete microDisplay_;
		//delete smallFont_;
		//delete largeFont_;
	}

	/**************************************************************************/

	BattleEngineSingleton::BattleEngineSingleton()
	{
	}
} // end namespace


