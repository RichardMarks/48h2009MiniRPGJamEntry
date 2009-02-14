
// CODESTYLE: v2.0

// GameObject.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a game object

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;
	
	class GameMapSprite;
	
	class GameObject
	{
	public:
		GameObject(const char* frameImagePathName, 
			int frameWidth, int frameHeight,
			int frameCount,
			int frameDelay,
			int initialFrame = 0);
		
		~GameObject();
		
		void Update();
		void Render(ImageResource* target);
		
		GameMapSprite* GetSprite() const;
		// GameEntity* GetEntity() const;
		
	private:
		GameObject(const GameObject& rhs);
		const GameObject& operator=(const GameObject& rhs);
		
	private:
		GameMapSprite* sprite_; // the animated sprite of the object
		// GameEntity* entity_; // the entity data of the object
	};
	
} // end namespace
#endif


