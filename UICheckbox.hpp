#pragma once
#include "UIElement.hpp"

class UI_Checkbox : public UI::UI_Element
{
public:
	UI_Checkbox() : isChecked(false) {}

	void Update() override
	{

	}

	bool isChecked;
};
