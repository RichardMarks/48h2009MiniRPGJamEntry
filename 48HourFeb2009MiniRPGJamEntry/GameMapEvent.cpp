
// CODESTYLE: v2.0

// GameMapEvent.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a map event

#include "GameLibrary.h"


namespace GAME
{
/*
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
*/
	GameMapEvent::GameMapEvent() :
		worldX_(0),
		worldY_(0),
		handler_(0),
		armed_(false),
		persists_(false),
		parentMapIndex_(0),
		targetMapIndex_(0),
		targetX_(0),
		targetY_(0)
	{
	}

	/**************************************************************************/

	GameMapEvent::GameMapEvent(
		int worldX, int worldY, int parentMapIndex,
		GameMapEventHandler* handler,
		bool armed,
		bool persists,
		const char* extra,
		int targetX, int targetY, int targetMapIndex) :
		worldX_(worldX),
		worldY_(worldY),
		handler_(handler),
		armed_(armed),
		persists_(persists),
		parentMapIndex_(parentMapIndex),
		targetMapIndex_(targetMapIndex),
		targetX_(targetX),
		targetY_(targetY)
	{
	}

	/**************************************************************************/

	GameMapEvent::~GameMapEvent()
	{
	}

	/**************************************************************************/

	void GameMapEvent::SetLocation(int parentMapIndex, int x, int y)
	{
		parentMapIndex_ = parentMapIndex;
		worldX_ = x;
		worldY_ = y;
	}

	/**************************************************************************/

	void GameMapEvent::GetLocation(int& storageMapIndex, int& storageX, int& storageY)
	{
		storageMapIndex = parentMapIndex_;
		storageX = worldX_;
		storageY = worldY_;
	}

	/**************************************************************************/

	void GameMapEvent::SetEventHandler(GameMapEventHandler* handler)
	{
		handler_ = handler;
	}

	/**************************************************************************/

	GameMapEventHandler* GameMapEvent::GetEventHandler()
	{
		return handler_;
	}

	/**************************************************************************/

	void GameMapEvent::Arm()
	{
		armed_ = true;
	}

	/**************************************************************************/

	void GameMapEvent::Disarm()
	{
		armed_ = false;
	}

	/**************************************************************************/

	bool GameMapEvent::IsArmed()
	{
		return armed_;
	}

	/**************************************************************************/

	void GameMapEvent::Persists(bool persists)
	{
		persists_ = persists;
	}

	/**************************************************************************/

	bool GameMapEvent::IsPersistent()
	{
		return persists_;
	}

	/**************************************************************************/

	void GameMapEvent::SetWarpTargetLocation(int targetMapIndex, int x, int y)
	{
		targetMapIndex_ = targetMapIndex;
		targetX_ = x;
		targetY_ = y;
	}

	/**************************************************************************/

	void GameMapEvent::GetWarpTargetLocation(int& storageMapIndex, int& storageX, int& storageY)
	{
		storageMapIndex = targetMapIndex_;
		storageX = targetX_;
		storageY = targetY_;
	}

	/**************************************************************************/

	void GameMapEvent::SetExtra(const char* extra)
	{
		extra_ = extra;
	}

	/**************************************************************************/

	const std::string& GameMapEvent::GetExtra()
	{
		return extra_;
	}

	/**************************************************************************/

	void GameMapEvent::CloneFrom(GameMapEvent* sourceEvent)
	{

		worldX_ = sourceEvent->worldX_;
		worldY_ = sourceEvent->worldY_;
		handler_ = sourceEvent->handler_;
		armed_ = sourceEvent->armed_;
		persists_ = sourceEvent->persists_;
		parentMapIndex_ = sourceEvent->parentMapIndex_;
		targetMapIndex_ = sourceEvent->targetMapIndex_;
		targetX_ = sourceEvent->targetX_;
		targetY_ = sourceEvent->targetY_;
		extra_ = sourceEvent->extra_;

		/*
		fprintf(stderr,
		"Event 0x%8X Data:\n{\n"
		"\tworldX_ = %d;\n"
		"\tworldY_ = %d;\n"
		"\thandler_ = 0x%8X;\n"
		"\tarmed_ = %s;\n"
		"\tpersists_ = %s;\n"
		"\tparentMapIndex_ = %d;\n"
		"\ttargetMapIndex_ = %d;\n"
		"\ttargetX_ = %d;\n"
		"\ttargetY_ = %d;\n"
		"\textra_ = %s;\n"
		"}\n",
		event->worldX_, event->worldY_, event->handler_); */

	}

	void GameMapEvent::DebugList()
	{
#if 0
		fprintf(stderr,
		"Event Data:\n{\n"
		"\tworldX_ = %d;\n"
		"\tworldY_ = %d;\n"
		"\tarmed_ = %s;\n"
		"\tpersists_ = %s;\n"
		"\tparentMapIndex_ = %d;\n"
		"\ttargetMapIndex_ = %d;\n"
		"\ttargetX_ = %d;\n"
		"\ttargetY_ = %d;\n"
		"\textra_ = \"%s\";\n"
		"}\n",
		worldX_, worldY_, (armed_)?"true":"false",
		(persists_)?"true":"false",
		parentMapIndex_,targetMapIndex_,targetX_,targetY_,extra_.c_str());
#endif
	}

} // end namespace


