#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	class UIPause : public UIMenuType
	{
	public:
		UIPause() = default;
		virtual ~UIPause() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void CloseMenu() override;
		void GoToOptionsMenu();
		void GoToMainMenu();

	private:
		SpriteInstance2D* mySpriteInstance;
	};
}
