#pragma once
#include <vector>
#include <memory>

#include "UIContainer.hpp"
#include "UIButton.hpp"
#include "UICheckbox.hpp"
#include "UISlider.hpp"
#include "UIText.hpp"
#include "Graphics/2D/SpriteRenderer.h"

struct SpriteInstance2D;

namespace Root
{
    static constexpr size_t maxText = 10;
    static constexpr size_t maxButtons = 15;
    static constexpr size_t maxSliders = 10;
    static constexpr size_t maxCheckboxes = 10;
    static constexpr size_t maxContainers = 15;

    template <typename T>
    void InitializeElements(std::vector<std::shared_ptr<T>>& aContainer, size_t aCount)
    {
		aContainer.reserve(aCount);
    }

    class UIMenuType
    {
    public:
        UIMenuType() : myWindowSize()
        {
            myWindowSize = Engine::Get().GetGraphicsEngine().GetResolution();
            UIMenuType::Init();
        }

        virtual ~UIMenuType() = default;

        virtual void Init()
        {
            InitializeElements(myContainers, maxContainers);
            InitializeElements(myButtons, maxButtons);
            InitializeElements(myTexts, maxText);
            InitializeElements(mySliders, maxSliders);
            InitializeElements(myCheckboxes, maxCheckboxes);
        }

        virtual void OnEnter() {}
        virtual void OnExit() {}

        virtual void Update()
        {
            ClearRenderList();
            PopulateRenderList();
            for (auto& element : myAllUIElements)
            {
                element->Update();
            }
        }

        void UpdateUIElementsForResolution(const JPH::Float2& aNewResolution)
        {
			myWindowSize = aNewResolution;
            for (auto& element : myAllUIElements)
            {
				element->UpdateSizeAndPositionAfterResize();
            }
            for (auto& button : myButtons)
            {
                button->UpdateHoverEffectPosition();
            }
        }

        virtual void Render()
        {
            auto& gEngine = Engine::Get().GetGraphicsEngine();
            for (auto& element : myAllUIElements)
            {
                if (&element->GetSprite())
                {
                    gEngine.GetSpriteRenderer()->AddToRender(&element->GetSprite());
                }
            }
            for (auto& button : myButtons)
            {
                if (button->GetIsHighlighted())
                {
                    gEngine.GetSpriteRenderer()->AddToRender(&button->GetHighlightSprite());
                }
            }
        }

        virtual void ClearRenderList() { myAllUIElements.clear(); }

        virtual void PopulateRenderList()
        {
            for (auto& text : myTexts)
            {
                if (text->GetIsVisible())
                {
                    myAllUIElements.push_back(text);
                }
            }

            for (auto& button : myButtons)
            {
                if (button->GetIsVisible())
                {
                    myAllUIElements.push_back(button);
                }
            }

            for (auto& slider : mySliders)
            {
                if (slider->GetIsVisible())
                {
                    myAllUIElements.push_back(slider);
                }
            }

            for (auto& checkbox : myCheckboxes)
            {
                if (checkbox->GetIsVisible())
                {
                    myAllUIElements.push_back(checkbox);
                }
            }

            for (auto& container : myContainers)
            {
                if (container->GetIsVisible())
                {
                    myAllUIElements.push_back(container);
                }
            }

            std::sort(myAllUIElements.begin(), myAllUIElements.end(),
                [](const std::shared_ptr<UI::UI_Element>& a, const std::shared_ptr<UI::UI_Element>& b)
                {
                    return a->GetRenderPriority() < b->GetRenderPriority();
                });
        }

        virtual void ExitState() { myIsExited = true; }
        virtual void ExitGame()
        {
            Engine::Get().StopRunning();
            PostQuitMessage(WM_QUIT);
            Root::Engine::Get().GetWorld().Quit();
        }

        virtual void CloseMenu() {}

        template <typename T>
        std::shared_ptr<T> AddElement(std::vector<std::shared_ptr<T>>& aContainer, const std::string& aSpritePath, std::shared_ptr<UI::UI_Element> aParent = nullptr)
        {
            auto newElement = std::make_shared<T>();

            auto sprite = SpriteManager::CreateSprite(aSpritePath);
            if (!sprite || !sprite->texture)
            {
                throw std::runtime_error("Failed to create or load sprite: " + aSpritePath);
            }

            sprite->pivot = { 0.5f, 0.5f };
            sprite->scale = sprite->texture->GetResolution();
            sprite->blendState = Alpha;

            newElement->SetSprite(std::make_shared<SpriteInstance2D>(*sprite));

            if (aParent)
            {
                aParent->AddChildElement(newElement.get());
            }

            newElement->Init();
            newElement->SetScale(sprite->texture->GetResolution());

            aContainer.push_back(newElement);
            return newElement;
        }
        std::shared_ptr<UI_Button> AddButton(const std::string& aSpritePath, std::shared_ptr<UI::UI_Element> aParent = nullptr)
        {
            return AddElement(myButtons, aSpritePath, aParent);
        }
        std::shared_ptr<UI_Container> AddContainer(const std::string& aSpritePath, std::shared_ptr<UI::UI_Element> aParent = nullptr)
        {
            return AddElement(myContainers, aSpritePath, aParent);
        }
		std::shared_ptr<UI_Slider> AddSlider(std::shared_ptr<UI::UI_Element> aParent = nullptr, const std::string& aSpritePath = "")
		{
            if (aSpritePath == "")
            {
				JPH::Float2 parentSize;

                // Load and initialize the background bar sprite
				auto sliderBar = AddElement(mySliders, "../ArtAssets/UI/UI/Settings/T_SliderBackground_C.dds", aParent);
                auto& sliderBarSprite = sliderBar->GetSprite();
				auto& sliderBarSize = sliderBarSprite.texture->GetResolution();
                float initialSizeRatioSlider = sliderBarSize.x / sliderBarSize.y;

                if (sliderBar->GetParent())
                {
                    auto& parentSprite = sliderBar->GetParent()->GetSprite();
                    parentSize = parentSprite.scale;
                }
                else
                {
                    parentSize = Engine::Get().GetGraphicsEngine().GetResMinusBorders();
                }
				float portionOfParentSizeX = parentSize.x * 0.4f;
				float portionOfParentSizeY = portionOfParentSizeX / initialSizeRatioSlider;
				sliderBar->SetScale({ portionOfParentSizeX, portionOfParentSizeY });
                sliderBarSprite.blendState = Alpha;
                sliderBarSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };

                // Load and initialize the slider handle sprite
                auto sliderHandle = AddButton("../ArtAssets/UI/UI/Settings/T_SliderHandle_C.dds", sliderBar);
                sliderBar->SetHandle(sliderHandle);
                sliderHandle->SetOnClickHeld([sliderBar]() { sliderBar->UpdateHandlePosition(); });
                sliderHandle->SetScale({ portionOfParentSizeY, portionOfParentSizeY });
                sliderHandle->SetScreenPositionInPercent(sliderBarSprite.pos);
                sliderHandle->GetSprite().blendState = Alpha;
                sliderHandle->GetSprite().color = { 1.0f, 1.0f, 1.0f, 0.8f };

				return sliderBar;
            }
			// Not really handled
            return AddElement(mySliders, aSpritePath, aParent);
		}

        virtual bool CheckIfResizeUI(unsigned int aWidth, unsigned int aHeight)
        {
			aWidth;
			aHeight;
            if (myAllUIElements.empty())
            {
                return false;
            }

            for (auto& element : myAllUIElements)
            {
                element->UpdateSizeAndPositionAfterResize();
            }
            for (auto& button : myButtons)
            {
                button->UpdateHoverEffectPosition();
            }
            return true;
        }

        virtual UIManager& GetUIManager() { return Engine::Get().GetUIManager(); }
        virtual const bool& GetIsExited() const { return myIsExited; }
        virtual const bool& GetIsTransparent() const { return myLetThroughRender; }
        virtual const eUIStateID GetStateID() const { return myStateID; }
        virtual const bool& GetShouldFreezeMovement() const { return myShouldFreezeMovement; }
		virtual const JPH::Float2& GetWindowSize() const { return myWindowSize; }
		virtual const std::vector<std::shared_ptr<UI_Text>>& GetTexts() const { return myTexts; }
		virtual const std::vector<std::shared_ptr<UI_Button>>& GetButtons() const { return myButtons; }
		virtual const std::vector<std::shared_ptr<UI_Slider>>& GetSliders() const { return mySliders; }
		virtual const std::vector<std::shared_ptr<UI_Checkbox>>& GetCheckboxes() const { return myCheckboxes; }
		virtual const std::vector<std::shared_ptr<UI_Container>>& GetContainers() const { return myContainers; }
		virtual const std::vector<std::shared_ptr<UI::UI_Element>>& GetAllUIElements() const { return myAllUIElements; }


        virtual void SetStateID(const eUIStateID aStateID) { myStateID = aStateID; }
        virtual void SetShouldFreezeMovement(const bool aBool) { myShouldFreezeMovement = aBool; }
		virtual void SetIsTransparent(const bool aBool) { myLetThroughRender = aBool; }
		virtual void SetIsExited(const bool aBool) { myIsExited = aBool; }

    protected:
        eUIStateID myStateID = {};
        bool myIsExited = false;
        bool myLetThroughRender = false;
        bool myShouldFreezeMovement = false;

        std::vector<std::shared_ptr<UI_Text>> myTexts = {};
        std::vector<std::shared_ptr<UI_Button>> myButtons = {};
        std::vector<std::shared_ptr<UI_Slider>> mySliders = {};
        std::vector<std::shared_ptr<UI_Checkbox>> myCheckboxes = {};
        std::vector<std::shared_ptr<UI_Container>> myContainers = {};
        std::vector<std::shared_ptr<UI::UI_Element>> myAllUIElements;

        JPH::Float2 myWindowSize;
    };
}
