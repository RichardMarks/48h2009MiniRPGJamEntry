
// CODESTYLE: v2.0

// GUIListBox.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for easily making a selection from a list of strings with the Allegro GUI

/**
 * \file GUIListBox.h
 * \brief Allegro GUI - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __GUILISTBOX_H__
#define __GUILISTBOX_H__

#include <vector>
#include <string>

struct DIALOG;

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains Allegro GUI Utility Classes
	namespace GUI
	{
		class GUIListBox
		{
		public:
			/// constructor
			GUIListBox();

			/// alternate constructor
			GUIListBox(std::vector<std::string>& items);

			/// destructor
			~GUIListBox();

			/// displays the listbox window
			void Show();

			/// clears the listbox of all items
			void Clear();

			/// sets the listbox items
			void Set(std::vector<std::string>& items);

			/// adds an item to the listbox
			void Add(const char* item);

			/// gets the selected item
			std::string GetSelection();

		private:
			/// hidden copy constructor
			GUIListBox(const GUIListBox& rhs);

			/// hidden assignment operator
			const GUIListBox& operator=(const GUIListBox& rhs);

		private:

			/// inits the allegro @dialog_ structure
			void InitDialog();

			/// the callback function for the list box
			static char* ListBoxHandler(int index, int* itemCount);

			/// the real listbox contents
			static std::vector<std::string> itemVector_;

			/// the allegro dialog structure
			DIALOG* internalDialog_;

			/// is the dialog initialized?
			bool init_;

		}; // end class

	} // end namespace

} // end namespace
#endif

