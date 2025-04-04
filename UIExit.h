#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UIExit : public UIMenuType
	{
	public:
		UIExit() = default;
		virtual ~UIExit() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
	};
}
