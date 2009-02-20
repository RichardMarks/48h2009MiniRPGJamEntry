
// CODESTYLE: v2.0

// GameBattleScene.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a battle background scene

#ifndef __GAMEBATTLESCENE_H__
#define __GAMEBATTLESCENE_H__

#include <vector>
#include <string>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;

	class GameBattleScene
	{
	public:
		GameBattleScene();
		~GameBattleScene();

		/// sets up the scene
		void Setup(const char* name, const char* terrainString, ImageResource* image);

		/// clears and sets up the terrain list -- call with no params to clear the terrain list
		void SetTerrains(const char* terrainString = "");

		/// adds a single tile index value to the terrain list
		void AddTerrain(unsigned int terrain);

		/// sets the name of the scene
		void SetName(const char* name);

		/// sets the image of the scene
		void SetImage(ImageResource* image);

		/// gets a pointer to the scene image
		ImageResource* GetImage() const;

		/// gets the name of the scene
		std::string GetName() const;

		/// returns true if the specified tile index is found in the list of @terrains_
		bool IsFoundOnTerrain(unsigned int terrain);

	private:
		GameBattleScene(const GameBattleScene& rhs);
		const GameBattleScene& operator=(const GameBattleScene& rhs);

	private:

		/// the name of the scene
		std::string name_;

		/// the image of the scene
		ImageResource* image_;

		/// a list of tile index values that will trigger this scene to be used in battle
		std::vector<unsigned int> terrains_;
	};

} // end namespace

#endif

