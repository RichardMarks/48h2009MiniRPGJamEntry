
// CODESTYLE: v2.0

// Game.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the primary game singleton class

#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <string>

// comment this out to build a non 48-hour version later
// to exclude the 48-hour demo stuff like the lofi logo and
// the starfield
#define FORTYEIGHTHOUR_JAM_ENTRY_VERSION

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


/*#**************************************************************************#*/

	/// this namespace holds the game state handling stuff
	namespace GAMESTATE
	{
		/// the states that the game can be in
		enum StateType
		{
			/// the game will process the main game world logic
			World,

			/// the game will process the battle system logic
			Battle,

			/// the game will process the menu system logic
			Menu,
		};

		/// a class for managing the state of the game
		class GameStateManager
		{
		public:
			GameStateManager();
			~GameStateManager();

			/// sets the state
			void SetState(StateType state);

			/// gets the state
			StateType GetState() const;

		private:
			GameStateManager(const GameStateManager& rhs);
			const GameStateManager& operator=(const GameStateManager& rhs);

		private:

			/// the current state
			StateType state_;
		};
	};

/*#**************************************************************************#*/

	/// temporary menu manager class shell just to get the code to compile
	class GameMenuManager
	{
	public:
		GameMenuManager();
		~GameMenuManager();
		void Update();
		void Render();
	private:
	};

	/// the main game class
	class GameSingleton
	{
	public:
		/// gets a pointer to the singleton instance of the class
		static GameSingleton* GetInstance();

		/// class destructor
		~GameSingleton();

/*#**************************************************************************#*/

		/// initializes the game
		bool Initialize(int argc, char* argv[]);

		/// starts the main game loop
		void Execute();

		/// changes the game state
		void SetState(GAMESTATE::StateType state);

		/// gets the game state
		GAMESTATE::StateType GetState() const;

		 /// gets a pointer to the tileset manager
		GameTilesetManager* GetTilesetManager() const;

		/// gets a pointer to the map manager
		GameMapManager* GetMapManager() const;

		/// gets a pointer to the map sprite manager
		GameMapSpriteManager* GetMapSpriteManager() const;

		/// gets a pointer to the npc manager
		GameNPCManager* GetNPCManager() const;

		/// gets the player sprite index value
		int GetPlayerSpriteIndex() const;

		 /// gets a pointer to the current map
		GameMap* GetMap() const;

		/// gets a pointer to the camera
		GameCamera* GetCamera() const;

		/// gets a pointer to the small font
		BitmapFont* GetSmallFont() const;

		/// gets a pointer to the large font
		BitmapFont* GetLargeFont() const;

		/// gets a pointer to the micro display
		ImageResource* GetDisplay() const;

		/// updates the main game logic
		void Update();

		/// changes the current map
		void ChangeMap(GameMap* map);

		/// tokenizes a string into a vector of strings using a list of delimiters
		std::vector<std::string> Tokenize(const std::string& source, const std::string& delimiters);

		/// loads files of the the .World format to initialize the game system
		void LoadWorldFile(const char* worldPath);

		/// writes out @message to the dialogue display panel
		void Dialogue(const char* message);

		/// clears the dialogue display panel
		void ClearDialogue();

		/// renders the main game
		void Render();

/*#**************************************************************************#*/

	private:
		GameSingleton();
		GameSingleton(const GameSingleton& rhs);
		const GameSingleton& operator=(const GameSingleton& rhs);

	private:

		/// checks the current map at the location specified for events
		void CheckMapEvents(int scanX, int scanY);

		/// returns true if there is a collision between the player sprite and any npc on the current map
		bool CheckNPCCollisions() const;

		/// releases all the pointers allocated for the game
		void Destroy();

/*#**************************************************************************#*/

		/// the 200x150 display buffer that we scale x4 -- draw to this
		ImageResource* microDisplay_;

		/// a 5x5 font (upper and lowercase)
		BitmapFont* smallFont_;

		/// a 8x8 font (uppercase only)
		BitmapFont* largeFont_;

		/// the camera used to display a region of the current map
		GameCamera* camera_;

		/// the cool looking borders are really just an image overlaid
		ImageResource* windowOverlay_;

		/// the player's portrait image
		ImageResource* playerPortrait_;

		/// manages the tilesets
		GameTilesetManager* gameTiles_;

		/// manages the maps
		GameMapManager* gameMaps_;

		/// a pointer to the current map
		GameMap* currentMap_;

		/// manages the sprites
		GameMapSpriteManager* gameSprites_;

		/// manages the NPCs
		GameNPCManager* gameNPCs_;

		/// the game dialogue system
		GameDialogueMessage* dialogueMessage_;

		/// the game menu system
		GameMenuManager* gameMenu_;

		/// manages the state that the game is in
		GAMESTATE::GameStateManager* gameStateManager_;

		/// the index into the game sprite manager that holds the player's sprite
		int playerSpriteIndex_;

		/// should the render pass clear the dialogue region?
		bool requestToClearDialogueRegion_;

		/// number of steps the player has taken
		int stepsTaken_;

		/// number of steps to initiate a random monster encounter
		int stepsUntilAmbush_;

/*#**************************************************************************#*/

// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		/// the 48 hour LOFI jam logo
		ImageResource* lofiOverlay_;

		/// really basic starfield code for the lofi logo area just to add something cool to distract the player LOL
		struct Star
		{
			int screenX_, screenY_;
			int speedY_;
			int color_;
		};

		/// a basic starfield class
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

		/// the starfield instance for the lofi logo area
		Starfield* starfield_;
#endif
	};

	/// allegro hw independent timing code
	extern volatile int allegroTimerSpeedCounter;

	/// allegro hw independent timing code
	extern void allegroTimerSpeedController();

} // end namespace
#endif


