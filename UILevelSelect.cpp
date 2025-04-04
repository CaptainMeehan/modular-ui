#include "UI/UILevelSelect.h"

#include "UIManager.h"
#include "Core/SceneManager.h"
#include "Engine/Input/Input.h"

void Root::UILevelSelect::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string backdropSpritePath = "../ArtAssets/Materials/CRTScreen/T_Background_C.dds";
	std::string menuPath = "../ArtAssets/UI/UI/T_Background_C.dds";
	std::string level1TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL1Button_C.dds";
	std::string level2TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL2Button_C.dds";
	std::string level3TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL3Button_C.dds";
	std::string level4TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL4Button_C.dds";
	std::string level5TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL5Button_C.dds";
	std::string level5p5TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVLToiletButton_C.dds";
	std::string level6TextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL6Button_C.dds";
	std::string levelPGymTextPath = "../ArtAssets/UI/UI/LevelSelect/T_PlayerGymButton_C.dds";
	std::string levelAGymTextPath = "../ArtAssets/UI/UI/LevelSelect/T_LVL9Button_C.dds";
	std::string backTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_BackButton_C.dds";

	auto backdrop = AddContainer(backdropSpritePath);
	backdrop->SetScale({ resolution.x, resolution.y });
	backdrop->SetName("BackdropContainer");
	backdrop->GetSprite().color = { 0.0f, 0.0f, 0.0f, 0.0f };

	auto levelSelectMenu = AddContainer(menuPath, backdrop);
	levelSelectMenu->SetName("LevelSelectMenuContainer");
	levelSelectMenu->SetScale({ levelSelectMenu->GetSprite().texture->GetResolution().x, levelSelectMenu->GetSprite().texture->GetResolution().x });

	auto topExitButton = AddButton(backTextSpritePath, levelSelectMenu);
	auto& topExitButtonSprite = topExitButton->GetSprite();
	topExitButtonSprite.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	topExitButton->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.02f,
									levelSelectMenu->GetSprite().scale.x * 0.02f });
	topExitButton->SetPosFromParentRight(-0.115f);
	topExitButton->SetPosFromParentTop(-0.002f);
	topExitButton->SetName("TopExitButton");
	topExitButton->SetOnClick([this]() { CloseMenu(); });

	auto level1Button = AddButton(level1TextPath, levelSelectMenu);
	auto& level1ButtonSprite = level1Button->GetSprite();
	float level1ButtonAspectRatio = level1ButtonSprite.scale.y / level1ButtonSprite.scale.x;
	level1Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level1ButtonAspectRatio });
	level1Button->SetPosFromParentLeft(0.0f);
	level1Button->SetPosFromParentTop(0.3f);
	level1Button->SetName("Level1Button");
	level1Button->SetOnClick([this]() { GoToLevel(1); });

	auto level2Button = AddButton(level2TextPath, levelSelectMenu);
	auto& level2ButtonSprite = level2Button->GetSprite();
	float level2ButtonAspectRatio = level2ButtonSprite.scale.y / level2ButtonSprite.scale.x;
	level2Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level2ButtonAspectRatio });
	level2Button->SetPosFromParentLeft(0.0f);
	level2Button->SetPosFromParentTop(0.35f);
	level2Button->SetName("Level2Button");
	level2Button->SetOnClick([this]() { GoToLevel(2); });

	auto level3Button = AddButton(level3TextPath, levelSelectMenu);
	auto& level3ButtonSprite = level3Button->GetSprite();
	float level3ButtonAspectRatio = level3ButtonSprite.scale.y / level3ButtonSprite.scale.x;
	level3Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level3ButtonAspectRatio });
	level3Button->SetPosFromParentLeft(0.0f);
	level3Button->SetPosFromParentTop(0.4f);
	level3Button->SetName("Level3Button");
	level3Button->SetOnClick([this]() { GoToLevel(3); });

	auto level4Button = AddButton(level4TextPath, levelSelectMenu);
	auto& level4ButtonSprite = level4Button->GetSprite();
	float level4ButtonAspectRatio = level4ButtonSprite.scale.y / level4ButtonSprite.scale.x;
	level4Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level4ButtonAspectRatio });
	level4Button->SetPosFromParentLeft(0.0f);
	level4Button->SetPosFromParentTop(0.45f);
	level4Button->SetName("Level4Button");
	level4Button->SetOnClick([this]() { GoToLevel(4); });

	auto level5Button = AddButton(level5TextPath, levelSelectMenu);
	auto& level5ButtonSprite = level5Button->GetSprite();
	float level5ButtonAspectRatio = level5ButtonSprite.scale.y / level5ButtonSprite.scale.x;
	level5Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level5ButtonAspectRatio });
	level5Button->SetPosFromParentLeft(0.0f);
	level5Button->SetPosFromParentTop(0.5f);
	level5Button->SetName("Level5Button");
	level5Button->SetOnClick([this]() { GoToLevel(5); });

	auto level5p5Button = AddButton(level5p5TextPath, levelSelectMenu);
	auto& level5p5ButtonSprite = level5p5Button->GetSprite();
	float level5p5ButtonAspectRatio = level5p5ButtonSprite.scale.y / level5p5ButtonSprite.scale.x;
	level5p5Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level5p5ButtonAspectRatio });
	level5p5Button->SetPosFromParentLeft(0.1f);
	level5p5Button->SetPosFromParentTop(0.55f);
	level5p5Button->SetName("Level5p5Button");
	level5p5Button->SetOnClick([this]() { GoToLevel(7); });

	auto level6Button = AddButton(level6TextPath, levelSelectMenu);
	auto& level6ButtonSprite = level6Button->GetSprite();
	float level6ButtonAspectRatio = level6ButtonSprite.scale.y / level6ButtonSprite.scale.x;
	level6Button->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * level6ButtonAspectRatio });
	level6Button->SetPosFromParentLeft(0.0f);
	level6Button->SetPosFromParentTop(0.6f);
	level6Button->SetName("Level6Button");
	level6Button->SetOnClick([this]() { GoToLevel(6); });

	auto levelPGymButton = AddButton(levelPGymTextPath, levelSelectMenu);
	auto& levelPGymButtonSprite = levelPGymButton->GetSprite();
	float levelPGymButtonAspectRatio = levelPGymButtonSprite.scale.y / levelPGymButtonSprite.scale.x;
	levelPGymButton->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * levelPGymButtonAspectRatio });
	levelPGymButton->SetPosFromParentLeft(0.1f);
	levelPGymButton->SetPosFromParentTop(0.65f);
	levelPGymButton->SetName("LevelPGymButton");
	levelPGymButton->SetOnClick([this]() { GoToLevel(8); });

	auto levelAGymButton = AddButton(levelAGymTextPath, levelSelectMenu);
	auto& levelAGymButtonSprite = levelAGymButton->GetSprite();
	float levelAGymButtonAspectRatio = levelAGymButtonSprite.scale.y / levelAGymButtonSprite.scale.x;
	levelAGymButton->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
					levelSelectMenu->GetSprite().scale.x * 0.2f * levelAGymButtonAspectRatio });
	levelAGymButton->SetPosFromParentLeft(0.1f);
	levelAGymButton->SetPosFromParentTop(0.7f);
	levelAGymButton->SetName("LevelAGymButton");
	levelAGymButton->SetOnClick([this]() { GoToLevel(9); });

	auto backButton = AddButton(backTextSpritePath, levelSelectMenu);
	auto& backButtonSprite = backButton->GetSprite();
	float backButtonAspectRatio = backButtonSprite.scale.y / backButtonSprite.scale.x;
	backButton->SetScale({ levelSelectMenu->GetSprite().scale.x * 0.2f,
									levelSelectMenu->GetSprite().scale.x * 0.2f * backButtonAspectRatio });
	backButton->SetPosFromParentLeft(0.0f);
	backButton->SetPosFromParentTop(0.75f);
	backButton->SetName("BackButton");
	backButton->SetOnClick([this]() {  CloseMenu(); });

	myStateID = eUIStateID::UILevelSelect;
	myLetThroughRender = true;
}
void Root::UILevelSelect::Update()
{
	UIMenuType::Update();

	if (Input::Pressed(InputAction::Exit))
	{
		CloseMenu();
	}
}
void Root::UILevelSelect::Render()
{
	UIMenuType::Render();
}
void Root::UILevelSelect::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UILevelSelect state.", Log::MessageType::Notice);
}
void Root::UILevelSelect::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UILevelSelect state.", Log::MessageType::Notice);
}
void Root::UILevelSelect::CloseMenu()
{
	if (GetUIManager().GetCurrentStateID() == eUIStateID::UILevelSelect && GetUIManager().PopState())
	{
		//GetUIManager().PlayUISound(eUISounds::Back);
	}
}
void Root::UILevelSelect::GoToLevel(const int aLevel)
{
	auto& engine = Engine::Get();
	auto& sceneManager = engine.GetSceneManager();
	if (aLevel < 6 || aLevel == 7)
	{
		sceneManager.SetNextScene("Lvl_" + std::to_string(aLevel), true, true);
	}
	else if (aLevel == 8)
	{
		sceneManager.SetNextScene("PlayerGym", true, true);
	}
	else if (aLevel == 9)
	{
		sceneManager.SetNextScene("AssetGym", true, true);
	}
	//auto sceneName = "Lvl_" + std::to_string(aLevel);

	PMT("Loading level " + aLevel, Log::MessageType::Notice);
	//sceneManager.SetNextScene(sceneName,true,true);
	GetUIManager().PopState(); // HACK: This is a hack to get the level select to close after selecting a level
	GetUIManager().PopState();
}
