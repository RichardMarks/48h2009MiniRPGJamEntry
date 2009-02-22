
// CODESTYLE: v2.0

// GUIListBox.h
// Project: Game Engine Design 101 (ENGINE)
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

namespace ENGINE
{
	class GUIListBox
	{
	public:
		GUIListBox();

		GUIListBox(std::vector<std::string>& items);

		~GUIListBox();

		void Show();

		void Clear();

		void Set(std::vector<std::string>& items);

		void Add(const char* item);

		std::string GetSelection();

	private:
		GUIListBox(const GUIListBox& rhs);
		const GUIListBox& operator=(const GUIListBox& rhs);

	private:

		void InitDialog();

		static char* ListBoxHandler(int index, int* itemCount);

		static std::vector<std::string> itemVector_;

		DIALOG* internalDialog_;

		bool init_;
	};

} // end namespace
#endif

