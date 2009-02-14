
// CODESTYLE: v2.0

// GameMapEventHandler.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: abstract class that serves as a base class for creating map event handler classes

#ifndef __GAMEMAPEVENTHANDLER_H__
#define __GAMEMAPEVENTHANDLER_H__

namespace GAME
{
	class GameMapEvent;
	
	class GameMapEventHandler
	{
	public:
		GameMapEventHandler() { };
		virtual ~GameMapEventHandler() { };
		
		virtual bool Initialize(GameMapEvent* event) = 0;
		virtual void Execute() = 0;
		virtual void Destroy() = 0;
		
	private:
		
		GameMapEventHandler(const GameMapEventHandler& rhs);
		const GameMapEventHandler& operator=(const GameMapEventHandler& rhs);
		
	public:
		GameMapEvent* event_;
	};
} // end namespace
#endif


