
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

	/**************************************************************************/

		/// a page in the dialogue system is a list of strings
		class DialoguePage
		{
		public:
			DialoguePage();
			~DialoguePage();

			/// clears the page
			void Clear();

			/// adds a line to the page
			void AddLine(const char* lineContent);

			/// gets the specified @lineIndex or an empty string "" if out of range
			std::string GetLine(unsigned int lineIndex);

			/// gets the number of lines in the page
			unsigned int GetNumLines() const;

		private:
			DialoguePage(const DialoguePage& rhs);
			const DialoguePage& operator=(const DialoguePage& rhs);

		private:
			std::vector<std::string> lines_;
		};

	/**************************************************************************/

		/// maximum characters per line
		const unsigned int LINE_LENGTH       = 21;

		/// maximum lines per page
		const unsigned int LINES_PER_PAGE    = 6;

		/// a dialogue is a list of pages
		class Dialogue
		{
		public:
			Dialogue();
			~Dialogue();

			/// clears the dialogue
			void Clear();

			/// adds a line to the dialogue, and as needed adds new pages
			void AddLine(const char* lineContent);

			/// gets a pointer to the specified @pageIndex or zero if out of range
			DialoguePage* GetPage(unsigned int pageIndex);

			/// gets the number of pages in the dialogue
			unsigned int GetNumPages() const;

		private:
			Dialogue(const Dialogue& rhs);
			const Dialogue& operator=(const Dialogue& rhs);

		private:
			std::vector<DialoguePage*> pages_;
		};

	} // end namespace

	/**************************************************************************/

	/// the dialogue system class
	class GameDialogueMessage
	{
	public:
		GameDialogueMessage();

		~GameDialogueMessage();

		/// initializes the dialogue system with data
		void Initialize(const std::vector<std::string>& message, BitmapFont* smallFont, bool useDualFonts = false, BitmapFont* largeFont = 0);

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
		void MessageToDialogue(const std::vector<std::string>& message);

		/// clears the @dialogue_ structure of all lines and pages
		void ResetDialogue();

		/// advances the page counter
		void AdvancePage();

		/// advances the line counter
		void AdvanceLine();

		/// advances the character counter
		void AdvanceCharacter();

	private:

		/// the state of the dialogue system
		DIALOGUE::DialogueState currentState_;

		/// the dialogue data structure
		DIALOGUE::Dialogue dialogue_;

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


