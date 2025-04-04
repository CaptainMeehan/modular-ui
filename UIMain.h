#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UIMain : public UIMenuType
	{
	public:
		UIMain() = default;
		virtual ~UIMain() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void GoIngame();
		void GoToSettingsMenu();
		void GoToLevelSelectMenu();
		void GoToCreditsMenu();
	};
}
