#include "UI/UIMain.h"

#include "UIManager.h"
#include "Engine/Core/SceneManager.h"

void Root::UIMain::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string backdropSpritePath = "../ArtAssets/UI/UI/T_EndscreenBlack_C.dds";
	std::string menuPath = "../ArtAssets/UI/UI/T_Background_C.dds";
	std::string playTextPath = "../ArtAssets/UI/UI/MainMenu/T_StartButton_C.dds";
	std::string settingsTextPath = "../ArtAssets/UI/UI/Settings/T_SettingsButton_C.dds";
	std::string levelSelectTextPath = "../ArtAssets/UI/UI/LevelSelect/T_LevelSelectButton_C.dds";
	std::string creditsTextPath = "../ArtAssets/UI/UI/MainMenu/T_CreditsButton_C.dds";
	std::string exitTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_ExitButton_C.dds";

	auto backdrop = AddContainer(backdropSpritePath);
	backdrop->SetScale({ resolution.x, resolution.y});
	backdrop->SetName("BackdropContainer");
	//backdrop->GetSprite().color = { 0.0f, 0.0f, 0.0f, 0.0f };

	auto mainMenu = AddContainer(menuPath, backdrop);
	mainMenu->SetName("MenuContainer");
	mainMenu->SetScale({ mainMenu->GetSprite().texture->GetResolution().x, mainMenu->GetSprite().texture->GetResolution().x });

	auto topExitButton = AddButton(exitTextSpritePath, mainMenu);
	auto& topExitButtonSprite = topExitButton->GetSprite();
	topExitButtonSprite.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	topExitButton->SetScale({ mainMenu->GetSprite().scale.x * 0.02f,
									mainMenu->GetSprite().scale.x * 0.02f });
	topExitButton->SetPosFromParentRight(-0.115f);
	topExitButton->SetPosFromParentTop(-0.002f);
	topExitButton->SetName("TopExitButton");
	topExitButton->SetOnClick([this]() { GoIngame(); });

	auto playButton = AddButton(playTextPath, mainMenu);
	auto& playButtonSprite = playButton->GetSprite();
	float playButtonAspectRatio = playButtonSprite.scale.y / playButtonSprite.scale.x;
	playButton->SetScale({ mainMenu->GetSprite().scale.x * 0.2f,
								mainMenu->GetSprite().scale.x * 0.2f  * playButtonAspectRatio});
	playButton->SetPosFromParentLeft(0.0f);
	playButton->SetPosFromParentTop(0.3f);
	playButton->SetName("PlayButton");
	playButton->SetOnClick([this]() { GoIngame(); });

	auto settingsButton = AddButton(settingsTextPath, mainMenu);
	auto& settingsButtonSprite = settingsButton->GetSprite();
	float settingsButtonAspectRatio = settingsButtonSprite.scale.y / settingsButtonSprite.scale.x;
	settingsButton->SetScale({	mainMenu->GetSprite().scale.x * 0.2f,
									mainMenu->GetSprite().scale.x * 0.2f * settingsButtonAspectRatio });
	settingsButton->SetPosFromParentLeft(0.0f);
	settingsButton->SetPosFromParentTop(0.35f);
	settingsButton->SetName("SettingsButton");
	settingsButton->SetOnClick([this]() { GoToSettingsMenu(); });

	auto levelSelectButton = AddButton(levelSelectTextPath, mainMenu);
	auto& levelSelectButtonSprite = levelSelectButton->GetSprite();
	float levelSelectButtonAspectRatio = levelSelectButtonSprite.scale.y / levelSelectButtonSprite.scale.x;
	levelSelectButton->SetScale({ mainMenu->GetSprite().scale.x * 0.2f,
									mainMenu->GetSprite().scale.x * 0.2f * levelSelectButtonAspectRatio });
	levelSelectButton->SetPosFromParentLeft(0.0f);
	levelSelectButton->SetPosFromParentTop(0.4f);
	levelSelectButton->SetOnClick([this]() { GoToLevelSelectMenu(); });
	levelSelectButton->SetName("LevelSelectButton");

	auto creditsButton = AddButton(creditsTextPath, mainMenu);
	auto& creditsButtonSprite = creditsButton->GetSprite();
	float creditsButtonAspectRatio = creditsButtonSprite.scale.y / creditsButtonSprite.scale.x;
	creditsButton->SetScale({ mainMenu->GetSprite().scale.x * 0.2f,
									mainMenu->GetSprite().scale.x * 0.2f * creditsButtonAspectRatio });
	creditsButton->SetPosFromParentLeft(0.0f);
	creditsButton->SetPosFromParentTop(0.45f);
	creditsButton->SetName("CreditsButton");
	creditsButton->SetOnClick([this]() { GoToCreditsMenu(); });

	auto exitButton = AddButton(exitTextSpritePath, mainMenu);
	auto& exitButtonSprite = exitButton->GetSprite();
	float exitButtonAspectRatio = exitButtonSprite.scale.y / exitButtonSprite.scale.x;
	exitButton->SetScale({ mainMenu->GetSprite().scale.x * 0.2f,
									mainMenu->GetSprite().scale.x * 0.2f * exitButtonAspectRatio });
	exitButton->SetPosFromParentLeft(0.0f);
	exitButton->SetPosFromParentTop(0.75f);
	exitButton->SetName("ExitButton");
	exitButton->SetOnClick([this]() { ExitGame(); });

	myStateID = eUIStateID::UIMain;
	myLetThroughRender = true;
}
void Root::UIMain::Update()
{
	UIMenuType::Update();

	if (Input::Pressed(InputAction::Exit))
	{
		ExitGame();
	}
}
void Root::UIMain::Render()
{
	UIMenuType::Render();
}
void Root::UIMain::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIMain state.", Log::MessageType::Notice);
}
void Root::UIMain::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIMain state.", Log::MessageType::Notice);
}
void Root::UIMain::GoIngame()
{
	if (GetUIManager().GetCurrentState()->GetStateID() == eUIStateID::UIMain && GetUIManager().PopState())
	{
		/*if (GetUIManager().GetUISound(eUISounds::MusicMain)->IsPlaying())
		{
			GetUIManager().PauseUISound(eUISounds::MusicMain, true);
		}
		GetUIManager().PlayUISound(eUISounds::Forward);*/
	}
}
void Root::UIMain::GoToSettingsMenu()
{
	GetUIManager().PushState(eUIStateID::UIOptions);
	//GetUIManager().PlayUISound(eUISounds::Select);
}
void Root::UIMain::GoToLevelSelectMenu()
{
	GetUIManager().PushState(eUIStateID::UILevelSelect);
	//GetUIManager().PlayUISound(eUISounds::Select);
}
void Root::UIMain::GoToCreditsMenu()
{
	GetUIManager().PushState(eUIStateID::UICredits);
	//GetUIManager().PlayUISound(eUISounds::Select);
}