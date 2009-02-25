
// CODESTYLE: v2.0

// GUITextEntryDialog.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for easily getting text input from the user with the Allegro GUI

/**
 * \file GUITextEntryDialog.cpp
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
#include "GUITextEntryDialog.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace GUI
	{
		GUITextEntryDialog::GUITextEntryDialog(const char* prompt, const char* windowCaption, const char* buttonCaption)
		{
			windowCaption_ 	= windowCaption;
			buttonCaption_	= buttonCaption;

			std::string promptStr = prompt;
			std::vector<std::string> lines = Tokenize(promptStr, "\n");
			unsigned int lineCount = lines.size();
			if (lineCount < 4)
			{
				for (unsigned int index = 0; index < lineCount; index++)
				{
					prompts_.push_back(lines.at(index).c_str());
				}
				while(prompts_.size() < 4) { prompts_.push_back(""); }
			}
			else
			{
				lineCount = (lineCount > 4) ? 4 : lineCount;
				for (unsigned int index = 0; index < lineCount; index++)
				{
					prompts_.push_back(lines.at(index).c_str());
				}
			}

			InitDialog();
		}

		/**************************************************************************/

		std::vector<std::string> GUITextEntryDialog::Tokenize(const std::string& source, const std::string& delimiters)
		{
			std::vector<std::string> tokens;

			size_t p0 = 0;
			size_t p1 = std::string::npos;

			while (p0 != std::string::npos)
			{
				p1 = source.find_first_of(delimiters, p0);
				if (p1 != p0)
				{
					std::string token = source.substr(p0, p1 - p0);
					tokens.push_back(token);
				}
				p0 = source.find_first_not_of(delimiters, p1);
			}

			return tokens;
		}

		/**************************************************************************/

		GUITextEntryDialog::~GUITextEntryDialog()
		{
			delete [] internalDialog_;
		}

		/**************************************************************************/

		void GUITextEntryDialog::InitDialog()
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

			// list of controls on the dialog:
			// d_box_proc			-- the entire dialog is within this box
			// d_box_proc			-- the window caption is surrounded by a box
			// d_ctext_proc			-- the window caption is centered horizontally

			// d_ctext_proc			-- the prompt text is centered horizontally
			// d_ctext_proc			-- the prompt text is centered horizontally
			// d_ctext_proc			-- the prompt text is centered horizontally
			// d_ctext_proc			-- the prompt text is centered horizontally

			// d_box_proc			-- the text edit field is surrounded by a box
			// d_edit_proc			-- the text edit field that text is entered by the user into
			// d_button_proc		-- the "OK" button (caption can be changed)
			// d_yield_proc 		-- used to let the CPU rest while the user is not actively using the dialog


			enum ControlNames
			{
				FormMain,
				FrameCaption,
				StaticCaption,
				StaticPrompt1,
				StaticPrompt2,
				StaticPrompt3,
				StaticPrompt4,
				FrameEditField,
				TextEditField,
				ButtonOKButton,
				HiddenCPUYield,
				EndDialog
			};

			int controlCount = 1 + EndDialog;
			internalDialog_ = new DIALOG [controlCount];

			// zero out all controls
			for (int index = 0; index < controlCount; index++)
			{
				memset(&internalDialog_[index], 0, sizeof(DIALOG));
			}

			// clear the internal string
			memset(reinterpret_cast<void*>(internalTextBuffer_), 0, sizeof(internalTextBuffer_));


			// calculate the size of the form
			/*
				form height = the button height + the text entry height + 4 static texts + caption

				form width = longest line of prompt length * width of the current font

				each control should have 4 pixel padding
			*/


			int charWidth = text_length(font, "A");
			int charHeight = text_height(font);
			int controlPadding = 4;
			int formWidth = 0;
			int formHeight = 0;

			unsigned int longestLine = 0;
			for (unsigned int index = 0; index < prompts_.size(); index++)
			{
				if (prompts_.at(index).size() > longestLine)
				{
					longestLine = prompts_.at(index).size();
				}
			}

			formWidth = controlPadding + (charWidth * longestLine) + controlPadding;
			formHeight = (controlCount * (2*controlPadding)) +  (4 * charHeight);


			// init the main form -- a d_box_proc control
			internalDialog_[FormMain].proc				= d_box_proc;
			internalDialog_[FormMain].x					= 0;
			internalDialog_[FormMain].y					= 0;
			internalDialog_[FormMain].w					= formWidth;
			internalDialog_[FormMain].h					= formHeight;

			// init the frame for the window's caption text -- a d_box_proc control
			internalDialog_[FrameCaption].proc			= d_box_proc;
			internalDialog_[FrameCaption].x				= 0;
			internalDialog_[FrameCaption].y				= 0;
			internalDialog_[FrameCaption].w				= internalDialog_[FormMain].w;
			internalDialog_[FrameCaption].h				= controlPadding + charHeight + controlPadding;

			// init the window's caption text -- a d_ctext_proc control
			/*
			These draw text onto the screen.
			The dp field should point to the string to display.
			d_ctext_proc() centers the string horizontally, and d_rtext_proc() right aligns it.
			Any '&' characters in the string will be replaced with lines underneath the following character,
			for displaying keyboard shortcuts (as in MS Windows). To display a single ampersand,
			put "&&". To draw the text in something other than the default font, set the dp2 field to point to your custom font data.
			*/
			internalDialog_[StaticCaption].proc			= d_ctext_proc;
			internalDialog_[StaticCaption].x			= internalDialog_[FrameCaption].x + controlPadding;
			internalDialog_[StaticCaption].y			= controlPadding;
			internalDialog_[StaticCaption].w			= internalDialog_[FrameCaption].w - controlPadding;
			internalDialog_[StaticCaption].dp			= const_cast<void*>(reinterpret_cast<const void*>(windowCaption_.c_str()));

			// init the prompt texts -- four d_ctext_proc controls
			internalDialog_[StaticPrompt1].proc			= d_ctext_proc;
			internalDialog_[StaticPrompt1].x			= internalDialog_[FrameCaption].x + controlPadding;
			internalDialog_[StaticPrompt1].y			= internalDialog_[FrameCaption].y + internalDialog_[FrameCaption].h + controlPadding;
			internalDialog_[StaticPrompt1].w			= internalDialog_[FrameCaption].w - controlPadding;
			internalDialog_[StaticPrompt1].h			= controlPadding + charHeight + controlPadding;
			internalDialog_[StaticPrompt1].dp			= const_cast<void*>(reinterpret_cast<const void*>(prompts_.at(0).c_str()));

			internalDialog_[StaticPrompt2].proc			= d_ctext_proc;
			internalDialog_[StaticPrompt2].x			= internalDialog_[FrameCaption].x + controlPadding;
			internalDialog_[StaticPrompt2].y			= internalDialog_[StaticPrompt1].y + internalDialog_[StaticPrompt1].h;
			internalDialog_[StaticPrompt2].w			= internalDialog_[FrameCaption].w - controlPadding;
			internalDialog_[StaticPrompt2].h			= controlPadding + charHeight + controlPadding;
			internalDialog_[StaticPrompt2].dp			= const_cast<void*>(reinterpret_cast<const void*>(prompts_.at(1).c_str()));

			internalDialog_[StaticPrompt3].proc			= d_ctext_proc;
			internalDialog_[StaticPrompt3].x			= internalDialog_[FrameCaption].x + controlPadding;
			internalDialog_[StaticPrompt3].y			= internalDialog_[StaticPrompt2].y + internalDialog_[StaticPrompt2].h;
			internalDialog_[StaticPrompt3].w			= internalDialog_[FrameCaption].w - controlPadding;
			internalDialog_[StaticPrompt3].h			= controlPadding + charHeight + controlPadding;
			internalDialog_[StaticPrompt3].dp			= const_cast<void*>(reinterpret_cast<const void*>(prompts_.at(2).c_str()));

			internalDialog_[StaticPrompt4].proc			= d_ctext_proc;
			internalDialog_[StaticPrompt4].x			= internalDialog_[FrameCaption].x + controlPadding;
			internalDialog_[StaticPrompt4].y			= internalDialog_[StaticPrompt3].y + internalDialog_[StaticPrompt3].h;
			internalDialog_[StaticPrompt4].w			= internalDialog_[FrameCaption].w - controlPadding;
			internalDialog_[StaticPrompt4].h			= controlPadding + charHeight + controlPadding;
			internalDialog_[StaticPrompt4].dp			= const_cast<void*>(reinterpret_cast<const void*>(prompts_.at(3).c_str()));


			// init the frame that surrounds the edit field -- a d_box_proc control
			internalDialog_[FrameEditField].proc			= d_box_proc;
			internalDialog_[FrameEditField].x				= 0;
			internalDialog_[FrameEditField].y				= (internalDialog_[StaticPrompt4].y + internalDialog_[StaticPrompt4].h) - 1;
			internalDialog_[FrameEditField].w				= internalDialog_[FormMain].w;
			internalDialog_[FrameEditField].h				= charHeight + controlPadding;

			// init the text edit field -- a d_edit_proc control
			/*
			An editable text object (the dp field points to the string).
			When it has the input focus (obtained by clicking on it with the mouse),
			text can be typed into this object. The d1 field specifies the maximum number of
			characters that it will accept, and d2 is the text cursor position within the string.

			Note: dp must point to a buffer at least (d1 + 1) * 4 bytes long because,
			depending on the encoding format in use, a single character can occupy up to 4 bytes
			and room must be reserved for the terminating null character.
			*/

			internalDialog_[TextEditField].proc			= d_edit_proc;
			internalDialog_[TextEditField].x			= controlPadding;
			internalDialog_[TextEditField].y			= internalDialog_[StaticPrompt4].y + internalDialog_[StaticPrompt4].h;
			internalDialog_[TextEditField].w			= internalDialog_[FormMain].w - (2 * controlPadding);
			internalDialog_[TextEditField].h			= charHeight;
			internalDialog_[TextEditField].d1			= 0x400;
			internalDialog_[TextEditField].dp			= internalTextBuffer_;

			// init the OK button -- a d_button_proc control
			internalDialog_[ButtonOKButton].proc		= d_button_proc;
			internalDialog_[ButtonOKButton].x			= (internalDialog_[FormMain].w / 2) - ((text_length(font, buttonCaption_.c_str()) + (controlPadding*12)) / 2);;
			internalDialog_[ButtonOKButton].w			= text_length(font, buttonCaption_.c_str()) + (controlPadding * 12);
			internalDialog_[ButtonOKButton].h			= controlPadding + charHeight + controlPadding;
			internalDialog_[ButtonOKButton].y			= controlPadding + controlPadding + internalDialog_[FrameEditField].y + internalDialog_[FrameEditField].h;
			internalDialog_[ButtonOKButton].flags		= D_EXIT;
			internalDialog_[ButtonOKButton].dp			= const_cast<void*>(reinterpret_cast<const void*>(buttonCaption_.c_str()));

			// init the CPU yield -- a d_yield_proc control
			internalDialog_[HiddenCPUYield].proc		= d_yield_proc;


			// set the color of the dialog
			const int controlColorFG = makecol(255, 255, 255);
			const int controlColorBG = makecol(0, 0, 0);
			set_dialog_color(internalDialog_, controlColorFG, controlColorBG);

			// re- color the window caption
			internalDialog_[StaticCaption].bg			= -1;
			internalDialog_[FrameCaption].bg			= makecol(0, 0, 64);
			internalDialog_[FrameCaption].fg			= makecol(255, 255, 255);


			// center the dialog
			centre_dialog(internalDialog_);
		}

		/**************************************************************************/

		void GUITextEntryDialog::Show()
		{
			popup_dialog(internalDialog_, 4);
		}

		/**************************************************************************/

		std::string GUITextEntryDialog::GetText()
		{
			return static_cast<std::string>(internalTextBuffer_);
		}

	} // end namespace

} // end namespace
