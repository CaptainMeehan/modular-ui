#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UILevelSelect : public UIMenuType
	{
	public:
		UILevelSelect() = default;
		virtual ~UILevelSelect() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void CloseMenu() override;
		void GoToLevel(int aLevel);

	private:
	};
}
