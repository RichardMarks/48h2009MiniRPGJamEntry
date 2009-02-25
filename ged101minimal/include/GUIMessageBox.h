
// CODESTYLE: v2.0

// GUIMessageBox.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for displaying multiple lines of text in a message box to the user with the Allegro GUI

/**
 * \file GUIMessageBox.h
 * \brief Allegro GUI - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __GUIMESSAGEBOX_H__
#define __GUIMESSAGEBOX_H__

#include <vector>
#include <string>

struct DIALOG;

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains Allegro GUI Utility Classes
	namespace GUI
	{
		/// A class for displaying multiple lines of text in a message box to the user with the Allegro GUI
		class GUIMessageBox
		{
		public:
			/// constructor
			GUIMessageBox();

			/// alternate constructor
			GUIMessageBox(const char* message, const char* windowCaption = "Message - ged101 GUI", const char* buttonCaption = "OK");

			/// destructor
			~GUIMessageBox();

			/// displays the message box
			void Show(bool centered = true, int left = 0, int top = 0);

			/// sets the message text
			void SetMessage(const char* message);

			/// sets the caption text of the message box window
			void SetWindowCaption(const char* windowCaption = "Message - ged101 GUI");

			/// sets the caption text of the OK button on the message box
			void SetButtonCaption(const char* buttoncaption = "OK");

		private:
			/// hidden copy constructor
			GUIMessageBox(const GUIMessageBox& rhs);

			/// hidden assignment operator
			const GUIMessageBox& operator=(const GUIMessageBox& rhs);

		private:
			/// splits the @message into lines by the newline character and fills the @messsage_ vector
			void SplitMessage(const char* message);

			/// initializes the allegro @dialog_ structure
			void InitDialog(bool centered, int left, int top);

			/// each line of the message box is held within this vector
			std::vector<std::string> message_;

			/// the caption text of the message box window
			std::string windowCaption_;

			/// the caption text of the message box OK button
			std::string buttonCaption_;

			/// the message box button index -- this lets us focus the button on display
			unsigned int buttonIndex_;

			/// the allegro dialog structure that holds all the gui info
			DIALOG* dialog_;

			/// is the dialog initialized? we recreate the dialog if we make a change to it.
			bool init_;
		}; // end class

	} // end namespace

} // end namespace

#endif

