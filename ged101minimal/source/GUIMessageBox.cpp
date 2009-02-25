
// CODESTYLE: v2.0

// GUIMessageBox.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for displaying multiple lines of text in a message box to the user with the Allegro GUI

/**
 * \file GUIMessageBox.cpp
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
#include "GUIMessageBox.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace GUI
	{
		GUIMessageBox::GUIMessageBox() :
			buttonIndex_(0),
			dialog_(0),
			init_(false)
		{
		}

		/**********************************************************************/

		GUIMessageBox::GUIMessageBox(const char* message, const char* windowCaption, const char* buttonCaption) :
			buttonIndex_(0),
			dialog_(0),
			init_(false)
		{
			SplitMessage(message);
			windowCaption_ = windowCaption;
			buttonCaption_ = buttonCaption;
		}

		/**********************************************************************/

		GUIMessageBox::~GUIMessageBox()
		{
			// de-allocate the dialog if it has been allocated
			if (0 != dialog_) { delete [] dialog_; }

			// clear out the message vector
			message_.clear();
		}

		/**********************************************************************/

		void GUIMessageBox::InitDialog(bool centered, int left, int top)
		{
			// if there is no message, we exit
			if (!message_.size())
			{
				init_ = false;
				return;
			}

			// de-allocate the dialog if it has been allocated
			if (0 != dialog_) { delete [] dialog_; }

			// the control IDs
			const unsigned int FormMain 		= 0;
			const unsigned int FrameCaption 	= 1;
			const unsigned int StaticCaption 	= 2;
			const int StaticTextFirstLine 		= StaticCaption + 1;
			unsigned int ButtonOKButton 		= StaticTextFirstLine + message_.size(); buttonIndex_ = ButtonOKButton;
			unsigned int HiddenCPUYield 		= ButtonOKButton + 1;
			unsigned int EndDialog 				= HiddenCPUYield + 1;

			// allocate the dynamic dialog array
			unsigned int controlCount = 1 + EndDialog;
			dialog_ = new DIALOG [controlCount];

			// zero out all controls so that we only need to init the members that will not be zero
			for (unsigned int index = 0; index < controlCount; index++)
			{
				memset(&dialog_[index], 0, sizeof(DIALOG));
			}

			// get the size of the font
			int charWidth 				= text_length(font, "A");
			int charHeight 				= text_height(font);

			// the amount of padding around each control
			int controlPadding 			= 4;

			// the number of lines we will have
			unsigned int numberOfLines 	= message_.size();

			// find the longest line of text
			unsigned int longestLine = 0;
			for (unsigned int index = 0; index < numberOfLines; index++)
			{
				if (message_.at(index).size() > longestLine)
				{
					longestLine = message_.at(index).size();
				}
			}

			// calculate the size of the form
			int formWidth	= controlPadding + (charWidth * longestLine) + controlPadding;
			int formHeight	= (controlCount * (2 * controlPadding)) +  (numberOfLines * charHeight);

			// init the form controls except the message lines

			dialog_[FormMain].proc				= d_box_proc;

			dialog_[FormMain].w					= formWidth;
			dialog_[FormMain].h					= formHeight;
			dialog_[FrameCaption].proc			= d_box_proc;

			dialog_[FrameCaption].w				= dialog_[FormMain].w;
			dialog_[FrameCaption].h				= controlPadding + charHeight + controlPadding;
			dialog_[StaticCaption].proc			= d_ctext_proc;
			dialog_[StaticCaption].x			= dialog_[FrameCaption].x + controlPadding;
			dialog_[StaticCaption].y			= controlPadding;
			dialog_[StaticCaption].w			= dialog_[FrameCaption].w - controlPadding;
			dialog_[StaticCaption].h			= charHeight;
			dialog_[StaticCaption].dp			= const_cast<void*>(reinterpret_cast<const void*>(windowCaption_.c_str()));

			// init the message lines
			int msgy = dialog_[FrameCaption].y + dialog_[FrameCaption].h + controlPadding;

			for (unsigned int index = StaticTextFirstLine; index < ButtonOKButton; index++)
			{
				dialog_[index].proc 		= d_ctext_proc;
				dialog_[index].x			= dialog_[FrameCaption].x + controlPadding;
				dialog_[index].y			= msgy + ((index - StaticTextFirstLine) * dialog_[index - 1].h) + (2*((index - StaticTextFirstLine) * controlPadding));
				dialog_[index].w			= dialog_[FrameCaption].w - controlPadding;
				dialog_[index].h			= charHeight;
				dialog_[index].dp			= const_cast<void*>(reinterpret_cast<const void*>(message_.at(index - StaticTextFirstLine).c_str()));
			}


			dialog_[ButtonOKButton].proc		= d_button_proc;
			dialog_[ButtonOKButton].x			= (dialog_[FormMain].w / 2) - ((text_length(font, buttonCaption_.c_str()) + (controlPadding*12)) / 2);
			dialog_[ButtonOKButton].w			= text_length(font, buttonCaption_.c_str()) + (controlPadding * 12);
			dialog_[ButtonOKButton].h			= controlPadding + charHeight + controlPadding;
			dialog_[ButtonOKButton].y			= controlPadding + controlPadding + dialog_[StaticTextFirstLine + numberOfLines-1].y + dialog_[StaticTextFirstLine + numberOfLines-1].h;
			dialog_[ButtonOKButton].flags		= D_EXIT;
			dialog_[ButtonOKButton].dp			= const_cast<void*>(reinterpret_cast<const void*>(buttonCaption_.c_str()));

			dialog_[HiddenCPUYield].proc 		= d_yield_proc;

			// set the color of the dialog
			const int controlColorFG = makecol(255, 255, 255);
			const int controlColorBG = makecol(0, 0, 0);
			set_dialog_color(dialog_, controlColorFG, controlColorBG);

			// re- color the window caption
			dialog_[StaticCaption].bg			= -1;
			dialog_[FrameCaption].bg			= makecol(0, 0, 64);
			dialog_[FrameCaption].fg			= makecol(255, 255, 255);

			// are we centering it?
			if (centered)
			{
				centre_dialog(dialog_);
			}
			else
			{
				// position it somewhere
				position_dialog(dialog_, left, top);
			}

			init_ = true;
		}

		/**********************************************************************/

		void GUIMessageBox::Show(bool centered, int left, int top)
		{
			// do we need to recreate the dialog ?
			if (!init_)
			{
				InitDialog(centered, left, top);
			}

			// show it
			popup_dialog(dialog_, buttonIndex_);
		}

		/**********************************************************************/

		void GUIMessageBox::SetMessage(const char* message)
		{
			SplitMessage(message);
			init_ = false;
		}

		/**********************************************************************/

		void GUIMessageBox::SetWindowCaption(const char* windowCaption)
		{
			windowCaption_ = windowCaption;
			init_ = false;
		}

		/**********************************************************************/

		void GUIMessageBox::SetButtonCaption(const char* buttonCaption)
		{
			buttonCaption_ = buttonCaption;
			init_ = false;
		}

		/**********************************************************************/

		void GUIMessageBox::SplitMessage(const char* message)
		{
			std::string source = message;
			message_.clear();

			size_t p0 = 0;
			size_t p1 = std::string::npos;

			while (p0 != std::string::npos)
			{
				p1 = source.find_first_of("\n", p0);
				if (p1 != p0)
				{
					std::string line = source.substr(p0, p1 - p0);
					message_.push_back(line);
				}
				p0 = source.find_first_not_of("\n", p1);
			}
		}

	} // end namespace
} // end namespace
