
// CODESTYLE: v2.0

// Game.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the primary game singleton class

#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <string>

namespace ENGINE 
{ 
	class ImageResource;
	class BitmapFont;
}

namespace GAME
{
	using ENGINE::ImageResource;
	using ENGINE::BitmapFont;
	
	class GameCamera;
	class GameObject;
	class GameTilesetManager;
	class GameMapManager;
	class GameMap;
	class GameMapSpriteManager;
	class GameNPCManager;
	
	class GameSingleton
	{
	public:
		static GameSingleton* GetInstance();
		~GameSingleton();
		
		bool Initialize(int argc, char* argv[]);
		void Execute();
		
		
		GameTilesetManager* GetTilesetManager() const; // get a pointer to the tileset manager
		GameMapManager* GetMapManager() const; // get a pointer to the map manager
		
		GameMapSpriteManager* GetMapSpriteManager() const; // get a pointer to the map sprite manager
		GameNPCManager* GetNPCManager() const; // get a pointer to the npc manager
		
		int GetPlayerSpriteIndex() const; // get the player sprite index value
		
		GameMap* GetMap() const; // get a pointer to the current map
		// GameObject* GetPlayerObject() const; // get a pointer to the player
		GameCamera* GetCamera() const; // get a pointer to the camera
		BitmapFont* GetSmallFont() const; // get a pointer to the small font
		BitmapFont* GetLargeFont() const; // get a pointer to the large font
		ImageResource* GetDisplay() const; // get a pointer to the micro display
		
		void Update();
		
		void ChangeMap(GameMap* map);
		
		std::vector<std::string> Tokenize(const std::string& source, const std::string& delimiters);
		void LoadWorldFile(const char* worldPath);
		
	private:
		GameSingleton();
		GameSingleton(const GameSingleton& rhs);
		const GameSingleton& operator=(const GameSingleton& rhs);
		
	private:

		void CheckMapEvents(int scanX, int scanY);
	
		
		void Render();
		void Destroy();
		
		ImageResource* microDisplay_; 		// the 200x150 display buffer that we scale x4 -- draw to this
		BitmapFont* smallFont_; 			// a 5x5 font (upper and lowercase)
		BitmapFont* largeFont_; 			// a 8x8 font (uppercase only)
		GameCamera* camera_; 				// the camera
		
		ImageResource* windowOverlay_; 		// cool borders 
		ImageResource* lofiOverlay_; 		// the 48 hour LOFI jam logo
		ImageResource* playerPortrait_; 	// the player's portrait
		
		//GameObject* playerObject_;			// the player's object
		
		GameTilesetManager* gameTiles_;		// manages the tilesets
		GameMapManager* gameMaps_;			// manages the maps
		
		GameMap* currentMap_;				// a pointer to the current map
		
		GameMapSpriteManager* gameSprites_;	// manages the sprites
		GameNPCManager* gameNPCs_; 			// manages the NPCs
		
		int playerSpriteIndex_;
		
		#if 1
		// NPC test code
		void InitializeNPCs();
		void UpdateNPCs();
		void RenderNPCs();
		GameObject* npcObject_;				// test NPC
		#endif
	};
	
	extern volatile int allegroTimerSpeedCounter;
	extern void allegroTimerSpeedController();
	
} // end namespace
#endif


