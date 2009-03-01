
// CODESTYLE: v2.0

// GameBasicMapWarp.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: a simple map to map transition event handler singleton class

#ifndef __GAMEBASICMAPWARP_H__
#define __GAMEBASICMAPWARP_H__

#include "GameMapEventHandler.h"

namespace GAME
{
	class GameMapEvent;
	
	class GameBasicMapWarp : public GameMapEventHandler
	{
	public:

		virtual ~GameBasicMapWarp();
		
		virtual bool Initialize(GameMapEvent* event);
		virtual void Execute();
		virtual void Destroy();
		
		static GameBasicMapWarp* GetInstance();
		
	private:
		GameBasicMapWarp();
		GameBasicMapWarp(const GameBasicMapWarp& rhs);
		const GameBasicMapWarp& operator=(const GameBasicMapWarp& rhs);
	};
	
} // end namespace
#endif


