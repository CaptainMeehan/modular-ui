#include "UI/UICredits.h"

#include "UIManager.h"
#include "Graphics/GraphicsEngine.h"
#include "Engine/Input/Input.h"

void Root::UICredits::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string backdropSpritePath = "../ArtAssets/Materials/CRTScreen/T_Background_C.dds";
	std::string creditsSpritePath = "../ArtAssets/UI/UI/Credits/T_CreditsMenu_C.dds";
	std::string backTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_BackButton_C.dds";

	auto backdrop = AddContainer(backdropSpritePath);
	backdrop->SetScale({ resolution.x, resolution.y });
	backdrop->SetName("BackdropContainer");
	backdrop->GetSprite().color = { 0.0f, 0.0f, 0.0f, 0.0f };

	auto creditsMenu = AddContainer(creditsSpritePath, backdrop);
	creditsMenu->SetName("CreditsMenuContainer");
	creditsMenu->SetScale({ creditsMenu->GetSprite().texture->GetResolution().x, creditsMenu->GetSprite().texture->GetResolution().x });

	auto topExitButton = AddButton(backTextSpritePath, creditsMenu);
	auto& topExitButtonSprite = topExitButton->GetSprite();
	topExitButtonSprite.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	topExitButton->SetScale({ creditsMenu->GetSprite().scale.x * 0.02f,
									creditsMenu->GetSprite().scale.x * 0.02f });
	topExitButton->SetPosFromParentRight(-0.115f);
	topExitButton->SetPosFromParentTop(-0.002f);
	topExitButton->SetName("TopExitButton");
	topExitButton->SetOnClick([this]() { CloseMenu(); });

	auto backButton = AddButton(backTextSpritePath, creditsMenu);
	auto& backButtonSprite = backButton->GetSprite();
	float backButtonAspectRatio = backButtonSprite.scale.y / backButtonSprite.scale.x;
	backButton->SetScale({ creditsMenu->GetSprite().scale.x * 0.2f,
									creditsMenu->GetSprite().scale.x * 0.2f * backButtonAspectRatio });
	backButton->SetPosFromParentLeft(0.0f);
	backButton->SetPosFromParentTop(0.86f);
	backButton->SetName("ExitButton");
	backButton->SetOnClick([this]() { CloseMenu(); });

	myStateID = eUIStateID::UICredits;
	myLetThroughRender = true;
}
void Root::UICredits::Update()
{
	UIMenuType::Update();

	if (Input::Pressed(InputAction::Exit))
	{
		CloseMenu();
	}
}
void Root::UICredits::Render()
{
	UIMenuType::Render();
}
void Root::UICredits::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UICredits state.", Log::MessageType::Notice);
	GetUIManager().PlayUISound(eUISounds::MusicCredits);
}
void Root::UICredits::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UICredits state.", Log::MessageType::Notice);
	GetUIManager().PauseUISound(eUISounds::MusicCredits, true);
}
void Root::UICredits::CloseMenu()
{
	GetUIManager().PopState();
	//GetUIManager().PlayUISound(eUISounds::Back);
}
