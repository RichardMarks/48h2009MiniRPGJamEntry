
// CODESTYLE: v2.0

// GameObject.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: handles game dialogs

#include "GameLibrary.h"

namespace GAME
{
	GameDialogueMessage::GameDialogueMessage() :
		currentState_(DIALOGUE::Undefined),
		smallFont_(0),
		largeFont_(0),
		useDualFonts_(false),
		useDelay_(false),
		counter_(0),
		delay_(0),
		currentPage_(0),
		currentLine_(0),
		currentChar_(0),
		pageCount_(0),
		lineCount_(0),
		charCount_(0)
	{
	}

	/**************************************************************************/

	GameDialogueMessage::~GameDialogueMessage()
	{
		// clear out the dialogue data structure
		ResetDialogue();
	}

	/**************************************************************************/

	void GameDialogueMessage::Initialize(const char* message, BitmapFont* smallFont, bool useDualFonts, BitmapFont* largeFont)
	{
		// initalize the dialogue data structure
		MessageToDialogue(message);

		// set the fonts to use
		smallFont_ 		= smallFont;
		largeFont_ 		= largeFont;

		// set if we are using dual fonts
		useDualFonts_ 	= useDualFonts;
	}

	/**************************************************************************/

	void GameDialogueMessage::Update(bool useDelay_)
	{
		// handle the dialogue system states
		switch(currentState_)
		{
			// starting printing
			case DIALOGUE::Ready: {} break;

			case DIALOGUE::Idle:
			{
				if (InputDevice->KeyPressed(KEY::Key_Space))
				{
					currentState_ = DIALOGUE::Busy;
					AdvancePage();
				}
			} break;

			case DIALOGUE::Busy:
			{
			} break;

			case DIALOGUE::Finished: {} break;

			default: break;
		}

		if (useDelay_)
		{
			// we are using delayed output
			// so we advance a single character at a time
		}
		else
		{
			// show the entire page at once
		}
	}

	/**************************************************************************/

	void GameDialogueMessage::Render(ImageResource* target)
	{
		// handle the dialogue system states
		switch(currentState_)
		{
			// starting printing
			case DIALOGUE::Ready: {} break;

			case DIALOGUE::Idle:
			{
			} break;

			case DIALOGUE::Busy:
			{
				if (useDualFonts_)
				{
					// we are using dual fonts to create a large first character
					// for the first line of each page.
				}
				else
				{
					// we are using only the small font
				}
			} break;

			case DIALOGUE::Finished:
			{
			} break;

			default: break;
		}

	}

	/**************************************************************************/

	DIALOGUE::DialogueState GameDialogueMessage::GetState() const
	{
		return currentState_;
	}

	/**************************************************************************/

	void GameDialogueMessage::MessageToDialogue(const char* message)
	{
		// TODO: Implement the message ripper code

		// cut message into lines (taking into account any '\n' characters)
		std::string fullText = message;
		std::vector<std::string> fullTextLines = GameSingleton::GetInstance()->Tokenize(fullText, "\n");
#if 1
		unsigned int lineCount = fullTextLines.size();
		unsigned int index = 0;
		fprintf(stderr, "There are %d new-lines in the fullText:\n", lineCount);

		for (index = 0; index < lineCount; index++)
		{
			fprintf(stderr, "\tLine %04d: \"%s\"\n", index + 1, fullTextLines.at(index).c_str());
		}

		// cut each line in fullTextLines into 21-character lines
		// making sure that we don't break any words
#endif
		// create pages from the lines

		// init the dialogue data counter variables
	}

	/**************************************************************************/

	void GameDialogueMessage::ResetDialogue()
	{
		// for each page, clear out the lines vector proper
		for (unsigned int index = 0; index < pageCount_; index++)
		{
			dialogue_.pages_.at(index)->lines_.clear();
		}

		// clear the pages vector proper
		dialogue_.pages_.clear();

		// reset the dialogue data counter variables
		pageCount_ 		= 0;
		lineCount_ 		= 0;
		charCount_ 		= 0;
	}

	/**************************************************************************/

	void GameDialogueMessage::GameDialogueMessage::AdvancePage()
	{
		if (currentPage_ < pageCount_)
		{
			currentPage_++;

			// update the new page info
			currentLine_ 	= 0;
			lineCount_ 		= dialogue_.pages_.at(currentPage_)->lines_.size();

		}
		else
		{
			// we are at the end of the dialogue, change state
			currentState_ = DIALOGUE::Finished;
		}
	}

	/**************************************************************************/

	void GameDialogueMessage::AdvanceLine()
	{
		if (currentLine_ < lineCount_)
		{
			currentLine_++;

			// update the new line info
			currentChar_ 	= 0;
			charCount_ 		= dialogue_.pages_.at(currentPage_)->lines_.at(currentLine_).size();
		}
		else
		{
			// we are at the end of the page, we go idle
			currentState_ = DIALOGUE::Idle;

			// advance page if there are more pages
			if (currentPage_ < pageCount_)
			{
				AdvancePage();
			}
			else
			{
				// no more pages, set Finished state
				currentState_ = DIALOGUE::Finished;
			}
		}
	}

	/**************************************************************************/

	void GameDialogueMessage::AdvanceCharacter()
	{
		if (currentChar_ < charCount_)
		{
			currentChar_++;
		}
		else
		{
			// advance line if there are more lines
			if (currentLine_ < lineCount_)
			{
				AdvanceLine();
			}
		}
	}
} // end namespace


