#pragma once
#include "UIElement.hpp"

class UI_Button : public UI::UI_Element
{
public:
	UI_Button() = default;

	void Update() override
	{
		UI_Element::Update();
	}

	void OnHover() override
	{
		UI_Element::OnHover();

		if (onHoverCallback)
		{
			onHoverCallback();
		}
	}
	void OnHoverExit() override
	{
		UI_Element::OnHoverExit();

		if (onHoverExitCallback)
		{
			onHoverExitCallback();
		}
	}
	void OnClick() override
	{
		//UI_Element::OnClick();

		if (onClickCallback)
		{
			onClickCallback();
		}
		else
		{
			// Play default click sound if no callback is set
			GetUIManager().PlayUISound(eUISounds::Select);
		}
	}
	void OnClickHeld() override
	{
		//UI_Element::OnClick();

		if (onClickHeldCallback)
		{
			onClickHeldCallback();
		}
	}
	bool GetIsHovered() const
	{
		return isHovered;
	}
	void SetIsHovered(const bool aBool)
	{
		isHovered = aBool;
	}
	void OnEnter()
	{
		// Set up the button
	}
	void OnExit()
	{
		// Clean up the button
	}
};
