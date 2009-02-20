
// CODESTYLE: v2.0

// GameBattleSprite.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a sprite that is used in battle

#ifndef __GAMEBATTLESPRITE_H__
#define __GAMEBATTLESPRITE_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

#include "GameSprite.h"

namespace GAME
{
	namespace BATTLESPRITE
	{
		enum SequenceType
		{
			/// the idle animation sequence will be used
			Idle,

			/// the attacking animation sequence will be used
			Attack,

			/// the defending animation sequence will be used
			Defend,

			/// the special skill animation sequence will be used
			Special,

			/// the injured animation sequence will be used
			Injured,

			/// the dying animation sequence will be used
			Dying,

			/// the number of sequence types that are available
			SequenceTypeCount
		};

		/// a class to hold animation sequence data
		class SequenceData
		{
		public:
			SequenceData();
			~SequenceData();

			/// sets the animation sequence starting and ending frame index values
			void Set(int firstFrame, int lastFrame);

			/// gets the animation sequence starting frame index value
			int GetFirstFrame() const;

			/// gets the animation sequence ending frame index value
			int GetLastFrame() const;

			/// enables or disables the sequence
			void Enable(bool enabled = true);

			/// returns the validity of the sequence data
			bool IsValid() const;

		private:
			SequenceData(const SequenceData& rhs);
			const SequenceData& operator=(const SequenceData& rhs);

		private:
			/// is this data valid?
			bool isValid_;

			/// the first frame index of the sequence
			int firstFrame_;

			/// the last frame index of the sequence
			int lastFrame_;
		};

	} // end namespace

	class GameBattleSprite : public GameSprite
	{
	public:
		GameBattleSprite();

		GameBattleSprite(
			const char* frameImagePathName,
			int frameWidth, int frameHeight,
			int frameCount,
			int frameDelay,
			int initialFrame = 0);

		~GameBattleSprite();

		/// updates the sprite
		void Update();

		/// renders the sprite
		void Render(ImageResource* target);

		/// deletes the animation sequence data and re-inits new zeroed-out data pointers. you need to call SetAnimationSequenceData again.
		void ClearAnimationSequenceData();

		/// sets the animation sequence data for the specified sequence type
		void SetAnimationSequenceData(BATTLESPRITE::SequenceType seqType, BATTLESPRITE::SequenceData* seqData);

		/// gets a pointer to the animation sequence data for the specified sequence type
		BATTLESPRITE::SequenceData* GetAnimationSequenceData(BATTLESPRITE::SequenceType seqType);

		/// selects the animation sequence to use
		void SelectSequence(BATTLESPRITE::SequenceType seqType);

		/// gets the selected animation sequence
		BATTLESPRITE::SequenceType GetSequence() const;

		/// gets a pointer to the image resource that holds the frames of the sprite
		ImageResource* GetFramesImage() const;

	private:
		GameBattleSprite(const GameBattleSprite& rhs);
		const GameBattleSprite& operator=(const GameBattleSprite& rhs);

	private:
		/// a list of animation sequence data
		std::vector<BATTLESPRITE::SequenceData*> sequenceData_;

		/// the animation sequence that we should currently use
		BATTLESPRITE::SequenceType sequenceType_;
	};

} // end namespace
#endif


