#pragma once
#include "UIMenuType.hpp"

namespace Root
{
    class UIIntro : public UIMenuType
    {
    public:
        UIIntro() = default;
        virtual ~UIIntro() = default;

        void Init() override;
        void Update() override;
        void Render() override;
        void OnEnter() override;
        void OnExit() override;
        void CloseMenu() override;
        void ResetIntro();

    private:
        SpriteInstance2D* myIntroBackgroundSprite;
        SpriteInstance2D* myTGALogoSprite;
        SpriteInstance2D* myFMODLogoSprite;
        SpriteInstance2D* myBullensLogoSprite;
        SpriteInstance2D* myGameLogoSprite;
        bool myTGALogoFadeIn;
        bool myTGALogoFadeOut;
        bool myFMODLogoFadeIn;
        bool myFMODLogoFadeOut;
        bool myBullensLogoFadeIn;
        bool myBullensLogoFadeOut;
        bool myGameLogoFadeIn;
        bool myGameLogoFadeOut;
        float myTGALogoFadeTimer;
        float myFMODLogoFadeTimer;
        float myBullensLogoFadeTimer;
        float myGameLogoFadeTimer;
    };
}