
// CODESTYLE: v2.0

// GameGUIGaugeWidget.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: a sweet little GUI bar gauge widget

#include "GameLibrary.h"

namespace GAME
{
	/*
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
	*/
	GameGUIGauge::GameGUIGauge() :
		x_(0),
		y_(0),
		width_(0),
		height_(0),
		curValue_(0.0f),
		maxValue_(0.0f),
		borderColor_(0),
		fillColor_(0),
		percentage_(0.0f),
		gaugeType_(LRHorizontalGUIGauge)
	{
	}

	/**************************************************************************/

	GameGUIGauge::GameGUIGauge(GameGUIGaugeType gaugeType, int x, int y, int width, int height, float maxValue, int borderColor, int fillColor) :
		x_(x),
		y_(y),
		width_(width),
		height_(height),
		curValue_(0.0f),
		maxValue_(maxValue),
		borderColor_(borderColor),
		fillColor_(fillColor),
		percentage_(0.0f),
		gaugeType_(gaugeType)
	{
		if (gaugeType_ == UDVerticalGUIGauge || gaugeType_ == LRHorizontalGUIGauge)
		{
			curValue_ = maxValue_;
		}
	}

	/**************************************************************************/

	GameGUIGauge::~GameGUIGauge()
	{
	}

	/**************************************************************************/
	void GameGUIGauge::SetType(GameGUIGaugeType gaugeType)
	{
		gaugeType_ = gaugeType;
	}

	void GameGUIGauge::SetPosition(int x, int y)
	{
		x_ = x;
		y_ = y;
	}

	/**************************************************************************/

	void GameGUIGauge::SetSize(int width, int height)
	{
		width_ = width;
		height_ = height;
	}

	/**************************************************************************/

	void GameGUIGauge::SetBorderColor(int color)
	{
		borderColor_ = color;
	}

	/**************************************************************************/

	void GameGUIGauge::SetFillColor(int color)
	{
		fillColor_ = color;
	}

	/**************************************************************************/

	void GameGUIGauge::SetMaximumValue(float value)
	{
		maxValue_ = value;
	}

	/**************************************************************************/

	void GameGUIGauge::Set(float value)
	{
		curValue_ = value;
	}

	/**************************************************************************/

	void GameGUIGauge::Reset()
	{
		if (gaugeType_ == UDVerticalGUIGauge || gaugeType_ == LRHorizontalGUIGauge)
		{
			curValue_ = maxValue_;
		}
		else
		{
			curValue_ = 0.0f;
		}
	}

	/**************************************************************************/
	GameGUIGaugeType GameGUIGauge::GetType() const
	{
		return gaugeType_;
	}

	float GameGUIGauge::GetCurrentValue() const
	{
		return curValue_;
	}

	/**************************************************************************/

	float GameGUIGauge::GetMaximumValue() const
	{
		return maxValue_;
	}

	/**************************************************************************/

	float GameGUIGauge::GetPercentage() const
	{
		return percentage_;
	}

	/**************************************************************************/

	int GameGUIGauge::GetX() const
	{
		return x_;
	}

	/**************************************************************************/

	int GameGUIGauge::GetY() const
	{
		return y_;
	}

	/**************************************************************************/

	int GameGUIGauge::GetWidth() const
	{
		return width_;
	}

	/**************************************************************************/

	int GameGUIGauge::GetHeight() const
	{
		return height_;
	}

	/**************************************************************************/

	void GameGUIGauge::Update(float amount)
	{
		switch(gaugeType_)
		{
			case RLHorizontalGUIGauge:
			case DUVerticalGUIGauge:
			{
				curValue_ += amount;
				if (curValue_ > maxValue_)
				{
					curValue_ = maxValue_;
				}
			} break;

			case LRHorizontalGUIGauge:
			case UDVerticalGUIGauge:
			{
				curValue_ -= amount;
				if (curValue_ < 0.0f)
				{
					curValue_ = 0.0f;
				}
			} break;

			default: break;
		}

		percentage_ = curValue_ / maxValue_;

		#if 0
		fprintf(stderr,
			"(%d, %d) - (%d, %d) %f/%f %f%%\n",
			x_, y_, x_ + width_, y_ + height_,
			curValue_, maxValue_, percentage_);
		#endif

		// increase curValue
		#if 0
		if (curValue_ < maxValue_)
		{
			curValue_ += amount;
			if (curValue_ > maxValue_)
			{
				curValue_ = maxValue_;
			}
		}

		// update percentage
		percentage_ = curValue_ / maxValue_;

		#endif

	}

	/**************************************************************************/

	void GameGUIGauge::Render(ENGINE::ImageResource* target)
	{
		switch(gaugeType_)
		{
			case DUVerticalGUIGauge:
			{
				int h = static_cast<int>(percentage_ * static_cast<float>(height_));
				if (width_ == 1)
				{
					target->Line(x_, y_, x_, y_ + height_, borderColor_);
					target->Line(x_, y_, x_, y_ + (height_ - h), fillColor_);
				}
				else
				{
					target->Rect(x_, y_, x_ + width_, y_ + height_, borderColor_, true);
					target->Rect(x_, y_, x_ + width_, y_ + (height_ - h), fillColor_, true);
				}
			} break;

			case UDVerticalGUIGauge:
			{
				int h = static_cast<int>(percentage_ * static_cast<float>(height_));
				if (width_ == 1)
				{
					target->Line(x_, y_, x_, y_ + height_, borderColor_);
					target->Line(x_, y_ + h, x_, y_ + height_, fillColor_);
				}
				else
				{
					target->Rect(x_, y_, x_ + width_, y_ + height_, borderColor_, true);
					target->Rect(x_, y_ + h, x_ + width_, y_ + height_, fillColor_, true);
				}
			} break;

			case RLHorizontalGUIGauge:
			{
				int w = static_cast<int>(percentage_ * static_cast<float>(width_));
				if (height_ == 1)
				{
					target->Line(x_, y_, x_ + width_, y_, borderColor_);
					target->Line(x_ + w, y_, x_ + width_, y_, fillColor_);
				}
				else
				{
					target->Rect(x_, y_, x_ + width_, y_ + height_, borderColor_, true);
					target->Rect(x_ + w, y_, x_ + width_, y_ + height_, fillColor_, true);
				}
			} break;

			case LRHorizontalGUIGauge:
			{
				int w = static_cast<int>(percentage_ * static_cast<float>(width_));
				if (height_ == 1)
				{
					target->Line(x_, y_, x_ + width_, y_, borderColor_);
					target->Line(x_, y_, x_ + (width_ - w), y_, fillColor_);
				}
				else
				{
					target->Rect(x_, y_, x_ + width_, y_ + height_, borderColor_, true);
					target->Rect(x_, y_, x_ + (width_ - w), y_ + height_, fillColor_, true);
				}
			} break;

			default: break;
		}

	}

} // end namespace


