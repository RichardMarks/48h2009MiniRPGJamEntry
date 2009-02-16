
// CODESTYLE: v2.0

// GameDialogueMessage.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: handles game dialogs

#ifndef __GAMEDIALOGUEMESSAGE_H__
#define __GAMEDIALOGUEMESSAGE_H__

#include <vector>
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

	/// constants and enumerations for the dialogue system
	namespace DIALOGUE
	{
		/// maximum characters per line
		const int LINE_LENGTH 		= 21;

		/// maximum lines per page
		const int LINES_PER_PAGE 	= 6;

		/// the possible states the dialogue system can be in
		enum DialogueState
		{
			/// the dialogue system is undefined, there is no message to print, you must call Initialize()
			Undefined,

			/// the dialogue system is ready, the data is init, and will begin printing
			Ready,

			/// the dialogue system is idle, the message has been printed, you need to press spacebar to advance the system
			Idle,

			/// the dialogue system is busy, the message is being printed, you can press spacebar to advance printing to the end of the current line
			Busy,

			/// the dialogue system is finished, there are no more pages to print, press spacebar to end the dialogue
			Finished
		};
	} // end namespace

	/// the dialogue system class
	class GameDialogueMessage
	{
	public:
		GameDialogueMessage();

		~GameDialogueMessage();

		/// initializes the dialogue system with data
		void Initialize(const char* message, BitmapFont* smallFont, bool useDualFonts = false, BitmapFont* largeFont = 0);

		/// updates the dialogue
		void Update(bool useDelay_ = true);

		/// renders the dialogue
		void Render(ImageResource* target);

		/// gets the state of the dialogue system
		DIALOGUE::DialogueState GetState() const;

	private:
		GameDialogueMessage(const GameDialogueMessage& rhs);
		const GameDialogueMessage& operator=(const GameDialogueMessage& rhs);

		/// fills the @dialogue_ structure with pages & lines
		void MessageToDialogue(const char* message);

		/// clears the @dialogue_ structure of all lines and pages
		void ResetDialogue();

		/// advances the page counter
		void AdvancePage();

		/// advances the line counter
		void AdvanceLine();

		/// advances the character counter
		void AdvanceCharacter();

	private:

		/// a page in the dialogue system is a list of strings
		struct DialoguePage
		{
			std::vector<std::string> lines_;
		};

		/// a dialogue is a list of pages
		struct Dialogue
		{
			std::vector<DialoguePage> pages_;

		};

		/// the state of the dialogue system
		DIALOGUE::DialogueState currentState_;

		/// the dialogue data structure
		Dialogue dialogue_;

		/// the fonts to use in rendering the dialogue; the dialogue system does NOT delete these pointers
		BitmapFont* smallFont_;
		BitmapFont* largeFont_;

		/// are we using the dual-font display for the first line of a page?
		bool useDualFonts_;

		/// are we using delayed output or writing all text on a page at once?
		bool useDelay_;

		/// delayed output counter
		int counter_;

		/// delayed output delay rate (higher numbers means it takes longer to output each character)
		int delay_;

		/// what page of the dialogue are we on?
		unsigned int currentPage_;

		/// what line of the current page are we on?
		unsigned int currentLine_;

		/// what character of the current line are we on?
		unsigned int currentChar_;

		/// number of pages in the dialogue
		unsigned int pageCount_;

		/// number of lines in the current page
		unsigned int lineCount_;

		/// number of characters in the current line
		unsigned int charCount_;
	};

} // end namespace
#endif


