
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

			/// we are editing the object layer of the map
			EditingObjectLayer,

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

		// shared resource pointers -- the game singleton class is responsible for init/destroy these pointers

		/// shared resource pointer to the display surface of the game system
		ImageResource* microDisplay_;

		/// a shared resource pointer to the game singleton's current map
		GameMap* currentMap_;

		// map editor resource pointers -- the map editor is responsible for init/destroy these pointers

		/// the camera used for previewing
		GameCamera* camera_;

		MAPEDITORS::EditorState state_;

		int mouseX_;
		int mouseY_;
		bool mouseClicked_;
	};

	/// simple define to make it easier to use the map editors singleton
	#define MapEditors GameMapEditorsSingleton::GetInstance()

} // end namespace
#endif


