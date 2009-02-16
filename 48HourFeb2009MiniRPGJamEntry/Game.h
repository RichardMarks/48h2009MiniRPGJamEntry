
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
	class GameDialogueMessage;

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
		GameCamera* GetCamera() const; // get a pointer to the camera
		BitmapFont* GetSmallFont() const; // get a pointer to the small font
		BitmapFont* GetLargeFont() const; // get a pointer to the large font
		ImageResource* GetDisplay() const; // get a pointer to the micro display

		void Update();

		void ChangeMap(GameMap* map);

		std::vector<std::string> Tokenize(const std::string& source, const std::string& delimiters);
		void LoadWorldFile(const char* worldPath);

		/// writes out @message to the dialogue display panel
		void Dialogue(const char* message);

		/// clears the dialogue display panel
		void ClearDialogue();

		/// renders the scene
		void Render();

	private:
		GameSingleton();
		GameSingleton(const GameSingleton& rhs);
		const GameSingleton& operator=(const GameSingleton& rhs);

	private:

		/// really basic starfield code for the lofi logo area just to add something cool to distract the player LOL
		struct Star
		{
			int screenX_, screenY_;
			int speedY_;
			int color_;
		};

		class Starfield
		{
		public:
			/// expects an array of 4 ints defining the left, top, right, and bottom clipping planes
			Starfield(int* fov, int count = 50, int maxSpeed = 2);
			~Starfield();

			void Update();
			void Render(ImageResource* target);

		private:
			int* fov_;
			Star* stars_;
			int starCount_;
		};

		Starfield* starfield_;


		void CheckMapEvents(int scanX, int scanY);

		// returns true if there is a collision between the player sprite and any npc on the current map
		bool CheckNPCCollisions() const;


		void Destroy();

		ImageResource* microDisplay_; 		// the 200x150 display buffer that we scale x4 -- draw to this
		BitmapFont* smallFont_; 			// a 5x5 font (upper and lowercase)
		BitmapFont* largeFont_; 			// a 8x8 font (uppercase only)
		GameCamera* camera_; 				// the camera

		ImageResource* windowOverlay_; 		// cool borders
		ImageResource* lofiOverlay_; 		// the 48 hour LOFI jam logo
		ImageResource* playerPortrait_; 	// the player's portrait

		GameTilesetManager* gameTiles_;		// manages the tilesets
		GameMapManager* gameMaps_;			// manages the maps

		GameMap* currentMap_;				// a pointer to the current map

		GameMapSpriteManager* gameSprites_;	// manages the sprites
		GameNPCManager* gameNPCs_; 			// manages the NPCs

		int playerSpriteIndex_;

		bool requestToClearDialogueRegion_; // should the render pass clear the dialogue region?

		GameDialogueMessage* dialogueMessage_; // the game dialogue message handling class instance

		/// number of steps the player has taken
		int stepsTaken_;

		/// number of steps to initiate a random monster encounter
		int stepsUntilAmbush_;

	};

	extern volatile int allegroTimerSpeedCounter;
	extern void allegroTimerSpeedController();

} // end namespace
#endif


