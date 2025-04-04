#pragma once
#include "UIMenuType.hpp"
#include "Graphics/Managers/MaterialManager.h"

namespace Root
{
	class UIMenuScreen
	{
	public:
		UIMenuScreen(EntityID aMainEntityID, EntityID aComputerEntityID, EntityID aScreenEntityID)
		{
			myMainEntityID = aMainEntityID;
			myComputerID = aComputerEntityID;
			myScreenEntityID = aScreenEntityID;
		}
		~UIMenuScreen() = default;

		EntityID GetMainEntityID() const { return myMainEntityID; }
		EntityID GetComputerID() const { return myComputerID; }
		EntityID GetScreenEntityID() const { return myScreenEntityID; }
		JPH::Mat44 GetScreenTransform() const { return Engine::Get().GetWorld().GetEntity(myScreenEntityID).GetTransform(); }
		void Screen() const
		{
			auto trans = GetScreenTransform();

		}
	private:
		EntityID myMainEntityID;
		EntityID myComputerID;
		EntityID myScreenEntityID;
		//auto transform = Root::Engine::Get().GetWorld().GetEntity(myMainEntityID).GetTransform();
	};
}
