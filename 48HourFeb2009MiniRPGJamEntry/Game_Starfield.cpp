
// CODESTYLE: v2.0

// Game_Starfield.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation of the Starfield child class for the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	GameSingleton::Starfield::Starfield(int* fov, int count, int maxSpeed)
	{
		fov_ 		= fov;
		starCount_ 	= count;

		stars_ = new GameSingleton::Star [starCount_];

		for (int index = 0; index < starCount_; index++)
		{
			ColorRGB starColor(255 - index / 8, 255 - index / 8, 255 - index / 8);

			stars_[index].color_ = starColor.Get();

			int starX = fov[0] + rand() % (fov[2] - fov[0]);
			int starY = fov[1] + rand() % (fov[3] - fov[1]);

			stars_[index].screenX_ = starX;
			stars_[index].screenY_ = starY;

			int starSpeed = 1 + rand() % (maxSpeed - 1);

			stars_[index].speedY_ = starSpeed;
		}

	}

	/**************************************************************************/

	GameSingleton::Starfield::~Starfield()
	{
		delete [] stars_;
	}

	/**************************************************************************/

	void GameSingleton::Starfield::Update()
	{
		for (int index = 0; index < starCount_; index++)
		{
			stars_[index].screenY_ += stars_[index].speedY_;
			if (stars_[index].screenY_ > fov_[3])
			{
				stars_[index].screenY_ = fov_[1];
			}
		}
	}

	/**************************************************************************/

	void GameSingleton::Starfield::Render(ImageResource* target)
	{
		for (int index = 0; index < starCount_; index++)
		{
			target->SetPixel(stars_[index].screenX_, stars_[index].screenY_, stars_[index].color_);
		}
	}

} // end namespace

