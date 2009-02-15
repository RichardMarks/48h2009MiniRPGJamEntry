
// CODESTYLE: v2.0

// GameObject.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a game object

#include "GameLibrary.h"

namespace GAME
{
	GameObject::GameObject(const char* frameImagePathName,
			int frameWidth, int frameHeight,
			int frameCount,
			int frameDelay,
			int initialFrame)
	{
		sprite_ = new GameMapSprite(frameImagePathName, frameWidth, frameHeight, frameCount, frameDelay, initialFrame);
	}

	/**************************************************************************/

	GameObject::~GameObject()
	{
		delete sprite_;
	}

	/**************************************************************************/

	void GameObject::Update()
	{
		sprite_->Update();
	}

	/**************************************************************************/

	void GameObject::Render(ImageResource* target)
	{
		sprite_->Render(target);
	}

	/**************************************************************************/

	GameMapSprite* GameObject::GetSprite() const
	{
		return sprite_;
	}

} // end namespace


