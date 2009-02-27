
// CODESTYLE: v2.0

// GUIListBox.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for easily making a selection from a list of strings with the Allegro GUI

/**
 * \file GUIListBox.cpp
 * \brief Allegro GUI - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

// include Allegro
#include <allegro.h>

// include the complementing header
#include "GUIListBox.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace GUI
	{
		std::vector<std::string> GUIListBox::itemVector_;

		/**************************************************************************/

		char* GUIListBox::ListBoxHandler(int index, int* itemCount)
		{
			if (index < 0)
			{
				*itemCount = static_cast<int>(GUIListBox::itemVector_.size());
				return 0;
			}
			else
			{
				return const_cast<char*>(GUIListBox::itemVector_.at(index).c_str());
			}
		}

		/**************************************************************************/

		GUIListBox::GUIListBox() :
			init_(false)
		{
			InitDialog();
		}

		/**************************************************************************/

		GUIListBox::GUIListBox(std::vector<std::string>& items)
		{
			Set(items);
			InitDialog();
		}

		/**************************************************************************/

		GUIListBox::~GUIListBox()
		{
			GUIListBox::itemVector_.clear();
			init_ = false;
			delete [] internalDialog_;
		}

		/**************************************************************************/

		void GUIListBox::InitDialog()
		{
			/*
				int (*proc)(int, DIALOG *, int); - dialog procedure (message handler)
				int x, y, w, h;       - position and size of the object
				int fg, bg;           - foreground and background colors
				int key;              - ASCII keyboard shortcut
				int flags;            - flags about the status of the object
				int d1, d2;           - whatever you want to use them for
				void *dp, *dp2, *dp3; - pointers to more object-specific data
			*/

			internalDialog_ = new DIALOG [3];

			// zero out all controls so that we only need to init the members that will not be zero
			for (unsigned int index = 0; index < 3; index++)
			{
				memset(&internalDialog_[index], 0, sizeof(DIALOG));
			}


			// the only control in the dialog is the list box
			internalDialog_[0].proc 	= d_list_proc;
			internalDialog_[0].dp 		= reinterpret_cast<void*>(GUIListBox::ListBoxHandler);
			internalDialog_[0].w		= 320;
			internalDialog_[0].h 		= 240;
			internalDialog_[0].fg		= makecol(255, 255, 255);
			internalDialog_[0].bg		= makecol(0, 0, 0);
			internalDialog_[0].flags	= D_EXIT;

			internalDialog_[1].proc 	= d_yield_proc;

			centre_dialog(internalDialog_);
		}

		/**************************************************************************/

		void GUIListBox::Show()
		{
			if (init_)
			{
				popup_dialog(internalDialog_, 0);
			}
		}

		/**************************************************************************/

		void GUIListBox::Clear()
		{
			GUIListBox::itemVector_.clear();
			init_ = false;
		}

		/**************************************************************************/

		void GUIListBox::Set(std::vector<std::string>& items)
		{
			GUIListBox::itemVector_.swap(items);
		}

		/**************************************************************************/

		void GUIListBox::Add(const char* item)
		{
			GUIListBox::itemVector_.push_back(item);
			init_ = true;
		}

		/**************************************************************************/

		std::string GUIListBox::GetSelection()
		{
			return GUIListBox::itemVector_.at(internalDialog_[0].d1);
		}

	} // end namespace
} // end namespace


