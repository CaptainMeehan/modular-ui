#include "UI/UIExit.h"

#include "UIManager.h"

void Root::UIExit::Init()
{
	SetShouldFreezeMovement(true);

	myStateID = eUIStateID::UIExit;
	myLetThroughRender = false;
}
void Root::UIExit::Update()
{
	UIMenuType::Update();
}
void Root::UIExit::Render()
{
	UIMenuType::Render();
}
void Root::UIExit::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIExit state.", Log::MessageType::Notice);
}
void Root::UIExit::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIExit state.", Log::MessageType::Notice);
}
