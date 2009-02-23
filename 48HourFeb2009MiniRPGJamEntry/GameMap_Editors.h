
// CODESTYLE: v2.0

// GameMap_Editors.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: singleton class that implements the in-game map editors

#ifndef __GAMEMAP_EDITORS_H__
#define __GAMEMAP_EDITORS_H__

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{

	namespace MAPEDITORS
	{
		enum EditorState
		{
			/// we are editing the base layer of the map
			EditingBaseLayer,

			/// we are editing the collision layer of the map
			EditingCollisionLayer,

			/// we are editing the map warps of the game
			EditingMapWarps,

			/// we are editing the events on the map
			EditingEvents,

			/// we are selecting a tile from the tileset browser
			SelectingTile,

			/// we are previewing the map
			Previewing
		};

	} // end namespace

	class GameMap;
	class GameCamera;

	class GameMapEditorsSingleton
	{
	public:
		static GameMapEditorsSingleton* GetInstance();
		~GameMapEditorsSingleton();

		bool Initialize();

		void Update();
		void Render();

		void Destroy();

		void SetState(MAPEDITORS::EditorState state);
		MAPEDITORS::EditorState GetState() const;

		void SetMap(GameMap* theMap);

	private:
		GameMapEditorsSingleton();
		GameMapEditorsSingleton(const GameMapEditorsSingleton& rhs);
		const GameMapEditorsSingleton& operator=(const GameMapEditorsSingleton& rhs);

	private:

		void RenderMap();

		// shared resource pointers -- the game singleton class is responsible for init/destroy these pointers

		/// shared resource pointer to the display surface of the game system
		ImageResource* microDisplay_;

		/// a shared resource pointer to the game singleton's current map
		GameMap* currentMap_;

		/// a shared resource pointer to the game singleton's camera
		GameCamera* camera_;

		/// a shared resource pointer to the game singleton's tileset manager
		GameTilesetManager* tiles_;

		/// a shared resource pointer to the game singleton's map manager
		GameMapManager* maps_;

		// map editor resource pointers -- the map editor is responsible for init/destroy these pointers

		/// the camera used by the editor
		int cameraX_;
		int cameraY_;
		int cameraW_;
		int cameraH_;

		/// the surface that the entire map is rendered on
		ImageResource* mapPanel_;

		/// the state of the editor
		MAPEDITORS::EditorState state_;

		/// the position of the mouse
		int mouseX_;
		int mouseY_;

		/// have we clicked the left mouse button?
		bool mouseLBClicked_;

		/// is the left mouse button down?
		bool mouseLBDown_;

		/// have we clicked the right mouse button?
		bool mouseRBClicked_;

		/// is the right mouse button down?
		bool mouseRBDown_;

		/// when placing map warps, we need to check to see if we have placed the first yet to place the second
		bool placedFirst_;

		/// a pointer to hold the previous map pointer when placing warps
		GameMap* previousMap_;

		/// holds the position of the camera before the map change when adding warps
		int previousCameraX_;
		int previousCameraY_;

		/// used internally for placing map warps
		struct WarpTarget
		{
			int worldX_, worldY_, targetMapID_;
			WarpTarget() :
				worldX_(0),
				worldY_(0),
				targetMapID_(0)
			{
			}

			WarpTarget(int worldX, int worldY, int targetMapID) :
				worldX_(worldX),
				worldY_(worldY),
				targetMapID_(targetMapID)
			{
			}

			void Copy(WarpTarget& source)
			{
				worldX_ 		= source.worldX_;
				worldY_ 		= source.worldY_;
				targetMapID_ 	= source.targetMapID_;
			}
		};

		WarpTarget warpFrom_;
		WarpTarget warpTo_;

	};

	/// simple define to make it easier to use the map editors singleton
	#define MapEditors GameMapEditorsSingleton::GetInstance()

} // end namespace
#endif


