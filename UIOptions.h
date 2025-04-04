#pragma once
#include "UIMenuType.hpp"

namespace Root
{
	enum class eResolutionType
	{
		eResolutionType_1920x1080,
		eResolutionType_1600x900,
		eResolutionType_1280x720,
		eResolutionType_854x480,
		eResolutionType_Count
	};
	class UIOptions : public UIMenuType

	{
	public:
		UIOptions() = default;
		virtual ~UIOptions() = default;

		void Init() override;
		void Update() override;
		void Render() override;
		void OnEnter() override;
		void OnExit() override;
		void CloseMenu() override;
		void SetResolution(eResolutionType aResType);
		void ToggleSoundMute(const int& aSoundChannel);
		void SetChannelVolume(const int& aSoundChannel, const float& aVolume);
		void ToggleFullscreen();

	private:
		std::shared_ptr<UI_Button> myMuteMainButton;
		std::shared_ptr<UI_Container> myUnmuteMainContainer;
		std::shared_ptr<UI_Button> myMuteSFXButton;
		std::shared_ptr<UI_Container> myUnmuteSFXContainer;
		std::shared_ptr<UI_Button> myMuteMusicButton;
		std::shared_ptr<UI_Container> myUnmuteMusicContainer;
		std::shared_ptr<UI_Button> myFullscreenButtonOn;
		std::shared_ptr<UI_Container> myFullscreenOffContainer;
	};
}
