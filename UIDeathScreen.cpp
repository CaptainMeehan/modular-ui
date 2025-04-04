#include "UIDeathScreen.h"

#include "UI/UIManager.h"
#include "UI/UIOptions.h"

void Root::UIDeathScreen::Init()
{
	SetShouldFreezeMovement(true);

	myStateID = eUIStateID::UIDeathScreen;
	myLetThroughRender = false;
}
void Root::UIDeathScreen::Update()
{
	UIMenuType::Update();
}
void Root::UIDeathScreen::Render()
{
	UIMenuType::Render();
}
void Root::UIDeathScreen::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIDeathScreen state.", Log::MessageType::Notice);
}
void Root::UIDeathScreen::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIDeathScreen state.", Log::MessageType::Notice);
}

void Root::UIDeathScreen::CloseMenu()
{
}
