#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UICredits : public UIMenuType
	{
	public:
		UICredits() = default;
		virtual ~UICredits() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void CloseMenu() override;

	private:
		SpriteInstance2D* mySpriteInstance;
	};
}
