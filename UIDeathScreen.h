#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UIDeathScreen : public UIMenuType
	{
	public:
		UIDeathScreen() = default;
		virtual ~UIDeathScreen() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void CloseMenu() override;
		std::shared_ptr<UI_Container> myChainTexture;
	};
}
