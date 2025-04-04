#include "UI/UIPause.h"

#include <functional>

#include "eUISounds.hpp"
#include "eUIStateID.hpp"
#include "UIContainer.hpp"
#include "UIElement.hpp"
#include "UIManager.h"
#include "Engine/Input/Input.h"

void Root::UIPause::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string backdropSpritePath = "../ArtAssets/Materials/CRTScreen/T_Background_C.dds";
	std::string menuPath = "../ArtAssets/UI/UI/T_Background_C.dds";
	std::string playTextPath = "../ArtAssets/UI/UI/MainMenu/T_StartButton_C.dds";
	std::string settingsTextPath = "../ArtAssets/UI/UI/Settings/T_SettingsButton_C.dds";
	std::string levelSelectTextPath = "../ArtAssets/UI/UI/LevelSelect/T_LevelSelectButton_C.dds";
	std::string creditsTextPath = "../ArtAssets/UI/UI/MainMenu/T_CreditsButton_C.dds";
	std::string mainMenuTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_MainMenuButton_C.dds";
	std::string exitTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_ExitButton_C.dds";

	auto backdrop = AddContainer(backdropSpritePath);
	backdrop->SetScale({ resolution.x, resolution.y });
	backdrop->SetName("BackdropContainer");
	backdrop->GetSprite().color = { 0.0f, 0.0f, 0.0f, 0.0f };

	auto pauseMenu = AddContainer(menuPath, backdrop);
	pauseMenu->SetName("MenuContainer");
	pauseMenu->SetScale({ pauseMenu->GetSprite().texture->GetResolution().x, pauseMenu->GetSprite().texture->GetResolution().x });

	auto topExitButton = AddButton(exitTextSpritePath, pauseMenu);
	auto& topExitButtonSprite = topExitButton->GetSprite();
	topExitButtonSprite.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	topExitButton->SetScale({ pauseMenu->GetSprite().scale.x * 0.02f,
									pauseMenu->GetSprite().scale.x * 0.02f });
	topExitButton->SetPosFromParentRight(-0.115f);
	topExitButton->SetPosFromParentTop(-0.002f);
	topExitButton->SetName("TopExitButton");
	topExitButton->SetOnClick([this]() { CloseMenu(); });

	auto playButton = AddButton(playTextPath, pauseMenu);
	auto& playButtonSprite = playButton->GetSprite();
	float playButtonAspectRatio = playButtonSprite.scale.y / playButtonSprite.scale.x;
	playButton->SetScale({ pauseMenu->GetSprite().scale.x * 0.2f,
								pauseMenu->GetSprite().scale.x * 0.2f * playButtonAspectRatio });
	playButton->SetPosFromParentLeft(0.0f);
	playButton->SetPosFromParentTop(0.3f);
	playButton->SetName("PlayButton");
	playButton->SetOnClick([this]() { CloseMenu(); });

	auto settingsButton = AddButton(settingsTextPath, pauseMenu);
	auto& settingsButtonSprite = settingsButton->GetSprite();
	float settingsButtonAspectRatio = settingsButtonSprite.scale.y / settingsButtonSprite.scale.x;
	settingsButton->SetScale({ pauseMenu->GetSprite().scale.x * 0.2f,
									pauseMenu->GetSprite().scale.x * 0.2f * settingsButtonAspectRatio });
	settingsButton->SetPosFromParentLeft(0.0f);
	settingsButton->SetPosFromParentTop(0.35f);
	settingsButton->SetName("SettingsButton");
	settingsButton->SetOnClick([this]() { GoToOptionsMenu(); });

	auto mainMenuButton = AddButton(mainMenuTextSpritePath, pauseMenu);
	auto& mainMenuButtonSprite = mainMenuButton->GetSprite();
	float mainMenuButtonAspectRatio = mainMenuButtonSprite.scale.y / mainMenuButtonSprite.scale.x;
	mainMenuButton->SetScale({ pauseMenu->GetSprite().scale.x * 0.2f,
									pauseMenu->GetSprite().scale.x * 0.2f * mainMenuButtonAspectRatio });
	mainMenuButton->SetPosFromParentLeft(0.0f);
	mainMenuButton->SetPosFromParentTop(0.4f);
	mainMenuButton->SetName("MainMenuButton");
	mainMenuButton->SetOnClick([this]() { GoToMainMenu(); });

	auto exitButton = AddButton(exitTextSpritePath, pauseMenu);
	auto& exitButtonSprite = exitButton->GetSprite();
	float exitButtonAspectRatio = exitButtonSprite.scale.y / exitButtonSprite.scale.x;
	exitButton->SetScale({ pauseMenu->GetSprite().scale.x * 0.2f,
									pauseMenu->GetSprite().scale.x * 0.2f * exitButtonAspectRatio });
	exitButton->SetPosFromParentLeft(0.0f);
	exitButton->SetPosFromParentTop(0.75f);
	exitButton->SetName("ExitButton");
	exitButton->SetOnClick([this]() { ExitGame(); });

	myStateID = eUIStateID::UIPause;
	myLetThroughRender = true;
}
void Root::UIPause::Update()
{
	UIMenuType::Update();
	if (Input::Pressed(InputAction::Exit))
	{
		CloseMenu();
	}
}
void Root::UIPause::Render()
{
	UIMenuType::Render();
}
void Root::UIPause::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIPause state.", Log::MessageType::Notice);
}
void Root::UIPause::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIPause state.", Log::MessageType::Notice);
}
void Root::UIPause::CloseMenu()
{
	if (GetUIManager().GetCurrentStateID() == eUIStateID::UIPause && GetUIManager().PopState())
	{
		//GetUIManager().PlayUISound(eUISounds::Back);
	}
}
void Root::UIPause::GoToOptionsMenu()
{
	GetUIManager().PushState(eUIStateID::UIOptions);
	//GetUIManager().PlayUISound(eUISounds::Select);
}
void Root::UIPause::GoToMainMenu()
{
	GetUIManager().PopState();
	GetUIManager().PushState(eUIStateID::UIMain);
	//GetUIManager().PlayUISound(eUISounds::Select);
}