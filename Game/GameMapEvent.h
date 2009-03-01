
// CODESTYLE: v2.0

// GameMapEvent.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a map event

#ifndef __GAMEMAPEVENT_H__
#define __GAMEMAPEVENT_H__

#include <string>

namespace GAME
{
	class GameMapEventHandler;
	class GameMapEvent
	{
	public:
		GameMapEvent();
		
		GameMapEvent(
			int worldX, int worldY, int parentMapIndex,
			GameMapEventHandler* handler,
			bool armed = true,
			bool persists = true,
			const char* extra = "",
			int targetX = -1, int targetY = -1, int targetMapIndex = -1);
		
		~GameMapEvent();
		
		void SetLocation(int parentMapIndex, int x, int y);
		void GetLocation(int& storageMapIndex, int& storageX, int& storageY);
		
		void SetEventHandler(GameMapEventHandler* handler);
		GameMapEventHandler* GetEventHandler();
		
		void Arm();
		void Disarm();
		bool IsArmed();
		
		void Persists(bool persists = true);
		bool IsPersistent();
		
		void SetWarpTargetLocation(int targetMapIndex, int x, int y);
		void GetWarpTargetLocation(int& storageMapIndex, int& storageX, int& storageY);
		
		void SetExtra(const char* extra);
		const std::string& GetExtra();
		
		void CloneFrom(GameMapEvent* sourceEvent);
		
		void DebugList();
		
	private:
		GameMapEvent(const GameMapEvent& rhs);
		const GameMapEvent& operator=(const GameMapEvent& rhs);
		
	private:
		int worldX_;					// world position of trigger
		int worldY_;					// ..

		GameMapEventHandler* handler_; // pointer to the class instance that has the code to Execute()
		
		bool armed_;					// is this trigger armed?
		bool persists_;					// should the trigger disarm after being activated?
		
		int parentMapIndex_;			// the index of the map that the trigger is placed on
		int targetMapIndex_;			// the index of the map to warp to (warp triggers only)
		int targetX_;					// the world position to warp to
		int targetY_;					// ..
		
		std::string extra_;				// anything extra I might need
	};
} // end namespace
#endif


