#include "UI/UIIntro.h"
#include "UIManager.h"
#include "Core/SceneManager.h"

void Root::UIIntro::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();
	float aThirdOfScreenWidth = resolution.x * 0.33f;
	float halfOfScreenWidth = resolution.x * 0.5f;

	std::string introBackgroundSpritePath = "../ArtAssets/UI/UI/T_EndscreenBlack_C.dds";
	std::string introTGALogoSpritePath = "../ArtAssets/UI/UI/T_tgalogo_C.dds";
	std::string introFMODLogoSpritePath = "../ArtAssets/UI/UI/T_fmodlogo_C.dds";
	std::string introBullensLogoSpritePath = "../ArtAssets/UI/UI/T_bullenslogo_C.dds";
	std::string introGameLogoSpritePath = "../ArtAssets/UI/UI/T_GameIntro_C.dds";

	auto introBackground = AddContainer(introBackgroundSpritePath);
	myIntroBackgroundSprite = &introBackground->GetSprite();
	myIntroBackgroundSprite->blendState = Alpha;
	introBackground->SetScale({ resolution.x, resolution.y });
	introBackground->SetName("IntroBackgroundContainer");

	auto introTGALogo = AddContainer(introTGALogoSpritePath, introBackground);
	myTGALogoSprite = &introTGALogo->GetSprite();
	myTGALogoSprite->blendState = Alpha;
	float yRatioToX = myTGALogoSprite->scale.y / myTGALogoSprite->scale.x;
	introTGALogo->SetScale({ aThirdOfScreenWidth, yRatioToX * aThirdOfScreenWidth });
	introTGALogo->SetName("IntroTGALogoContainer");
	introTGALogo->GetSprite().color.w = 0.0f;

	auto introFMODLogo = AddContainer(introFMODLogoSpritePath, introBackground);
	myFMODLogoSprite = &introFMODLogo->GetSprite();
	myFMODLogoSprite->blendState = Alpha;
	yRatioToX = myFMODLogoSprite->scale.y / myFMODLogoSprite->scale.x;
	introFMODLogo->SetScale({ aThirdOfScreenWidth, yRatioToX * aThirdOfScreenWidth });
	introFMODLogo->SetName("IntroFMODLogoContainer");
	introFMODLogo->GetSprite().color.w = 0.0f;

	auto introBullensLogo = AddContainer(introBullensLogoSpritePath, introBackground);
	myBullensLogoSprite = &introBullensLogo->GetSprite();
	myBullensLogoSprite->blendState = Alpha;
	yRatioToX = myBullensLogoSprite->scale.y / myBullensLogoSprite->scale.x;
	introBullensLogo->SetScale({ halfOfScreenWidth, yRatioToX * halfOfScreenWidth });
	introBullensLogo->SetName("IntroBullensLogoContainer");
	introBullensLogo->GetSprite().color.w = 0.0f;

	auto introGameLogo = AddContainer(introGameLogoSpritePath, introBackground);
	myGameLogoSprite = &introGameLogo->GetSprite();
	myGameLogoSprite->blendState = Alpha;
	yRatioToX = myGameLogoSprite->scale.y / myGameLogoSprite->scale.x;
	introGameLogo->SetScale({ halfOfScreenWidth, yRatioToX * halfOfScreenWidth });
	introGameLogo->SetName("IntroGameLogoContainer");
	introGameLogo->GetSprite().color.w = 0.0f;

	myStateID = eUIStateID::UIIntro;
	myLetThroughRender = true;
}
void Root::UIIntro::Update()
{
	UIMenuType::Update();

	if (Input::Pressed(InputAction::Exit))
	{
		CloseMenu();
	}

	if (!myTGALogoFadeOut)
	{
		if (!myTGALogoFadeIn)
		{
			myTGALogoFadeTimer += TinTime::unscaledDeltaTime;

			if (myTGALogoFadeTimer <= 0.7f)
			{
				myTGALogoSprite->color.w = myTGALogoFadeTimer;
			}
			else if (myTGALogoFadeTimer > 4.0f)
			{
				myTGALogoSprite->color.w = 0.7f;
				myTGALogoFadeIn = true;
			}
		}
		else
		{
			myTGALogoFadeTimer -= TinTime::unscaledDeltaTime;

			if (myTGALogoFadeTimer >= 0.0f)
			{
				myTGALogoSprite->color.w = myTGALogoFadeTimer;
			}
			else
			{
				myTGALogoSprite->color.w = 0.0f;
				myTGALogoFadeOut = true;
			}
		}
	}
	else if (!myFMODLogoFadeOut)
	{
		if (!myFMODLogoFadeIn)
		{
			myFMODLogoFadeTimer += TinTime::unscaledDeltaTime;

			if (myFMODLogoFadeTimer <= 0.7f)
			{
				myFMODLogoSprite->color.w = myFMODLogoFadeTimer;
			}
			else if (myFMODLogoFadeTimer > 4.0f)
			{
				myFMODLogoSprite->color.w = 0.7f;
				myFMODLogoFadeIn = true;
			}
		}
		else
		{
			myFMODLogoFadeTimer -= TinTime::unscaledDeltaTime;

			if (myFMODLogoFadeTimer >= 0.0f)
			{
				myFMODLogoSprite->color.w = myFMODLogoFadeTimer;
			}
			else
			{
				myFMODLogoSprite->color.w = 0.0f;
				myFMODLogoFadeOut = true;
			}
		}
	}
	else if (!myBullensLogoFadeOut)
	{
		if (!myBullensLogoFadeIn)
		{
			myBullensLogoFadeTimer += TinTime::unscaledDeltaTime;
			if (myBullensLogoFadeTimer <= 0.7f)
			{
				myBullensLogoSprite->color.w = myBullensLogoFadeTimer;
			}
			else if (myBullensLogoFadeTimer > 4.0f)
			{
				myBullensLogoSprite->color.w = 0.7f;
				myBullensLogoFadeIn = true;
			}
		}
		else
		{
			myBullensLogoFadeTimer -= TinTime::unscaledDeltaTime;
			if (myBullensLogoFadeTimer >= 0.0f)
			{
				myBullensLogoSprite->color.w = myBullensLogoFadeTimer;
			}
			else
			{
				myBullensLogoSprite->color.w = 0.0f;
				myBullensLogoFadeOut = true;
			}
		}
	}
	else if (!myGameLogoFadeOut)
	{
		if (!myGameLogoFadeIn)
		{
			myGameLogoFadeTimer += TinTime::unscaledDeltaTime;
			if (myGameLogoFadeTimer <= 0.7f)
			{
				myGameLogoSprite->color.w = myGameLogoFadeTimer;
			}
			else if (myGameLogoFadeTimer > 4.0f)
			{
				myGameLogoSprite->color.w = 0.7f;
				myGameLogoFadeIn = true;
			}
		}
		else
		{
			myGameLogoFadeTimer -= TinTime::unscaledDeltaTime;
			if (myGameLogoFadeTimer >= 0.0f)
			{
				myGameLogoSprite->color.w = myGameLogoFadeTimer;
			}
			else
			{
				myGameLogoSprite->color.w = 0.0f;
				myGameLogoFadeOut = true;
			}
		}
	}
	else
	{
		ResetIntro();
		CloseMenu();
	}
}
void Root::UIIntro::Render()
{
	UIMenuType::Render();
}
void Root::UIIntro::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIIntro state.", Log::MessageType::Notice);
}
void Root::UIIntro::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIIntro state.", Log::MessageType::Notice);
}
void Root::UIIntro::ResetIntro()
{
	myTGALogoSprite->color.w = 0.0f;
	myFMODLogoSprite->color.w = 0.0f;
	myBullensLogoSprite->color.w = 0.0f;
	myTGALogoFadeIn = false;
	myTGALogoFadeOut = false;
	myFMODLogoFadeIn = false;
	myFMODLogoFadeOut = false;
	myBullensLogoFadeIn = false;
	myBullensLogoFadeOut = false;
	myTGALogoFadeTimer = 0.0f;
	myFMODLogoFadeTimer = 0.0f;
	myBullensLogoFadeTimer = 0.0f;
}
void Root::UIIntro::CloseMenu()
{
	Engine::Get().GetUIManager().PopState();
}