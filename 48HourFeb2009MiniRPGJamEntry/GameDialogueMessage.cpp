
// CODESTYLE: v2.0

// GameObject.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: handles game dialogs

#include "GameLibrary.h"

namespace GAME
{
	namespace DIALOGUE
	{
		DialoguePage::DialoguePage()
		{
		}

		/**************************************************************************/

		DialoguePage::~DialoguePage()
		{
			Clear();
		}

		/**************************************************************************/

		void DialoguePage::Clear()
		{
			lines_.clear();
		}

		/**************************************************************************/

		void DialoguePage::AddLine(const char* lineContent)
		{
			lines_.push_back(lineContent);
		}

		/**************************************************************************/

		std::string DialoguePage::GetLine(unsigned int lineIndex)
		{
			return (lineIndex < lines_.size()) ? lines_.at(lineIndex) : "";
		}

		/**************************************************************************/

		unsigned int DialoguePage::GetNumLines() const
		{
			return lines_.size();
		}

		/**************************************************************************/

		/**************************************************************************/
		Dialogue::Dialogue()
		{
		}

		/**************************************************************************/

		Dialogue::~Dialogue()
		{
			Clear();
		}

		/**************************************************************************/

		void Dialogue::Clear()
		{
			unsigned int pageCount = pages_.size();

			for (unsigned int index = 0; index < pageCount; index++)
			{
				if (0 != pages_.at(index))
				{
					delete pages_[index];
					pages_[index] = 0;
				}
			}

			pages_.clear();
		}

		/**************************************************************************/

		void Dialogue::AddLine(const char* lineContent)
		{
			// if there are no pages in the dialogue, add the first page
			if (!pages_.size())
			{
				pages_.push_back(new DialoguePage());
			}
			else
			{
				// if the current (last) page is full, then we must add another page
				if (pages_.at(pages_.size() - 1)->GetNumLines() == LINES_PER_PAGE)
				{
					pages_.push_back(new DialoguePage());
				}
			}

			// add the line to the current (last) page in the dialogue
			pages_.at(pages_.size() - 1)->AddLine(lineContent);
		}

		/**************************************************************************/

		DialoguePage* Dialogue::GetPage(unsigned int pageIndex)
		{
			return (pageIndex < pages_.size()) ? pages_.at(pageIndex) : 0;
		}

		/**************************************************************************/

		unsigned int Dialogue::GetNumPages() const
		{
			return pages_.size();
		}
	} // end namespace

	/**************************************************************************/

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

	void GameDialogueMessage::Initialize(const std::vector<std::string>& message, BitmapFont* smallFont, bool useDualFonts, BitmapFont* largeFont)
	{
		// initalize the dialogue data structure
		ResetDialogue();
		MessageToDialogue(message);

		// set the fonts to use
		smallFont_ 		= smallFont;
		largeFont_ 		= largeFont;

		// set if we are using dual fonts
		useDualFonts_ 	= useDualFonts;

		//LogSimpleMessage("Initializing Dialogue... %d pages", dialogue_.GetNumPages());

		currentState_ = DIALOGUE::Ready;

		waitingToAdvance_ = false;
	}

	/**************************************************************************/

	void GameDialogueMessage::Update(bool useDelay)
	{
		useDelay_ = useDelay;

		// handle the dialogue system states
		switch(currentState_)
		{
			// starting printing
			case DIALOGUE::Ready:
			{
				//LogSimpleMessage("Dialogue is Ready...");
				currentState_ = DIALOGUE::Busy;
			} break;

			case DIALOGUE::Idle:
			{

				if (InputDevice->KeyPressed(KEY::Key_Enter))
				{
					AdvancePage();
				}
			} break;

			case DIALOGUE::Busy:
			{
				if (waitingToAdvance_)
				{
					AdvanceLine();
					waitingToAdvance_ = false;
				}

			} break;

			case DIALOGUE::Finished:
			{
				//LogSimpleMessage("Dialogue is Finished...");
				ResetDialogue();
				currentState_ = DIALOGUE::Undefined;
			} break;

			default: break;
		}


	}

	/**************************************************************************/

	void GameDialogueMessage::Render(ImageResource* target)
	{
		// handle the dialogue system states
		switch(currentState_)
		{
			case DIALOGUE::Idle:
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
					for (unsigned int index = 0; index < currentLine_; index++)
					{
						smallFont_->Print(target, 5, 94 + (index * (1 + smallFont_->GetLetterHeight())), "%s", dialogue_.GetPage(currentPage_)->GetLine(index).c_str());
					}
					waitingToAdvance_ = true;
				}
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

	void GameDialogueMessage::MessageToDialogue(const std::vector<std::string>& message)
	{
		dialogue_.Clear();
		unsigned int messageLineCount = message.size();
		for (unsigned int index = 0; index < messageLineCount; index++)
		{
			dialogue_.AddLine(message.at(index).c_str());
		}

		pageCount_ = dialogue_.GetNumPages();
		if (pageCount_)
		{
			lineCount_ = dialogue_.GetPage(0)->GetNumLines();
			if (lineCount_)
			{
				charCount_ = dialogue_.GetPage(0)->GetLine(0).size();
			}
		}
	}

	/**************************************************************************/

	void GameDialogueMessage::ResetDialogue()
	{
		dialogue_.Clear();

		// reset the dialogue data counter variables
		pageCount_ 		= 0;
		lineCount_ 		= 0;
		charCount_ 		= 0;

		currentPage_ = 0;
		currentLine_ = 0;
		currentChar_ = 0;
	}

	/**************************************************************************/

	void GameDialogueMessage::AdvancePage()
	{
		//LogSimpleMessage(
		//	"AdvancePage (before code block): Page(%d/%d) Line(%d/%d) Char(%d/%d)",
		//	currentPage_, pageCount_, currentLine_, lineCount_, currentChar_, charCount_);

		if (currentPage_ < pageCount_ - 1)
		{
			currentPage_++;

			// update the new page info
			currentLine_ 	= 0;
			lineCount_ 		= dialogue_.GetPage(currentPage_)->GetNumLines();
			if (lineCount_)
			{
				charCount_ = dialogue_.GetPage(0)->GetLine(0).size();
			}

			//LogSimpleMessage("Advanced Page...%d/%d", currentPage_, pageCount_);
			currentState_ = DIALOGUE::Busy;
		}
		else
		{
			//LogSimpleMessage("Reached End of Dialogue...");
			// we are at the end of the dialogue, change state
			currentState_ = DIALOGUE::Finished;
		}
		//LogSimpleMessage(
		//	"AdvancePage (after code block): Page(%d/%d) Line(%d/%d) Char(%d/%d)",
		//	currentPage_, pageCount_, currentLine_, lineCount_, currentChar_, charCount_);
	}

	/**************************************************************************/

	void GameDialogueMessage::AdvanceLine()
	{
		if (currentLine_ < lineCount_)
		{
			currentLine_++;

			// update the new line info
			currentChar_ 	= 0;
			charCount_ 		= dialogue_.GetPage(currentPage_)->GetLine(currentLine_).size();
			//LogSimpleMessage("Advanced Line...%d/%d", currentLine_, lineCount_);
		}
		else
		{
			//LogSimpleMessage("Reached End Of Page...");

			// we are at the end of the page, we go idle
			currentState_ = DIALOGUE::Idle;

			if (!(currentPage_ < pageCount_))
			{
				// no more pages, set Finished state
				currentState_ = DIALOGUE::Finished;
			}
		}
	}

	/**************************************************************************/

	void GameDialogueMessage::AdvanceCharacter()
	{
		//LogSimpleMessage("AdvanceCharacter");
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


