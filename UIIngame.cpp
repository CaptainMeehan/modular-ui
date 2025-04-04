#include "UI/UIIngame.h"
#include "Engine/Core/Scene.h"
#include "Engine.h"
#include "UIManager.h"
#include "Core/SceneManager.h"
#include "Game/Player.h"
#include "World/Components/HealthComponent.hpp"
#include "World/Components/PlayerComponent.hpp"

#define CLOCK_OFF 0.0125f

void Root::UIIngame::Init()
{
	SetShouldFreezeMovement(false);

	myWindowSize = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string vignetteAlbedoSpritePath = "../ArtAssets/UI/UIVFX/T_ScreenDmg_C.dds";
	std::string ammoBarSpritePath = "../ArtAssets/UI/UI/HUD/T_AmmoBar_C.dds";
	std::string bulletAmmoSpritePath = "../ArtAssets/UI/UI/HUD/T_BulletAmmo_C.dds";
	std::string creditsSpritePath = "../ArtAssets/UI/UI/T_EndScreen_C.dds";
	std::string creditsBackgroundSpritePath = "../ArtAssets/UI/UI/T_EndscreenBlack_C.dds";

	auto vignette = AddContainer(vignetteAlbedoSpritePath);
	myVignetteSprite = &vignette->GetSprite();
	//JPH::Float2 scale = myVignetteSprite->scale;
	//float aspectRatio = scale.x / scale.y;
	vignette->SetScale({ myWindowSize.x, myWindowSize.y });
	vignette->SetName("BackdropContainer");
	vignette->GetSprite().pixelShaderToUSe = Root::Engine::Get().GetGraphicsEngine().GetSpriteRenderer()->AddPixelShader("ScreenDmg_PS.cso");
	vignette->GetSprite().blendState = Additive;

	/*auto ammoBar = AddContainer(ammoBarSpritePath);
	myAmmoBar = &ammoBar->GetSprite();
	ammoBar->SetPosFromParentLeft(0.15f);
	ammoBar->SetPosFromParentBottom(0.1f);

	auto bulletAmmo1 = AddContainer(bulletAmmoSpritePath, ammoBar);
	myBulletAmmo1 = &bulletAmmo1->GetSprite();
	myBulletAmmo1->blendState = Alpha;
	bulletAmmo1->SetPosFromParentLeft(-0.438f);
	bulletAmmo1->SetPosFromParentBottom(-0.273f);
	auto bulletAmmo2 = AddContainer(bulletAmmoSpritePath, ammoBar);
	myBulletAmmo2 = &bulletAmmo2->GetSprite();
	myBulletAmmo2->blendState = Alpha;
	bulletAmmo2->SetPosFromParentLeft(-0.368f);
	bulletAmmo2->SetPosFromParentBottom(-0.273f);

	auto bulletAmmo3 = AddContainer(bulletAmmoSpritePath, ammoBar);
	myBulletAmmo3 = &bulletAmmo3->GetSprite();
	myBulletAmmo3->blendState = Alpha;
	bulletAmmo3->SetPosFromParentLeft(-0.298f);
	bulletAmmo3->SetPosFromParentBottom(-0.273f);*/

	// Ammo Cursor
	{
		auto ammoCursor = AddContainer("../Assets/WhiteSquare.dds");
		myAmmoCursor = &ammoCursor->GetSprite();
		myAmmoCursor->pixelShaderToUSe = Root::Engine::Get().GetGraphicsEngine().GetSpriteRenderer()->AddPixelShader("../Bin/Shaders/SpriteCursor_PS.cso", false);
		myAmmoCursor->blendState = BlendState::Alpha;
		myAmmoCursor->color = { 0.f, 0.8f, 0.f, 0.35f };
		//myAmmoCursor->color.w = 0.25f;
		ammoCursor->SetScale(JPH::Float2(myWindowSize.x * 0.025f, myWindowSize.x * 0.025f));
		vignette->SetPivot({ 0.5f, 0.5f });
		vignette->SetScreenPositionInPercent({ 0.5f, 0.5f });
	}

	// Timer
	{
		auto clock = AddContainer("../ArtAssets/Materials/ClockShader/T_ClockShader_C.dds");
		myClock = &clock->GetSprite();

		SpriteManager::AddTexture(myClock, "../ArtAssets/Materials/ClockShader/T_ClockShader_M.dds");
		SpriteManager::AddTexture(myClock, "../ArtAssets/Materials/ClockShader/T_ClockShader_FX.dds");

		myClock->pixelShaderToUSe = Root::Engine::Get().GetGraphicsEngine().GetSpriteRenderer()->AddPixelShader("../Bin/Shaders/ClockShaderSprite_PS.cso", false);

		clock->SetScale(JPH::Float2(myWindowSize.x * 0.1f, myWindowSize.x * 0.03f));
		clock->SetPivot({ 0.00f, 1.00f });
		clock->SetScreenPositionInPercent({ 0.0f + CLOCK_OFF * (9.00f / 16.f), 1.0f - CLOCK_OFF });
	}

	// End Credits
	{
		auto creditsBackground = AddContainer(creditsBackgroundSpritePath);
		myCreditsBackground = &creditsBackground->GetSprite();
		creditsBackground->SetScale({ myWindowSize.x, myWindowSize.y });
		creditsBackground->SetName("CreditsBackgroundContainer");
		//creditsBackground->GetSprite().pixelShaderToUSe = Root::Engine::Get().GetGraphicsEngine().GetSpriteRenderer()->AddPixelShader("CRTScreen_PS.cso");
		creditsBackground->GetSprite().blendState = Alpha;

		auto credits = AddContainer(creditsSpritePath, creditsBackground);
		myCredits = &credits->GetSprite();
		//JPH::Float2 scale = myVignetteSprite->scale;
		//float aspectRatio = scale.x / scale.y;
		credits->SetScale({ myWindowSize.x * 0.9f, myWindowSize.y * 0.9f });
		credits->SetName("CreditsContainer");
		//credits->GetSprite().pixelShaderToUSe = Root::Engine::Get().GetGraphicsEngine().GetSpriteRenderer()->AddPixelShader("CRTScreen_PS.cso");
		credits->GetSprite().blendState = Alpha;
		credits->GetSprite().color.w = 0.0f;
	}

	// Initialize fade-in variables
	myCreditsFadeTimer = 0.0f;
	myCreditsBackgroundFadeTimer = 0.0f;
	myCreditsFadeIn = false;
	myCreditsBackgroundFadeIn = false;

	myStateID = eUIStateID::UIIngame;
	myLetThroughRender = true;
}
void Root::UIIngame::Update()
{
	UIMenuType::Update();

	auto& w = Root::Engine::Get().GetWorld();
	auto player = w.TQuery<PlayerComponent>();
	auto healthComp = player.GetComponent<HealthComponent>();
	auto gunComp = player.GetComponent<GunComponent>();

	if (UITimer::Get().myTimerActive == true)
		UITimer::Get().myCurrentTime += TinTime::deltaTime;

	myAmmoCursor->other = (gunComp->hasGun == true) ? gunComp->ammo : 0.0f;
	myClock->other = UITimer::Get().myCurrentTime;

	if (healthComp)
	{
        myVignetteSprite->other = 1.0f - healthComp->currentHealth / healthComp->maxHealth;
	}
	/*if (gunComp && gunComp->hasGun)
	{
		if(gunComp->ammo > 0)
		{
			myBulletAmmo1->color.w = 1.0f;
		}
		else
		{
			myBulletAmmo1->color.w = 0.0f;
		}
		if(gunComp->ammo > 1)
		{
			myBulletAmmo2->color.w = 1.0f;
		}
		else
		{
			myBulletAmmo2->color.w = 0.0f;
		}
		if(gunComp->ammo > 2)
		{
			myBulletAmmo3->color.w = 1.0f;
		}
		else
		{
			myBulletAmmo3->color.w = 0.0f;
		}
	}
	else
	{
		myAmmoBar->color.w = 0.0f;
		myBulletAmmo1->color.w = 0.0f;
		myBulletAmmo2->color.w = 0.0f;
		myBulletAmmo3->color.w = 0.0f;
	}*/


	// Fade-in logic for credits
	if (Engine::Get().GetSceneManager().GetCurrentScene()->GetShowCredits())
	{
		if (!myCreditsFadeIn)
		{
			myCreditsFadeTimer += TinTime::deltaTime;
			if (myCreditsFadeTimer <= 0.7f)
			{
				myCredits->color.w = myCreditsFadeTimer;
			}
			else
			{
				myCredits->color.w = 0.7f;
				myCreditsFadeIn = true;
			}
		}
		else if (!myCreditsBackgroundFadeIn)
		{
			myCreditsBackgroundFadeTimer += TinTime::deltaTime;
			if (myCreditsBackgroundFadeTimer <= 1.0f)
			{
				myCreditsBackground->color.w = myCreditsBackgroundFadeTimer;
			}
			else
			{
				myCreditsBackground->color.w = 1.0f;
				myCreditsBackgroundFadeIn = true;
			}
		}
		else if (myCreditsBackgroundFadeIn && myCreditsFadeIn)
		{
			myCreditsBackgroundFadeTimer += TinTime::deltaTime;
			if (myCreditsBackgroundFadeTimer >= 20.0f)
			{
				myCredits->color.w = 0.0f;
				myCreditsBackground->color.w = 0.0f;
				myCreditsFadeTimer = 0.0f;
				myCreditsBackgroundFadeTimer = 0.0f;
				myCreditsFadeIn = false;
				myCreditsBackgroundFadeIn = false;
				Engine::Get().GetSceneManager().GetCurrentScene()->SetShowCredits(false);
				GoToLevel(1);
			}
		}
	}
	else
	{
		myCredits->color.w = 0.0f;
		myCreditsBackground->color.w = 0.0f;
		myCreditsFadeTimer = 0.0f;
		myCreditsBackgroundFadeTimer = 0.0f;
		myCreditsFadeIn = false;
		myCreditsBackgroundFadeIn = false;
	}

	/*if (myCredits)
	{
		if (myShowCredits)
		{
			myCredits->color.w = 1.0f;
		}
		else
		{
			myCredits->color.w = 0.0f;
		}
	}*/
	if (Input::Pressed(InputAction::Exit))
	{
		GoToPauseMenu();
	}
}
void Root::UIIngame::Render()
{
	UIMenuType::Render();
}
void Root::UIIngame::OnEnter()
{
	UIMenuType::OnEnter();
	Input::GetInputManager()->EnableMouseCapture();
	PMT("Entering UIIngame state.", Log::MessageType::Notice);
	auto& scene = *Engine::Get().GetSceneManager().GetCurrentScene();
	scene.TurnOnSounds();
}
void Root::UIIngame::OnExit()
{
	UIMenuType::OnExit();
	Input::GetInputManager()->DisableMouseCapture();
	PMT("Exiting UIIngame state.", Log::MessageType::Notice);
	Engine::Get().GetSceneManager().GetCurrentScene()->ShutOffSounds();
}
void Root::UIIngame::GoToPauseMenu()
{
	GetUIManager().PushState(eUIStateID::UIPause);
	//GetUIManager().PlayUISound(eUISounds::Select);
}

void Root::UIIngame::GoToLevel(const int aLevel)
{
	auto& engine = Engine::Get();
	auto& sceneManager = engine.GetSceneManager();
	auto sceneName = "Lvl_" + std::to_string(aLevel);

	PMT("Loading level " + aLevel, Log::MessageType::Notice);
	sceneManager.SetNextScene(sceneName, true, true);
	GetUIManager().PopState(); // HACK: This is a hack to get the level select to close after selecting a level
	GetUIManager().PopState();
}

void UITimer::SetTimerActive(bool aState)
{
	myTimerActive = aState;
}

void UITimer::ResetTimer()
{
	myCurrentTime = 0;
}

float UITimer::GetTime()
{
	return myCurrentTime;
}
