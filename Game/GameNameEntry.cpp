
// CODESTYLE: v2.0

// GameNameEntry.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks

#include "GameLibrary.h"

namespace GAME
{
	char GameNameEntry::keyMap_[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!_ ";

	/**************************************************************************/

	GameNameEntry::GameNameEntry(const std::string& defaultName,int maxChars) :
		maxChars_(maxChars), name_(defaultName), cursor_(0),
		font_(new BitmapFont()),
		overlay_(new ImageResource("data/graphics/game/overlays/nameentry.png")),
		cursorImage_(new ImageResource("data/graphics/battle/overlays/cursor.png"))
	{
		font_->Load("data/graphics/fonts/font5x5white.png", 5, 5, 0);
	}

	/**************************************************************************/

	GameNameEntry::~GameNameEntry()
	{
		delete font_;
		delete overlay_;
		delete cursorImage_;
	}

	/**************************************************************************/

	int GameNameEntry::Update()
	{
		using namespace ENGINE;
		const int numChars = std::strlen(keyMap_);

		if(InputDevice->KeyPressed(KEY::Key_Right)) {
			cursor_++;
		}

		if(InputDevice->KeyPressed(KEY::Key_Left)) {
			if(cursor_) cursor_--;
		}

		if(InputDevice->KeyPressed(KEY::Key_Up)) {
			if(name_.length() != 0 && cursor_ == numChars + 1)
			{
				cursor_--; // if cursor is on cancel, decrement instead of jumping a row
			}
			else cursor_ -= 13;
		}

		if(InputDevice->KeyPressed(KEY::Key_Down)) {
			if(cursor_ >= numChars - 13 && cursor_ < numChars) cursor_ = numChars;
			else cursor_ += 13;
		}

		if(InputDevice->KeyPressed(KEY::Key_Enter)) {
			const int numChar = std::strlen(keyMap_);
			if(cursor_ == numChar) return 1;
			if(cursor_ == numChar + 1) return -1;
			if(name_.length() < static_cast<unsigned int>(maxChars_))
			{
				name_ += keyMap_[cursor_];
			}
		}

		if(InputDevice->KeyPressed(KEY::Key_Backspace)) {
			if(name_.length() > 0)
			{
				name_.resize(name_.length() - 1);
			}
		}

		if(cursor_ < 0) cursor_ += 13;
		if(cursor_ > numChars + 1) cursor_ = numChars + 1;
		if(name_.length() == 0 && cursor_ == numChars) cursor_ = numChars + 1;

		return 0;
	}

	/**************************************************************************/

	void GameNameEntry::Render(ImageResource *target)
	{
		using ENGINE::ColorRGB;

		// background
		overlay_->BlitMasked(target,0,0,0,0,overlay_->GetWidth(),overlay_->GetHeight());

		// name
		font_->Print(target, 42, 17, name_.c_str());

		// output virtual keyboard
		const int numChars = std::strlen(keyMap_);
		for(int n = 0, y = 40; y < 140; y += 14)
		{

			for(int x = 20; x < 180 && n < numChars; x += 13, n++)
			{
				if(n == cursor_)
				{
					cursorImage_->BlitMasked(target,0,0,x - 5,y,cursorImage_->GetWidth(),cursorImage_->GetHeight());
				}

				font_->Print(target, x, y, "%c", keyMap_[n]);
			}
		}

		// if cursor is on special symbols (numChar = accept, numChar + 1 = cancel)
		if(cursor_ == numChars) // accept
		{
			cursorImage_->BlitMasked(target,0,0,145,120,cursorImage_->GetWidth(),cursorImage_->GetHeight());
		}

		if(cursor_ == numChars + 1) // cancel
		{
			cursorImage_->BlitMasked(target,0,0,145,130,cursorImage_->GetWidth(),cursorImage_->GetHeight());
		}

		// accept/cancel
		if(name_.length() != 0) font_->Print(target, 150, 120, "Accept");
		font_->Print(target, 150, 130, "Cancel");
	}

	/**************************************************************************/

	void GameNameEntry::SetName(const std::string& name)
	{
		name_ = name;
	}

	/**************************************************************************/

	std::string GameNameEntry::GetName() const
	{
		return name_;
	}
}
