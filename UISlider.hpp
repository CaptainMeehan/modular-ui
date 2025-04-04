#pragma once
#include "UIElement.hpp"

class UI_Slider : public UI::UI_Element
{
public:
	UI_Slider()
		: myMinValue(0.0f), myMaxValue(1.0f), myCurrentValue(1.0f), myIsDragging(false)
	{
	}

	void Update() override
	{
		UI_Element::Update();
	}

	const float& GetValue() const
	{
		return myCurrentValue;
	}

	void SetValue(const float& aSliderValue)
	{
		//float sliderStart = GetPositionInPercent().x - (GetSizeInPercent().x * 0.5f);
		//float sliderEnd = GetPositionInPercent().x + (GetSizeInPercent().x * 0.5f);
		//float sliderLengthPercent = GetSizeInPercent().x;

		myCurrentValue = std::fmax(myMinValue, std::fmin(aSliderValue, myMaxValue));

		//mySliderHandle->GetSprite().pos = 
	}
	void SetOnValueChanged(std::function<void(float)> aCallback)
	{
		myOnValueChanged = aCallback;
	}
	void SetHandle(std::shared_ptr<UI_Button>& aHandle)
	{
		mySliderHandle = aHandle;
	}
	void UpdateHandlePosition()
	{
		if (mySliderHandle)
		{
			JPH::Float2 mousePos = Engine::Get().GetGraphicsEngine().GetScreenPercentPos();
			auto& sliderSprite = GetSprite();
			auto& handleSprite = mySliderHandle->GetSprite();
			float halfHandleWidth = mySliderHandle->GetSizeInPercent().x * 0.5f;

			float sliderStart = GetPositionInPercent().x - (GetSizeInPercent().x * 0.5f) + halfHandleWidth;
			float sliderEnd = GetPositionInPercent().x + (GetSizeInPercent().x * 0.5f) - halfHandleWidth;
			float sliderLengthPercent = GetSizeInPercent().x;

			float clampedPos = std::fmax(sliderStart, std::fmin(mousePos.x, sliderEnd));

			// Convert position to a percentage-based value
			float normalizedValue = (clampedPos - sliderStart) / sliderLengthPercent;
			myCurrentValue = myMinValue + normalizedValue * (myMaxValue - myMinValue);

			// Move handle
			handleSprite.pos.x = clampedPos;
			handleSprite.pos.y = sliderSprite.pos.y;

			if (myOnValueChanged)
			{
				myOnValueChanged(myCurrentValue);
			}
		}
	}
	void SetScreenPositionInPercent(const JPH::Float2& aPosition) override
	{
		myPositionPercent = aPosition;

		if (mySpriteInstance)
		{
			mySpriteInstance->pos = aPosition;
			if (mySliderHandle)
			{
				mySliderHandle->SetScreenPositionInPercent(mySpriteInstance->pos);
			}
		}
	}

private:

	std::shared_ptr<UI_Button> mySliderHandle;
	std::function<void(float)> myOnValueChanged;
	float myMinValue;
	float myMaxValue;
	float myCurrentValue;
	bool myIsDragging;
};
