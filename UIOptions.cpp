#include "UI/UIOptions.h"
#include "UIManager.h"
#include "Audio/AudioManager.h"
#include "Core/Window.h"
#include "FMOD/fmod_studio.hpp"

void Root::UIOptions::Init()
{
	SetShouldFreezeMovement(true);

	JPH::Float2 resolution = Engine::Get().GetGraphicsEngine().GetResolution();

	std::string backdropSpritePath = "../ArtAssets/Materials/CRTScreen/T_Background_C.dds";
	std::string menuPath = "../ArtAssets/UI/UI/T_Background_C.dds";
	std::string res1TextPath = "../ArtAssets/UI/UI/Settings/T_Res1920X1080Button_C.dds";
	std::string res2TextPath = "../ArtAssets/UI/UI/Settings/T_Res1280X720Button_C.dds";
	std::string res3TextPath = "../ArtAssets/UI/UI/Settings/T_Res854X480Button_C.dds";
	std::string backTextSpritePath = "../ArtAssets/UI/UI/MainMenu/T_BackButton_C.dds";
	std::string soundONSpritePath = "../ArtAssets/UI/UI/Settings/T_SoundONButton_C.dds";
	std::string soundOFFSpritePath = "../ArtAssets/UI/UI/Settings/T_SoundOFFButton_C.dds";
	std::string soundSFXONSpritePath = "../ArtAssets/UI/UI/Settings/T_SoundSFXONButton_C.dds";
	std::string soundSFXOFFSpritePath = "../ArtAssets/UI/UI/Settings/T_SoundSFXOFFButton_C.dds";
	std::string musicONSpritePath = "../ArtAssets/UI/UI/Settings/T_MusicONButton_C.dds";
	std::string musicOFFSpritePath = "../ArtAssets/UI/UI/Settings/T_MusicOFFButton_C.dds";
	std::string fullscreenONSpritePath = "../ArtAssets/UI/UI/Settings/T_FullscreenON_C.dds";
	std::string fullscreenOFFSpritePath = "../ArtAssets/UI/UI/Settings/T_FullscreenOFF_C.dds";

	auto backdrop = AddContainer(backdropSpritePath);
	backdrop->SetScale({ resolution.x, resolution.y });
	backdrop->SetName("BackdropContainer");
	backdrop->GetSprite().color = { 0.0f, 0.0f, 0.0f, 0.0f };

	auto optionsMenu = AddContainer(menuPath, backdrop);
	optionsMenu->SetName("MenuContainer");
	optionsMenu->SetScale({ optionsMenu->GetSprite().texture->GetResolution().x, optionsMenu->GetSprite().texture->GetResolution().x });

	auto topExitButton = AddButton(backTextSpritePath, optionsMenu);
	auto& topExitButtonSprite = topExitButton->GetSprite();
	topExitButtonSprite.color = { 0.0f, 0.0f, 0.0f, 0.0f };
	topExitButton->SetScale({ optionsMenu->GetSprite().scale.x * 0.02f,
									optionsMenu->GetSprite().scale.x * 0.02f });
	topExitButton->SetPosFromParentRight(-0.115f);
	topExitButton->SetPosFromParentTop(-0.002f);
	topExitButton->SetName("TopExitButton");
	topExitButton->SetOnClick([this]() { CloseMenu(); });

	auto res1Button = AddButton(res1TextPath, optionsMenu);
	auto& res1ButtonSprite = res1Button->GetSprite();
	float res1ButtonAspectRatio = res1ButtonSprite.scale.y / res1ButtonSprite.scale.x;
	res1Button->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
								optionsMenu->GetSprite().scale.x * 0.2f * res1ButtonAspectRatio });
	res1Button->SetPosFromParentLeft(0.0f);
	res1Button->SetPosFromParentTop(0.3f);
	res1Button->SetName("Res1Button");
	res1Button->SetOnClick([this]() { SetResolution(eResolutionType::eResolutionType_1920x1080);  });

	auto res2Button = AddButton(res2TextPath, optionsMenu);
	auto& res2ButtonSprite = res2Button->GetSprite();
	float res2ButtonAspectRatio = res2ButtonSprite.scale.y / res2ButtonSprite.scale.x;
	res2Button->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * res2ButtonAspectRatio });
	res2Button->SetPosFromParentLeft(0.0f);
	res2Button->SetPosFromParentTop(0.35f);
	res2Button->SetName("Res2Button");
	res2Button->SetOnClick([this]() { SetResolution(eResolutionType::eResolutionType_1280x720);  });

	auto res3Button = AddButton(res3TextPath, optionsMenu);
	auto& res3ButtonSprite = res3Button->GetSprite();
	float res3ButtonAspectRatio = res3ButtonSprite.scale.y / res3ButtonSprite.scale.x;
	res3Button->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * res3ButtonAspectRatio });
	res3Button->SetPosFromParentLeft(0.0f);
	res3Button->SetPosFromParentTop(0.4f);
	res3Button->SetName("Res3Button");
	res3Button->SetOnClick([this]() { SetResolution(eResolutionType::eResolutionType_854x480);  });

	myMuteMainButton = AddButton(soundONSpritePath, optionsMenu);
	auto& muteMainButtonSprite = myMuteMainButton->GetSprite();
	float muteMainButtonAspectRatio = muteMainButtonSprite.scale.y / muteMainButtonSprite.scale.x;
	myMuteMainButton->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteMainButtonAspectRatio });
	myMuteMainButton->SetPosFromParentLeft(0.0f);
	myMuteMainButton->SetPosFromParentTop(0.45f);
	myMuteMainButton->SetName("MuteMainButton");
	myMuteMainButton->SetOnClick([this]() { ToggleSoundMute(0); });

	myUnmuteMainContainer = AddContainer(soundOFFSpritePath, optionsMenu);
	auto& unmuteMainContainerSprite = myUnmuteMainContainer->GetSprite();
	myUnmuteMainContainer->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteMainButtonAspectRatio });
	myUnmuteMainContainer->SetPosFromParentLeft(0.0f);
	myUnmuteMainContainer->SetPosFromParentTop(0.45f);
	myUnmuteMainContainer->SetName("UnmuteMainContainer");
	unmuteMainContainerSprite.color.w = 0.0f;

	auto mainVolumeSlider = AddSlider(optionsMenu, "");
	mainVolumeSlider->SetPosFromParentLeft(0.1f);
	mainVolumeSlider->SetPosFromParentTop(0.5f);
	mainVolumeSlider->SetName("MainVolumeSlider");
	mainVolumeSlider->SetOnValueChanged([this](float aValue) { SetChannelVolume(0, aValue); });

	myMuteSFXButton = AddButton(soundSFXONSpritePath, optionsMenu);
	auto& muteSFXButtonSprite = myMuteSFXButton->GetSprite();
	float muteSFXButtonAspectRatio = muteSFXButtonSprite.scale.y / muteSFXButtonSprite.scale.x;
	myMuteSFXButton->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteSFXButtonAspectRatio });
	myMuteSFXButton->SetPosFromParentLeft(0.0f);
	myMuteSFXButton->SetPosFromParentTop(0.55f);
	myMuteSFXButton->SetName("MuteSFXButton");
	myMuteSFXButton->SetOnClick([this]() { ToggleSoundMute(1); });

	myUnmuteSFXContainer = AddContainer(soundSFXOFFSpritePath, optionsMenu);
	auto& unmuteSFXContainerSprite = myUnmuteSFXContainer->GetSprite();
	myUnmuteSFXContainer->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteSFXButtonAspectRatio });
	myUnmuteSFXContainer->SetPosFromParentLeft(0.0f);
	myUnmuteSFXContainer->SetPosFromParentTop(0.55f);
	myUnmuteSFXContainer->SetName("UnmuteSFXContainer");
	unmuteSFXContainerSprite.color.w = 0.0f;

	auto sfxVolumeSlider = AddSlider(optionsMenu, "");
	sfxVolumeSlider->SetPosFromParentLeft(0.1f);
	sfxVolumeSlider->SetPosFromParentTop(0.6f);
	sfxVolumeSlider->SetName("SFXVolumeSlider");
	sfxVolumeSlider->SetOnValueChanged([this](float aValue) { SetChannelVolume(1, aValue); });

	myMuteMusicButton = AddButton(musicONSpritePath, optionsMenu);
	auto& muteMusicButtonSprite = myMuteMusicButton->GetSprite();
	float muteMusicButtonAspectRatio = muteMusicButtonSprite.scale.y / muteMusicButtonSprite.scale.x;
	myMuteMusicButton->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteMusicButtonAspectRatio });
	myMuteMusicButton->SetPosFromParentLeft(0.0f);
	myMuteMusicButton->SetPosFromParentTop(0.65f);
	myMuteMusicButton->SetName("MuteMusicButton");
	myMuteMusicButton->SetOnClick([this]() { ToggleSoundMute(2); });

	myUnmuteMusicContainer = AddContainer(musicOFFSpritePath, optionsMenu);
	auto& unmuteMusicContainerSprite = myUnmuteMusicContainer->GetSprite();
	myUnmuteMusicContainer->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * muteMusicButtonAspectRatio });
	myUnmuteMusicContainer->SetPosFromParentLeft(0.0f);
	myUnmuteMusicContainer->SetPosFromParentTop(0.65f);
	myUnmuteMusicContainer->SetName("UnmuteMusicContainer");
	unmuteMusicContainerSprite.color.w = 0.0f;

	auto musicVolumeSlider = AddSlider(optionsMenu, "");
	musicVolumeSlider->SetPosFromParentLeft(0.1f);
	musicVolumeSlider->SetPosFromParentTop(0.7f);
	musicVolumeSlider->SetName("MusicVolumeSlider");
	musicVolumeSlider->SetOnValueChanged([this](float aValue) { SetChannelVolume(2, aValue); });

	myFullscreenButtonOn = AddButton(fullscreenONSpritePath, optionsMenu);
	auto& fullscreenButtonSprite = myFullscreenButtonOn->GetSprite();
	float fullscreenButtonAspectRatio = fullscreenButtonSprite.scale.y / fullscreenButtonSprite.scale.x;
	myFullscreenButtonOn->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * fullscreenButtonAspectRatio });
	myFullscreenButtonOn->SetPosFromParentLeft(0.0f);
	myFullscreenButtonOn->SetPosFromParentTop(0.75f);
	myFullscreenButtonOn->SetName("FullscreenButton");
	myFullscreenButtonOn->SetOnClick([this]() {ToggleFullscreen(); });

	myFullscreenOffContainer = AddContainer(fullscreenOFFSpritePath, optionsMenu);
	auto& fullscreenOffContainerSprite = myFullscreenOffContainer->GetSprite();
	myFullscreenOffContainer->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * fullscreenButtonAspectRatio });
	myFullscreenOffContainer->SetPosFromParentLeft(0.0f);
	myFullscreenOffContainer->SetPosFromParentTop(0.75f);
	myFullscreenOffContainer->SetName("FullscreenOffContainer");
	fullscreenOffContainerSprite.color.w = 0.0f;

	auto backButton = AddButton(backTextSpritePath, optionsMenu);
	auto& backButtonSprite = backButton->GetSprite();
	float backButtonAspectRatio = backButtonSprite.scale.y / backButtonSprite.scale.x;
	backButton->SetScale({ optionsMenu->GetSprite().scale.x * 0.2f,
									optionsMenu->GetSprite().scale.x * 0.2f * backButtonAspectRatio });
	backButton->SetPosFromParentLeft(0.0f);
	backButton->SetPosFromParentTop(0.85f);
	backButton->SetName("BackButton");
	backButton->SetOnClick([this]() {  CloseMenu(); });

	myStateID = eUIStateID::UIOptions;
	myLetThroughRender = false;
}
void Root::UIOptions::Update()
{
	UIMenuType::Update();

	if (Input::Pressed(InputAction::Exit))
	{
		CloseMenu();
	}
}
void Root::UIOptions::Render()
{
	UIMenuType::Render();
}
void Root::UIOptions::OnEnter()
{
	UIMenuType::OnEnter();
	PMT("Entering UIOptions state.", Log::MessageType::Notice);
}
void Root::UIOptions::OnExit()
{
	UIMenuType::OnExit();
	PMT("Exiting UIOptions state.", Log::MessageType::Notice);
}

void Root::UIOptions::CloseMenu()
{
	if (GetUIManager().GetCurrentStateID() == eUIStateID::UIOptions && GetUIManager().PopState())
	{
		//GetUIManager().PlayUISound(eUISounds::Back);
	}
}
void Root::UIOptions::SetResolution(eResolutionType aResType)
{
	switch (aResType)
	{
	case eResolutionType::eResolutionType_1920x1080:
		Root::Engine::Get().SetResolution(1920, 1080);
		break;
	case eResolutionType::eResolutionType_1280x720:
		Root::Engine::Get().SetResolution(1280, 720);
		break;
	case eResolutionType::eResolutionType_854x480:
		Root::Engine::Get().SetResolution(854, 480);
		break;
	default:
		break;
	}
}
void Root::UIOptions::ToggleSoundMute(const int& aSoundChannel)
{
	switch (aSoundChannel)
	{
		case 0:
		{
			if (myUnmuteMainContainer->GetSprite().color.w < 0.01f)
			{
				myMuteMainButton->GetSprite().color.w = 0.0f;
				myUnmuteMainContainer->GetSprite().color.w = 1.0f;
			}
			else
			{
				myMuteMainButton->GetSprite().color.w = 1.0f;
				myUnmuteMainContainer->GetSprite().color.w = 0.0f;
			}
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/");
			bool current;
			bus->getMute(&current);
			bus->setMute(!current);
			break;
		}
		case 1:
		{
			if (myUnmuteSFXContainer->GetSprite().color.w < 0.01f)
			{
				myMuteSFXButton->GetSprite().color.w = 0.0f;
				myUnmuteSFXContainer->GetSprite().color.w = 1.0f;
			}
			else
			{
				myMuteSFXButton->GetSprite().color.w = 1.0f;
				myUnmuteSFXContainer->GetSprite().color.w = 0.0f;
			}
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/SFX");
			bool current;
			bus->getMute(&current);
			bus->setMute(!current);
			break;
		}
		case 2:
		{
			if (myUnmuteMusicContainer->GetSprite().color.w < 0.01f)
			{
				myMuteMusicButton->GetSprite().color.w = 0.0f;
				myUnmuteMusicContainer->GetSprite().color.w = 1.0f;
			}
			else
			{
				myMuteMusicButton->GetSprite().color.w = 1.0f;
				myUnmuteMusicContainer->GetSprite().color.w = 0.0f;
			}
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/Music");
			bool current;
			bus->getMute(&current);
			bus->setMute(!current);
			break;
		}
	}
}
void Root::UIOptions::SetChannelVolume(const int& aSoundChannel, const float& aVolume)
{
	aSoundChannel;
	aVolume;
	switch (aSoundChannel)
	{
		case 0:
		{
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/");
			bus->setVolume(aVolume * 2);
			break;
		}
		case 1:
		{
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/SFX");
			bus->setVolume(aVolume * 2);
			break;
		}
		case 2:
		{
			FMOD::Studio::Bus* bus = Root::AudioManager::Get().GetAudioBus("bus:/Music");
			bus->setVolume(aVolume * 2);
			break;
		}
	}
}

void Root::UIOptions::ToggleFullscreen()
{
	auto& window = Window::Get();

	if (window.GetWindowSettings().style == WindowStyle::WindowFullscreen)
	{
		myFullscreenButtonOn->GetSprite().color.w = 0.0f;
		myFullscreenOffContainer->GetSprite().color.w = 1.0f;
		window.ChangeWindowStyle(WindowStyle::Windowed);
	}
	else
	{
		myFullscreenButtonOn->GetSprite().color.w = 1.0f;
		myFullscreenOffContainer->GetSprite().color.w = 0.0f;
		window.ChangeWindowStyle(WindowStyle::WindowFullscreen);
	}
}
