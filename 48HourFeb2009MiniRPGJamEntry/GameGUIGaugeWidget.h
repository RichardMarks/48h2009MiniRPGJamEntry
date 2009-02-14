
// CODESTYLE: v2.0

// GameGUIGaugeWidget.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: a sweet little GUI bar gauge widget

#ifndef __GAMEGUIGAUGE_H__
#define __GAMEGUIGAUGE_H__

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	enum GameGUIGaugeType
	{
		DUVerticalGUIGauge, 	// Down -> Up
		UDVerticalGUIGauge, 	// Up -> Down
		RLHorizontalGUIGauge, 	// Right -> Left
		LRHorizontalGUIGauge 	// Left -> Right
	};
	
	class GameGUIGauge
	{
	public:
		GameGUIGauge();
		GameGUIGauge(GameGUIGaugeType gaugeType, int x, int y, int width, int height, float maxValue, int borderColor, int fillColor);
		~GameGUIGauge();
		
		void SetType(GameGUIGaugeType gaugeType);
		void SetPosition(int x, int y);
		void SetSize(int width, int height);
		void SetBorderColor(int color);
		void SetFillColor(int color);
		void SetMaximumValue(float value);
		void Set(float value);
		void Reset();
		
		GameGUIGaugeType GetType() const;
		float GetCurrentValue() const;
		float GetMaximumValue() const;
		float GetPercentage() const;
		
		int GetX() const;
		int GetY() const;
		int GetWidth() const;
		int GetHeight() const;
		
		void Update(float amount);
		void Render(ENGINE::ImageResource* target);
		
	private:
		GameGUIGauge(const GameGUIGauge& rhs);
		const GameGUIGauge& operator=(const GameGUIGauge& rhs);
		
	private:
	
		int x_;
		int y_;
		int width_;
		int height_;
		float curValue_;
		float maxValue_;
		int borderColor_;
		int fillColor_;
		float percentage_;
		GameGUIGaugeType gaugeType_;
		
	}; // end class

} // end namespace
#endif


