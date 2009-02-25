
// CODESTYLE: v2.0

// GUITextEntryDialog.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for easily getting text input from the user with the Allegro GUI

/**
 * \file GUITextEntryDialog.h
 * \brief Allegro GUI - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __GUITEXTENTRYDIALOG_H__
#define __GUITEXTENTRYDIALOG_H__

#include <vector>
#include <string>

struct DIALOG;

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains Allegro GUI Utility Classes
	namespace GUI
	{
		/// A class for easily getting text input from the user with the Allegro GUI
		class GUITextEntryDialog
		{
		public:
			/// default constructor
			GUITextEntryDialog(const char* prompt = "Enter your text below.",
				const char* windowCaption = "Enter Text - ged101 GUI",
				const char* buttonCaption = "OK");

			/// default destructor
			~GUITextEntryDialog();

			/// displays the dialog
			void Show();

			/// returns the value entered into the text edit field control
			std::string GetText();

		private:
			/// hidden copy constructor
			GUITextEntryDialog(const GUITextEntryDialog& rhs);

			/// hidden assignment operator
			const GUITextEntryDialog& operator=(const GUITextEntryDialog& rhs);

		private:

			/// tokenizes a string -- this really needs to be stuck in a utility class since we use it so much!
			std::vector<std::string> Tokenize(const std::string& source, const std::string& delimiters);

			/// initializaes the allegro dialog
			void InitDialog();

			/// the allegro dialog
			DIALOG* internalDialog_;

			/// a static text buffer to hold entered text
			char internalTextBuffer_[0x401]; // 1024 characters should be more than enough! (+ 1 for allegro d_edit_proc)

			/// the caption of the window
			std::string windowCaption_;

			/// the caption on the "OK" button
			std::string buttonCaption_;

			/// the lines of text in the dialog
			std::vector<std::string> prompts_;
		}; // end class

	} // end namespace

} // end namespace

#endif

