#include "UIManager.h"

#include "UIMenuType.hpp"
#include "UICredits.h"
#include "UIDeathScreen.h"
#include "UIExit.h"
#include "UIPause.h"
#include "UIIngame.h"
#include "UIIntro.h"
#include "UILevelSelect.h"
#include "UIMain.h"
#include "UIOptions.h"
#include "Audio/AudioClip.h"
#include "Core/Window.h"
int UI::UI_Element::nextID = 0;

Root::UIManager::UIManager()
{
}

Root::UIManager::~UIManager()
{
}

void Root::UIManager::Init()
{
	InitSounds();
	AddState(eUIStateID::UIIngame);
	AddState(eUIStateID::UIIntro);
	AddState(eUIStateID::UIPause);
	AddState(eUIStateID::UIMain);
	AddState(eUIStateID::UIOptions);
	AddState(eUIStateID::UILevelSelect);
	AddState(eUIStateID::UICredits);
	AddState(eUIStateID::UIExit);
	AddState(eUIStateID::UIDeathScreen);

	PushState(eUIStateID::UIIngame);
	PushState(eUIStateID::UIMain);
#ifdef _RETAIL
	PushState(eUIStateID::UIIntro);
#endif
}
void Root::UIManager::Update()
{
	if (myStack.empty())
	{
		PMT("Error: Stack is empty when trying to update!", Log::MessageType::MajorError);
		return;
	}

	RemoveExitedStates();
	CheckShouldFreezeMovement();

	auto currentState = GetCurrentState();
	if (!currentState)
		return;

	currentState->Update();

	auto& buttons = currentState->GetButtons();
	if (buttons.empty())
		return;

	bool hoverDetected = false;

	if (GetIsDragging())
	{
		if (Input::Held(InputAction::Select))
		{
			if (GetHoveredButton())
			{
				GetHoveredButton()->OnHover();
				GetHoveredButton()->OnClickHeld();
				return;
			}
		}
		else
		{
			SetIsDragging(false);
		}
	}
	if (GetHoveredButton())
	{
		if (GetHoveredButton()->IsMouseOver())
		{
			hoverDetected = true;

			if (Input::Pressed(InputAction::Select))
			{
				PM("Clicked button: " + GetHoveredButton()->GetName());
				GetHoveredButton()->OnClick();
			}
			if (Input::Held(InputAction::Select))
			{
				SetIsDragging(true);
				GetHoveredButton()->OnClickHeld();
			}
		}
		else
		{
			GetHoveredButton()->SetIsHovered(false);
			GetHoveredButton()->PlayHighlightEffect(false);
			GetHoveredButton()->OnHoverExit();
			ResetHoveredButton();
		}
	}

	if (!hoverDetected)
	{
		for (auto& button : buttons)
		{
			if (button->IsMouseOver())
			{
				hoverDetected = true;

				SetHoveredButton(*button);
				button->SetIsHovered(true);
				button->OnHover();
				break;
			}
		}
	}
}
void Root::UIManager::Render()
{
	if (!myStack.empty())
	{
		std::vector<std::shared_ptr<UIMenuType>> stackContents;
		std::stack<std::shared_ptr<UIMenuType>> tempStack = myStack;

		while (!tempStack.empty())
		{
			stackContents.push_back(tempStack.top());
			tempStack.pop();
		}

		for (auto it = stackContents.rbegin(); it != stackContents.rend(); ++it)
		{
			auto& state = *it;
			if (state)
			{
				state->Render();
				if (!state->GetIsTransparent())
					break;
			}
		}
	}
	else
	{
		PMT("Error: Stack is empty when trying to render!", Log::MessageType::MajorError);
	}
}

void Root::UIManager::AddState(eUIStateID aStateID)
{
	if (myCachedStates.find(aStateID) != myCachedStates.end())
	{
		PMT("State already exists: " + std::to_string(static_cast<int>(aStateID)), Log::MessageType::MajorError);
		return;
	}

	std::shared_ptr<UIMenuType> aState = nullptr;
	switch (aStateID)
	{
	case eUIStateID::UIIngame:
		aState = std::make_shared<UIIngame>();
		break;
	case eUIStateID::UIIntro:
		aState = std::make_shared<UIIntro>();
		break;
	case eUIStateID::UIPause:
		aState = std::make_shared<UIPause>();
		break;
	case eUIStateID::UIMain:
		aState = std::make_shared<UIMain>();
		break;
	case eUIStateID::UIOptions:
		aState = std::make_shared<UIOptions>();
		break;
	case eUIStateID::UILevelSelect:
		aState = std::make_shared<UILevelSelect>();
		break;
	case eUIStateID::UICredits:
		aState = std::make_shared<UICredits>();
		break;
	case eUIStateID::UIExit:
		aState = std::make_shared<UIExit>();
		break;
	case eUIStateID::UIDeathScreen:
		aState = std::make_shared<UIDeathScreen>();
		break;
	default:
		PMT("State not found: " + std::to_string(static_cast<int>(aStateID)), Log::MessageType::MajorError);
		break;
	}
	aState->SetStateID(aStateID);
	aState->Init();
	myCachedStates[aStateID] = std::move(aState);
}
void Root::UIManager::Clear()
{
	while (myStack.size() > 0)
	{
		myStack.top().reset();
		myStack.pop();
	}
	ResetHoveredButton();
}
void Root::UIManager::PushStateAndPop(eUIStateID aNewState)
{
	if (!myStack.empty())
	{
		auto& currentTop = myStack.top();
		if (currentTop)
		{
			currentTop->OnExit();
			currentTop->ExitState();
		}
		myStack.pop();
	}

	ResetHoveredButton();
	PushState(aNewState);
}
void Root::UIManager::PushState(eUIStateID aStateID)
{
	auto it = myCachedStates.find(aStateID);
	myTimeChange = false;

	if (it != myCachedStates.end())
	{
		if (!myStack.empty() && myStack.top())
			myStack.top()->OnExit();

		auto& menu = it->second;
		SwitchMenu();
		myStack.push(menu);
		myStack.top()->OnEnter();
	}
	else
	{
		PMT("State not found: " + std::to_string(static_cast<int>(aStateID)), Log::MessageType::MajorError);
	}
}
bool Root::UIManager::PopState()
{
	if (myStack.empty())
	{
		PMT("Error: Can't Pop. Stack is empty!", Log::MessageType::MajorError);
		return false;
	}

	myTimeChange = false;

	auto& topState = myStack.top();
	if (topState)
	{
		PMT("Exiting state: " + std::to_string(static_cast<int>(topState->GetStateID())), Log::MessageType::Notice);
		topState->OnExit();
	}
	SwitchMenu();
	myStack.pop();

	if (!myStack.empty() && myStack.top())
	{
		PMT("Entering state: " + std::to_string(static_cast<int>(myStack.top()->GetStateID())), Log::MessageType::Notice);
		myStack.top()->OnEnter();
	}
	return true;
}
void Root::UIManager::PopAllExceptIngame()
{
	while (!myStack.empty())
	{
		auto& topState = myStack.top();

		if (topState && topState->GetStateID() == eUIStateID::UIIngame)
		{
			SwitchMenu();
			topState->OnEnter();
			return;
		}

		if (topState)
		{
			topState->OnExit();
		}
		myStack.pop();
	}
}
void Root::UIManager::RemoveExitedStates()
{
	while (myStack.size() > 0 && myStack.top()->GetIsExited())
	{
		myStack.top().reset();
		myStack.pop();
	}
}
eUIStateID Root::UIManager::GetCurrentStateID()
{
	if (!myStack.empty())
	{
		auto& currentState = myStack.top();
		if (currentState)
		{
			return currentState->GetStateID();
		}
		PMT("Error: Current state is expired! Returning UINoState...", Log::MessageType::MajorError);
		return eUIStateID::UINoState;
	}
	else
	{
		PMT("Error: Stack is empty! Returning UINoState...", Log::MessageType::MajorError);
		return eUIStateID::UINoState;
	}
}
std::shared_ptr<Root::UIMenuType> Root::UIManager::GetCurrentState()
{
	if (!myStack.empty())
	{
		auto& currentState = myStack.top();
		if (currentState)
		{
			return currentState;
		}
		PMT("Error: Current state is expired!", Log::MessageType::MajorError);
	}
	else
	{
		PMT("Error: Stack is empty! Returning empty UIMenuType...", Log::MessageType::MajorError);
	}

	return std::shared_ptr<UIMenuType>();
}
std::stack<std::shared_ptr<Root::UIMenuType>> Root::UIManager::GetStack()
{
	return myStack;
}
std::unordered_map<eUIStateID, std::shared_ptr<Root::UIMenuType>> Root::UIManager::GetCachedStates()
{
	return myCachedStates;
}
const int Root::UIManager::GetStackSize() const
{
	return static_cast<int>(myStack.size());
}
const JPH::Float2 Root::UIManager::GetWindowSize() const
{
	//auto& engine = Engine::Get();
	//JPH::Float2 res = engine.GetGraphicsEngine().GetResolution();
	JPH::Float2 res;
	res.x = (float)Window::Get().GetWindowSettings().activeArea.right;
	res.y = (float)Window::Get().GetWindowSettings().activeArea.bottom;
	return res;
}

UI_Button* Root::UIManager::GetHoveredButton()
{
	return myCurrentlyHoveredButton;
}
void Root::UIManager::SetHoveredButton(UI_Button& aButton)
{
	if (myCurrentlyHoveredButton != &aButton)
	{
		if (myCurrentlyHoveredButton)
		{
			// Perform any hover-end logic for the previous button.
			//myCurrentlyHoveredButton->OnHoverEnd();
		}

		myCurrentlyHoveredButton = &aButton;

		if (myCurrentlyHoveredButton)
		{
			// Perform any hover-start logic for the new button.
			//myCurrentlyHoveredButton->OnHoverStart();
		}
	}
}
void Root::UIManager::ResetHoveredButton()
{
	if (myCurrentlyHoveredButton)
	{
		myCurrentlyHoveredButton = nullptr;
	}
}
void Root::UIManager::SwitchMenu()
{
	if(GetHoveredButton())
	{
		GetHoveredButton()->SetIsHovered(false);
		GetHoveredButton()->PlayHighlightEffect(false);
		GetHoveredButton()->OnHoverExit();
	}
	ResetHoveredButton();
}

void Root::UIManager::InitSounds()
{
	if (mySoundsInitialized)
	{
		return;
	}

	AudioManager& audioManager = AudioManager::Get();

	const std::unordered_map<eUISounds, std::string> soundEvents =
	{
		{eUISounds::UltimateReady, "event:/UI/gameplay/ability/ultimate_ready"},
		{eUISounds::Back, "event:/UI/menu/back"},
		{eUISounds::Forward, "event:/UI/menu/forward"},
		{eUISounds::Hover, "event:/UI/menu/hover"},
		{eUISounds::MainMenuGameStart, "event:/UI/menu/main_menu_game_start"},
		{eUISounds::Pause, "event:/UI/menu/pause"},
		{eUISounds::Select, "event:/UI/menu/select"},
		{eUISounds::Slider, "event:/UI/menu/slider"},
		{eUISounds::UnPause, "event:/UI/menu/unpause"},
		{eUISounds::MusicMain, "event:/music/music_main_menu"},
		{eUISounds::MusicCredits, "event:/music/credits"}
	};

	for (int i = 0; i < static_cast<int>(eUISounds::Count); ++i)
	{
		if (!myUIAudioClips[i])
		{
			auto it = soundEvents.find(static_cast<eUISounds>(i));
			if (it != soundEvents.end())
			{
				myUIAudioClips[i] = std::unique_ptr<AudioClip>(audioManager.CreateSound(it->second));
			}
		}
	}

	mySoundsInitialized = true;
}
void Root::UIManager::PlayUISound(eUISounds aSound)
{
	if (myUIAudioClips[static_cast<int>(aSound)])
	{
		myUIAudioClips[static_cast<int>(aSound)]->Play();
	}
}
void Root::UIManager::PauseUISound(eUISounds aSound, bool aBool)
{
	if (myUIAudioClips[static_cast<int>(aSound)])
	{
		if (myUIAudioClips[static_cast<int>(aSound)]->IsPlaying() && aBool == true)
		{
			myUIAudioClips[static_cast<int>(aSound)]->Pause(true);
		}
		else if (!myUIAudioClips[static_cast<int>(aSound)]->IsPlaying() && aBool == false)
		{
			myUIAudioClips[static_cast<int>(aSound)]->Pause(false);
		}
	}
}
AudioClip* Root::UIManager::GetUISound(eUISounds aSound)
{
	if (myUIAudioClips[static_cast<int>(aSound)])
	{
		return myUIAudioClips[static_cast<int>(aSound)].get();
	}

	PMT("Error: Sound not found: " + std::to_string(static_cast<int>(aSound)), Log::MessageType::MajorError);
	return nullptr;
}

void Root::UIManager::CheckShouldFreezeMovement()
{
	// BUG : this might break something...
	if (myTimeChange == true)
		return;

	myTimeChange = true;

	if (!myStack.empty())
	{
		if (myStack.top()->GetShouldFreezeMovement())
		{
			TinTime::deltaTimeScale = 0.0f;
		}
		else
		{
			TinTime::deltaTimeScale = 1.0f;
		}
	}
}
bool Root::UIManager::CheckIfResizeUI(unsigned int aWidth, unsigned int aHeight)
{
	for (auto& state : myCachedStates)
	{
		if(!state.second->CheckIfResizeUI(aWidth, aHeight))
		{
			return false;
		}
	}
	return true;
}

