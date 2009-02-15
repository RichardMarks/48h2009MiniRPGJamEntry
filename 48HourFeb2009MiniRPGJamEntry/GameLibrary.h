
// CODESTYLE: v2.0

// GameLibrary.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

#ifndef __GAMELIBRARY_H__
#define __GAMELIBRARY_H__

// so we don't need to prefix the ENGINE:: namespace scope to everything
#define GED101_USE_SIMPLE_NAMESPACES

// windows build bug patch
#if defined(WIN_32)
#define DWORD signed long long
#endif

// include the ged101 minimal header
#include "ged101minimal.h"

#include "GameCamera.h"
#include "GameMapLayer.h"
#include "GameMap.h"
#include "GameTileset.h"

#include "GameMapEvent.h"
#include "GameMapEventHandler.h"
#include "GameBasicMapWarp.h"

#include "GameObject.h"
#include "GameMapSprite.h"
#include "GameNPC.h"

#include "GameMapManager.h"
#include "GameTilesetManager.h"
#include "GameMapSpriteManager.h"
#include "GameNPCManager.h"

#include "GameGUIGaugeWidget.h"
#include "GameBattle.h"
#include "GameDialogueMessage.h"

#include "Game.h"

#endif

