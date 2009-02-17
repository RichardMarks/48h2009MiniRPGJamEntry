
// CODESTYLE: v2.0

// GameBattleSprite.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a sprite that is used in battle

#include "GameLibrary.h"

namespace GAME
{
	namespace BATTLESPRITE
	{
		SequenceData::SequenceData() :
		isValid_(false),
		firstFrame_(0),
		lastFrame_(0)
		{
		}

		/**********************************************************************/

		SequenceData::~SequenceData()
		{
		}

		/**********************************************************************/

		void SequenceData::Set(int firstFrame, int lastFrame)
		{
			firstFrame_ = firstFrame;
			lastFrame_ = lastFrame;
		}

		/**********************************************************************/

		int SequenceData::GetFirstFrame() const
		{
			return firstFrame_;
		}

		/**********************************************************************/

		int SequenceData::GetLastFrame() const
		{
			return lastFrame_;
		}

		/**********************************************************************/

		void SequenceData::Enable(bool enabled)
		{
			isValid_ = enabled;
		}

		/**********************************************************************/

		bool SequenceData::IsValid() const
		{
			return isValid_;
		}

	} // end namespace

	GameBattleSprite::GameBattleSprite() :
		GameSprite(),
		sequenceType_(BATTLESPRITE::Idle)
	{
		ClearAnimationSequenceData();
	}

	/**************************************************************************/

	GameBattleSprite::GameBattleSprite(
		const char* frameImagePathName,
		int frameWidth, int frameHeight,
		int frameCount,
		int frameDelay,
		int initialFrame) :
		GameSprite(frameImagePathName, frameWidth, frameHeight, frameCount, frameDelay, initialFrame),
		sequenceType_(BATTLESPRITE::Idle)
	{
		ClearAnimationSequenceData();
	}

	/**************************************************************************/

	GameBattleSprite::~GameBattleSprite()
	{
		GameSprite::Destroy();

		sequenceData_.clear();
	}

	/**************************************************************************/

	void GameBattleSprite::Update()
	{
		// GameSprite::Update();
		unsigned int seqIndex = static_cast<unsigned int>(sequenceType_);

		frameCount_ = sequenceData_.at(seqIndex)->GetLastFrame();

		if (!animating_) return;
		if (++frameCounter_ >= frameDelay_)
		{
			frame_++;
			if (frame_ > frameCount_)
			{
				frame_ = sequenceData_.at(seqIndex)->GetFirstFrame();
			}

			frameCounter_ = 0;
		}

	}

	/**************************************************************************/

	void GameBattleSprite::Render(ImageResource* target)
	{
		frames_->BlitMasked(
			target,
			1 + (frame_ * width_) + (frame_), 1,
			screenX_, screenY_,
			width_, height_);
	}

	/**************************************************************************/

	void GameBattleSprite::ClearAnimationSequenceData()
	{
		sequenceData_.clear();

		for (int index = 0; index < BATTLESPRITE::SequenceTypeCount; index++)
		{
			BATTLESPRITE::SequenceData* seqData = new BATTLESPRITE::SequenceData();
			seqData->Enable(false);
			sequenceData_.push_back(seqData);
		}
	}

	/**************************************************************************/

	void GameBattleSprite::SetAnimationSequenceData(BATTLESPRITE::SequenceType seqType, BATTLESPRITE::SequenceData* seqData)
	{
		unsigned int seqIndex = static_cast<unsigned int>(seqType);
		if (0 != sequenceData_.at(seqIndex))
		{
			delete sequenceData_[seqIndex];
			sequenceData_[seqIndex] = new BATTLESPRITE::SequenceData();
			sequenceData_[seqIndex]->Set(seqData->GetFirstFrame(), seqData->GetLastFrame());
			sequenceData_[seqIndex]->Enable();
		}
	}

	/**************************************************************************/

	BATTLESPRITE::SequenceData* GameBattleSprite::GetAnimationSequenceData(BATTLESPRITE::SequenceType seqType)
	{
		unsigned int seqIndex = static_cast<unsigned int>(seqType);
		switch(seqType)
		{
			case BATTLESPRITE::Idle:
			case BATTLESPRITE::Attack:
			case BATTLESPRITE::Defend:
			case BATTLESPRITE::Special:
			case BATTLESPRITE::Injured:
			case BATTLESPRITE::Dying: { return sequenceData_.at(seqIndex); } break;
			default: break;
		}

		// should never be able to reach this code
		return 0;
	}

	/**************************************************************************/

	void GameBattleSprite::SelectSequence(BATTLESPRITE::SequenceType seqType)
	{
		sequenceType_ = seqType;
	}

	/**************************************************************************/

	BATTLESPRITE::SequenceType GameBattleSprite::GetSequence() const
	{
		return sequenceType_;
	}

} // end namespace


