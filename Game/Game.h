
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

namespace UTILITY { namespace CONFIGURATION { class Settings; }}

/// all game related classes are contained within this namespace
namespace GAME
{
	using ENGINE::ImageResource;
	using ENGINE::BitmapFont;

	using UTILITY::CONFIGURATION::Settings;

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

			/// map editor
			MapEditor,

			/// event editor
			EventEditor

		};

		/// a class for managing the state of the game
		class GameStateManager
		{
		public:
			/// constructor
			GameStateManager();

			/// destructor
			~GameStateManager();

			/// sets the state
			void SetState(StateType state);

			/// gets the state
			StateType GetState() const;

		private:
			/// hidden copy constructor
			GameStateManager(const GameStateManager& rhs);

			/// hidden assignment operator
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
		/// constructor
		GameMenuManager();

		/// destructor
		~GameMenuManager();

		/// updates the menu system
		void Update();

		/// renders the menu system
		void Render();

	private:
		/// hidden copy constructor
		GameMenuManager(const GameMenuManager& rhs);

		/// hidden assignment operator
		const GameMenuManager& operator=(const GameMenuManager& rhs);
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

		/// sets the directory that tiles are stored in
		void SetTilesDirectory(const char* path);

		/// sets the directory that maps are stored in
		void SetMapsDirectory(const char* path);

		/// sets the directory that sprites are stored in
		void SetSpritesDirectory(const char* path);

		/// gets the directory that tiles are stored in
		std::string GetTilesDirectory() const;

		/// gets the directory that maps are stored in
		std::string GetMapsDirectory() const;

		/// gets the directory that sprites are stored in
		std::string GetSpritesDirectory() const;

		// the new configuration handling

		/// sets a game setting
		void SetSetting(const char* setting, const char* value);

		/// gets a game setting
		std::string GetSetting(const char* setting);

		/// gets a pointer to the game settings
		Settings* GetSettings() const;

		/// saves the game state
		void SaveGame();

		/// loads the game state
		void LoadGame();


/*#**************************************************************************#*/
		/// constructor
		GameSingleton();

	private:


		/// hidden copy constructor
		GameSingleton(const GameSingleton& rhs);

		/// hidden assignment operator
		const GameSingleton& operator=(const GameSingleton& rhs);

	private:

		/// a small structure for holding data about the NPC that is being interacted with
		struct NPCInteractionData
		{
			/// the index into the gameNPCs_ of the NPC that we are interacting with
			unsigned int npcIndex_;

			/// the direction that the NPC was facing before we interacted with it
			int npcFacing_;

			/// is the NPC currently in use? (busy)
			bool npcBusy_;

			/// constructor
			NPCInteractionData() : npcIndex_(0) , npcFacing_(0), npcBusy_(false) {}
		};

		/// used for NPC interations
		NPCInteractionData npcInteractionData_;

		/// checks the current map at the location specified for events
		void CheckMapEvents(int scanX, int scanY);

		/// returns true if there is a collision between the player sprite and any npc on the current map
		bool CheckNPCCollisions() const;

		/// returns > -1 if there is no NPC on the specified tile on the current map; and the index number of the NPC if there is
		int CheckNPCCollisions(int tileX, int tileY) const;

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

		/// manages the game settings
		Settings* gameSettings_;

// all of this will be removed after the 48 hour jam entry
#if defined(FORTYEIGHTHOUR_JAM_ENTRY_VERSION)

		/// the 48 hour LOFI jam logo
		ImageResource* lofiOverlay_;
#endif

	};

	extern GameSingleton* globalGameInstance;

	/// allegro hw independent timing code
	extern volatile int allegroTimerSpeedCounter;

	/// allegro hw independent timing code
	extern void allegroTimerSpeedController();

} // end namespace
#endif


