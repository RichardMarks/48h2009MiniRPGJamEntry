
// CODESTYLE: v2.0

// GameNPCManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: class to manage a list of NPC pointers

#ifndef __GAMENPCMANAGER_H__
#define __GAMENPCMANAGER_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;

	class GameNPC;

	class GameNPCManager
	{
	public:
		GameNPCManager();
		~GameNPCManager();

		void Clear();

		unsigned int Add(GameNPC* npc, const char* parentMapName);

		GameNPC* Get(unsigned int index);

		const unsigned int GetNumNPCs();

		bool IsNPCOnMap(unsigned int index, const char* mapName) const;

		void Update(const char* mapName);
		void Render(const char* mapName, ImageResource* target, int* fov = 0);

		void Pause(unsigned int index);
		void Resume(unsigned int index);

		void List(FILE* target);

	private:
		GameNPCManager(const GameNPCManager& rhs);
		const GameNPCManager& operator=(const GameNPCManager& rhs);

	private:
		std::vector<GameNPC*> npcs_;
		std::vector<std::string> maps_;
	};

} // end namespace
#endif


