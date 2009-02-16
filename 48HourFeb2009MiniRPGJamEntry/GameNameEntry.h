
// CODESTYLE: v2.0

// GameTilesetManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: manages a registry of named tilesets

#ifndef __GAMENAMEENTRY_H__
#define __GAMENAMEENTRY_H__

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

	class GameNameEntry
	{
	public:
		/**
		 * Creates a new name entry object. Fixed 200x150 screen.
		 * @param defaultName defaultName
		 * @param maxChars Specify the maximum number of characters a name can hold.
		 */
		GameNameEntry(const std::string& defaultName,int maxChars);
		
		// destructor
		~GameNameEntry();
		
		/**
		 * Call Update() in the main loop.
		 * @return Returns 1 when user accepts name entry, -1 if user cancels, 0 otherwise
		 */
		int Update();
		
		/**
		 * Renders the name entry screen to a buffer.
		 * @param target Destination image.
		 */
		void Render(ImageResource *target);
		
		/// Changes the name explicitly
		void SetName(const std::string& name);
		
		/// Returns the name entered.
		std::string GetName() const;
	private:
		GameNameEntry(const GameNameEntry&);
		GameNameEntry& operator =(const GameNameEntry&);

		int maxChars_;
		std::string name_;
		int cursor_;
		BitmapFont *font_;
		ImageResource *overlay_;
		ImageResource *cursorImage_;
		
		static char keyMap_[];
	};
}

#endif
