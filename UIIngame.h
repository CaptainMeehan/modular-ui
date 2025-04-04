#pragma once
#include "UIMenuType.hpp"

namespace Root
{
    class UIIngame : public UIMenuType
    {
    public:
        UIIngame() = default;
        virtual ~UIIngame() = default;

        void Init() override;
        void Update() override;
        void Render() override;
        void OnEnter() override;
        void OnExit() override;
        void GoToPauseMenu();
        void GoToLevel(int aLevel);

    private:
        SpriteInstance2D* myVignetteSprite;
        SpriteInstance2D* myAmmoBar;
        SpriteInstance2D* myBulletAmmo1;
        SpriteInstance2D* myBulletAmmo2;
        SpriteInstance2D* myBulletAmmo3;
        SpriteInstance2D* myAmmoCursor;
        SpriteInstance2D* myClock;
        SpriteInstance2D* myCredits;
        SpriteInstance2D* myCreditsBackground;
        float myCreditsFadeTimer;
        float myCreditsBackgroundFadeTimer;
        bool myCreditsFadeIn;
        bool myCreditsBackgroundFadeIn;
    };
}

class UITimer : public Singleton<UITimer>
{
    float myCurrentTime = 0;
    bool myTimerActive = false;

    friend UIIngame;

public:
    void SetTimerActive(bool aState);
    void ResetTimer();
    float GetTime();
};
