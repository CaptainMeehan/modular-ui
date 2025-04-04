#pragma once
#include <stack>
#include "eUISounds.hpp"
#include "eUIStateID.hpp"

class UI_Button;
namespace Root
{
	class UIMenuType;
}
struct SpriteInstance2D;
class AudioClip;

namespace Root
{

	class UIManager
	{
	public:
		UIManager(const UIManager& aManager) = delete;
		UIManager();
		~UIManager();

		void Init();
		void Update();
		void Render();

		void AddState(eUIStateID aStateID = eUIStateID::UIIngame);
		void Clear();
		void PushStateAndPop(eUIStateID aNewState);
		void PushState(eUIStateID aStateID);
		bool PopState();
		void PopAllExceptIngame();

		void RemoveExitedStates();

		eUIStateID GetCurrentStateID();
		std::shared_ptr<UIMenuType> GetCurrentState();
		std::stack<std::shared_ptr<UIMenuType>> GetStack();
		std::unordered_map<eUIStateID, std::shared_ptr<UIMenuType>> GetCachedStates();
		const int GetStackSize() const;
		const JPH::Float2 GetWindowSize() const;
		UI_Button* GetHoveredButton();
		bool GetIsDragging() { return myIsDragging; }
		AudioClip* GetUISound(eUISounds aSound);

		void SetHoveredButton(UI_Button& aButton);
		void SetIsDragging(bool aBool) { myIsDragging = aBool; }

		void ResetHoveredButton();
		void SwitchMenu();

		bool CheckIfResizeUI(unsigned int aWidth, unsigned int aHeight);
		void CheckShouldFreezeMovement();

		void InitSounds();
		void PlayUISound(eUISounds aSound);
		void PauseUISound(eUISounds aSound, bool aBool);

	private:
		bool mySoundsInitialized = false;
		bool myTimeChange = false;
		bool myIsDragging = false;
		UI_Button* myCurrentlyHoveredButton = {};
		UIMenuType* myCurrentMenu = {};
		std::array<std::unique_ptr<AudioClip>, 11> myUIAudioClips = {};

		std::stack<std::shared_ptr<UIMenuType>> myStack = {};
		std::unordered_map<eUIStateID, std::shared_ptr<UIMenuType>> myCachedStates = {};
	};
}
