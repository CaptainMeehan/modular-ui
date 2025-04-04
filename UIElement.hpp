#pragma once
#include "Engine.h"
#include "eUISounds.hpp"
#include "UIManager.h"
#include "UIQuad.hpp"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/2D/SpriteInstance.hpp"
#include "Graphics/2D/SpriteManager.h"
#include "Graphics/2D/SpriteRenderer.h"
#include "Input/Input.h"
#include "Tools/MessagePrinter.h"


class UI_Button;

namespace UI
{
	class UI_Element
	{
	public:
		virtual ~UI_Element() = default;

		UI_Element() : onClickCallback(nullptr), isHovered(false), myID(++nextID), myRenderPriority(0), myIsRelative(false), myIsVisible(false), myIsAvailable(true),
				myParent(nullptr), myParentBoxInPercent(), myParentBoxInPixels(), myPositionPercent(),
				myPositionPixels(), mySizePercent(), mySizePixels(), mySpriteInstance(nullptr)
		{
			buttonHoverEffect = SpriteManager::CreateSprite("../ArtAssets/Materials/ButtonSelect/T_ButtonSelect_C.dds");
			JPH::Float2 textureResolution = buttonHoverEffect->texture->GetResolution();
			buttonHoverEffect->scale =  { textureResolution.x * 2.0f, textureResolution.y * 2.0f };
			//buttonHoverEffect->pixelShaderToUSe = 1;
			buttonHoverEffect->blendState = Alpha;
			buttonHoverEffect->color.x = 0.0f;
			buttonHoverEffect->color.y = 1.0f;
			buttonHoverEffect->color.z = 0.0f;
			buttonHoverEffect->color.w = 0.7f;

			SetOnHover([this]() { PlayHighlightEffect(true); });
		}

		virtual void Init()
		{
			ContainWithinParentByX();
			UpdateParentBoundaries();

			UpdateOriginalScaleInfo();
		}
		virtual void Update() {}
		virtual void OnClick() {}
		virtual void OnClickHeld() {}
		virtual void OnHover() {}
		virtual void OnHoverExit() {}

		bool IsMouseOver()
		{
			if (!mySpriteInstance)
			{
				return false;
			}

			Engine& engine = Engine::Get();
			// Get the raw mouse position
			JPH::Float2 mousePos = engine.GetGraphicsEngine().GetScreenPercentPos();
			JPH::Float2 currentRes = engine.GetGraphicsEngine().GetResMinusBorders();
			JPH::Float2 mousePosPixels = {mousePos.x * currentRes.x,mousePos.y * currentRes.y };

			// Convert sprite position from normalized (0-1) to pixel space
			JPH::Float2 spritePosPixels =
			{
				mySpriteInstance->pos.x * currentRes.x, // X: normalized -> pixels
				mySpriteInstance->pos.y * currentRes.y  // Y: normalized -> pixels
			};

			// Sprite scale is already in pixels
			JPH::Float2 spriteScalePixels = mySpriteInstance->scale;

			// Pivot
			JPH::Float2 spritePivotPercent = mySpriteInstance->pivot;

			// Calculate top-left corner from the pivot
			JPH::Float2 topLeft;
			topLeft.x = spritePosPixels.x - (spritePivotPercent.x * spriteScalePixels.x);
			topLeft.y = spritePosPixels.y - (spritePivotPercent.y * spriteScalePixels.y);

			// Calculate bottom-right corner
			JPH::Float2 bottomRight;
			bottomRight.x = topLeft.x + spriteScalePixels.x;
			bottomRight.y = topLeft.y + spriteScalePixels.y;

			// Check if the mouse is within the sprite boundaries
			return (mousePosPixels.x >= topLeft.x && mousePosPixels.x <= bottomRight.x &&
				mousePosPixels.y >= topLeft.y && mousePosPixels.y <= bottomRight.y);
		}

	    void PlayHighlightEffect(bool aBool)
	    {
	        if (buttonHoverEffect && aBool)
	        {
	            GetUIManager().PlayUISound(eUISounds::Hover);
	            UpdateHoverEffectPosition();
	            buttonHoverEffect->other = 1.0f;
	        }
	        else
	        {
	            buttonHoverEffect->other = 0.0f;
	        }
	    }
		void RenderButtonHighlight(GraphicsEngine& aGraphicsEngine)
		{
			aGraphicsEngine.GetSpriteRenderer()->AddToRender(buttonHoverEffect);
		}
		void AddChildElement(UI_Element* aChild)
		{
			aChild->SetParent(this);
			aChild->SetIsRelative(true);
			aChild->SetRenderPriority(this->GetRenderPriority() + 1);
			children.push_back(aChild);
		}

		void UpdateSizeAndPositionAfterResize()
		{
			JPH::Float2 newWindowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();
			JPH::Float2 currentScale = {mySpriteInstance->scale.x, mySpriteInstance->scale.y};

			// Calculate the new size based on the original size and the new window size
			mySpriteInstance->scale.x = (currentScale.x / myLastWindowSize.x) * newWindowSize.x;
			mySpriteInstance->scale.y = (currentScale.y / myLastWindowSize.y) * newWindowSize.y;

			UpdateHoverEffectPosition();

			myLastWindowSize = newWindowSize;
		}
		void UpdateHoverEffectPosition()
		{
			if (!buttonHoverEffect || &this->GetSprite() == nullptr)
			{
				return;
			}
			buttonHoverEffect->scale = {this->GetSprite().scale.x * 2.0f, this->GetSprite().scale.y * 2.0f};
			buttonHoverEffect->pos = this->GetSprite().pos;
			buttonHoverEffect->pivot = this->GetSprite().pivot;
		}
		void UpdateParentBoundaries()
		{
			if (myParent && myIsRelative)
			{
				JPH::Float2 parentContainerSizePixels = myParent->GetSizeInPixels();
				JPH::Float2 parentContainerSizePercent = myParent->GetSizeInPercent();
				myParentBoxInPixels.SetTopLeftPos({0.0f, parentContainerSizePixels.y,0.0f});
				myParentBoxInPixels.SetTopRightPos({ parentContainerSizePixels.x, parentContainerSizePixels.y,0.0f});
				myParentBoxInPixels.SetBottomRightPos({ parentContainerSizePixels.x,0.0f,0.0f});
				myParentBoxInPixels.SetBottomLeftPos({0.0f,0.0f,0.0f});

				myParentBoxInPercent.SetTopLeftPos({0.0f, parentContainerSizePercent.y,0.0f});
				myParentBoxInPercent.SetTopRightPos({ parentContainerSizePercent.x, parentContainerSizePercent.y,0.0f});
				myParentBoxInPercent.SetBottomRightPos({ parentContainerSizePercent.x,0.0f,0.0f});
				myParentBoxInPercent.SetBottomLeftPos({0.0f,0.0f,0.0f});
			}
			else
			{
				JPH::Float2 windowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();

				myParentBoxInPixels.vertices = {
				UI_Vertex{ JPH::Float3(0.0f,  windowSize.y, 0.0f), JPH::Float2(0.0f, 0.0f) }, // Top-left
				UI_Vertex{ JPH::Float3(windowSize.x,  windowSize.y, 0.0f), JPH::Float2(1.0f, 0.0f) }, // Top-right
				UI_Vertex{ JPH::Float3(windowSize.x, 0.0f, 0.0f), JPH::Float2(1.0f, 1.0f) }, // Bottom-right
				UI_Vertex{ JPH::Float3(0.0f, 0.0f, 0.0f), JPH::Float2(0.0f, 1.0f) }, // Bottom-left
				};
				myParentBoxInPercent.vertices = {
				UI_Vertex{ JPH::Float3(0.0f,  1.0f, 0.0f), JPH::Float2(0.0f, 0.0f) }, // Top-left
				UI_Vertex{ JPH::Float3(1.0f, 1.0f, 0.0f), JPH::Float2(1.0f, 0.0f) }, // Top-right
				UI_Vertex{ JPH::Float3(1.0f, 0.0f, 0.0f), JPH::Float2(1.0f, 1.0f) }, // Bottom-right
				UI_Vertex{ JPH::Float3(0.0f, 0.0f, 0.0f), JPH::Float2(0.0f, 1.0f) }, // Bottom-left
				};
			}
		}
		void UpdatePositionForScale()
		{
			JPH::Float2 windowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();
			myPositionPixels.x = (windowSize.x - mySizePixels.x) * 0.5f;
			myPositionPixels.y = (windowSize.y - mySizePixels.y) * 0.5f;
		}
		void UpdateOriginalScaleInfo()
		{
			myOriginalWindowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();
			myLastWindowSize = myOriginalWindowSize;
			myOriginalScalePixels = mySpriteInstance->scale;
			myOriginalAspectRatio = { mySpriteInstance->scale.x / mySpriteInstance->scale.y};
		}
		void UpdateSpritePositionAndSize()
		{
			if (mySpriteInstance)
			{
				mySpriteInstance->scale = mySizePixels; // Set size in pixels based on Y-axis scaling
				mySpriteInstance->pos = myPositionPixels; // Centered position in pixels
			}
		}

		void ContainWithinParentByX()
		{
			if (mySpriteInstance)
			{
				JPH::Float2 parentSize = myParent ? myParent->GetSizeInPixels() : Root::Engine::Get().GetGraphicsEngine().GetResolution();
				float spriteXYRatio = mySpriteInstance->scale.x / mySpriteInstance->scale.y;
				float parentChildRatioX = mySpriteInstance->scale.x / parentSize.x;
				float parentChildRatioY = mySpriteInstance->scale.y / parentSize.y;
				if (parentChildRatioX > 1.0f)
				{
					mySpriteInstance->scale.x = parentSize.x;
					mySpriteInstance->scale.y = mySpriteInstance->scale.x * spriteXYRatio;
				}

				SetSizeInPixels({ mySpriteInstance->scale.x, mySpriteInstance->scale.y });
				SetSizeInPercent({parentChildRatioX, parentChildRatioY});

				JPH::Float2 percentPos = myParent ? myParent->GetPositionInPercent() : JPH::Float2{ 0.5f,0.5f };
				SetScreenPositionInPercent(percentPos);
				myIsVisible = true;
			}
		}
		void ContainWithinParentByY()
		{
			
		}

		bool GetIsHighlighted()
		{
			return (buttonHoverEffect->other > 0.001f);
		}
		SpriteInstance2D& GetHighlightSprite()
		{
			return *buttonHoverEffect;
		}
		Root::UIManager& GetUIManager() { return Root::Engine::Get().GetUIManager(); }
		SpriteInstance2D& GetSprite()
		{
			if (!mySpriteInstance)
			{
				throw std::runtime_error("SpriteInstance2D is not initialized.");
			}
			return *mySpriteInstance;
		}
		[[nodiscard]] const UI_Quad& GetParentBoundaries() const { return myParentBoxInPercent; }
		[[nodiscard]] std::string GetName() const { return myName; }
		[[nodiscard]] UI_Element* GetParent() const { return myParent; }
		[[nodiscard]] int GetID() const { return myID; }
		[[nodiscard]] int GetRenderPriority() const { return myRenderPriority; }
		[[nodiscard]] int GetIsVisible() const { return myIsVisible; }
		[[nodiscard]] bool GetIsAvailable() const { return myIsAvailable; }
		[[nodiscard]] bool GetIsRelative() const { return myIsRelative; }
		JPH::Float2 GetPivotInPercent() const { return mySpriteInstance ? mySpriteInstance->pivot : JPH::Float2(0.5f, 0.5f); }
		//JPH::Float2 GetPivotInPixels() const { return mySpriteInstance ? mySpriteInstance->pivot : JPH::Float2(0.5f, 0.5f); }
		JPH::Float2 GetPositionInPercent() const { return mySpriteInstance ? mySpriteInstance->pos : myPositionPercent; }
		//JPH::Float2 GetPositionInPixels() const { return mySpriteInstance ? mySpriteInstance->pos : myPositionPixels; }
		JPH::Float2 GetSizeInPercent() const
		{
			JPH::Float2 windowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();
			return {mySpriteInstance->scale.x / windowSize.x, mySpriteInstance->scale.y / windowSize.y};
		}
		JPH::Float2 GetSizeInPixels() const { return mySizePixels; }
		[[nodiscard]] std::vector<UI_Element*> GetChildren() const { return children; }
		[[nodiscard]] bool GetHasChildren() const { return !children.empty(); }

		void SetScale(JPH::Float2 aScale)
		{
			mySpriteInstance->scale = aScale;
			UpdateOriginalScaleInfo();
		}
		void SetName(const std::string& aName)
		{
			myName = aName;
		}
		void SetParent(UI_Element* aParent)
		{
			myParent = aParent;
		}
		void SetRenderPriority(const int& aPriority)
		{
			myRenderPriority = aPriority;
		}
		void SetIsVisible(bool aBool)
		{
			myIsVisible = aBool;
		}
		void SetIsAvailable(bool aBool)
		{
			myIsAvailable = aBool;
		}
		void SetIsRelative(const bool& aIsRelative)
		{
			myIsRelative = aIsRelative;
		}
		void SetSprite(std::shared_ptr<SpriteInstance2D> aSpriteInstance)
		{
			mySpriteInstance = aSpriteInstance;
		}
		void SetParentBoundaries(const UI_Quad& aQuad)
		{
			myParentBoxInPercent = aQuad;
		}

		virtual void SetScreenPositionInPercent(const JPH::Float2& aPosition)
		{
			myPositionPercent = aPosition;

			if(mySpriteInstance)
			{
				mySpriteInstance->pos = aPosition;
			}
		}
		void SetScreenPositionInPixels(const JPH::Float2& aPosition)
		{
			myPositionPixels = aPosition;

			if (mySpriteInstance)// TODO : fix this
			{
				JPH::Float2 windowSize = Root::Engine::Get().GetGraphicsEngine().GetResolution();
				myPositionPercent.x = aPosition.x / windowSize.x;
				myPositionPercent.y = aPosition.y / windowSize.y;
				mySpriteInstance->pos = myPositionPercent;
			}
		} 
		void SetSizeInPercent(const JPH::Float2& aSize)
		{
			mySizePercent = aSize;
		}
		void SetSizeInPixels(const JPH::Float2& aSize)
		{
			mySizePixels = aSize;
		}
		void SetPivot(JPH::Float2 aPivot)
		{
			if (mySpriteInstance)
			{
				mySpriteInstance->pivot = aPivot;
			}
		}
		void SetPosFromParentBottom(float anPercentOffset = 0.0f)
		{
			if (myParent)
			{
				JPH::Float2 parentSize = myParent->GetSizeInPercent();
				JPH::Float2 parentPos = myParent->GetPositionInPercent();

				// Parent bottom edge = parent's center - half height
				SetScreenPositionInPercent({
					myPositionPercent.x,
					parentPos.y - (parentSize.y * 0.5f) + (mySizePercent.y * 0.5f) + anPercentOffset
					});
			}
			else
			{
				SetScreenPositionInPercent({ myPositionPercent.x, anPercentOffset });
			}
		}
		void SetPosFromParentTop(float anPercentOffset = 0.0f)
		{
			if (myParent)
			{
				JPH::Float2 parentSize = myParent->GetSizeInPercent();
				JPH::Float2 parentPos = myParent->GetPositionInPercent();

				// Parent top edge = parent's center + half height
				SetScreenPositionInPercent({
					myPositionPercent.x,
					parentPos.y + (parentSize.y * 0.5f) - (mySizePercent.y * 0.5f) - anPercentOffset
					});
			}
			else
			{
				SetScreenPositionInPercent({ myPositionPercent.x, anPercentOffset });
			}
		}
		void SetPosFromParentLeft(float anPercentOffset = 0.0f)
		{
			if (myParent)
			{
				JPH::Float2 parentSize = myParent->GetSizeInPercent();
				JPH::Float2 parentPos = myParent->GetPositionInPercent();

				// Parent left edge = parent's center - half width
				SetScreenPositionInPercent({
					parentPos.x - (parentSize.x * 0.5f) + (mySizePercent.x * 0.5f) + anPercentOffset,
					myPositionPercent.y
					});
			}
			else
			{
				SetScreenPositionInPercent({ anPercentOffset, myPositionPercent.y });
			}
		}
		void SetPosFromParentRight(float anPercentOffset = 0.0f)
		{
			if (myParent)
			{
				JPH::Float2 parentSize = myParent->GetSizeInPercent();
				JPH::Float2 parentPos = myParent->GetPositionInPercent();

				// Parent right edge = parent's center + half width
				SetScreenPositionInPercent({
					parentPos.x + (parentSize.x * 0.5f) - (mySizePercent.x * 0.5f) - anPercentOffset,
					myPositionPercent.y
					});
			}
			else
			{
				SetScreenPositionInPercent({ anPercentOffset, myPositionPercent.y });
			}
		}
		void SetOnClick(std::function<void()> aCallback)
		{
			onClickCallback = aCallback;
		}
		void SetOnClickHeld(std::function<void()> aCallback)
		{
			onClickHeldCallback = aCallback;
		}
		void SetOnHover(std::function<void()> aCallback)
		{
			onHoverCallback = aCallback;
		}
		void SetOnHoverExit(std::function<void()> aCallback)
		{
			onHoverExitCallback = aCallback;
		}

	protected:
		JPH::Float2 myOriginalScalePixels;
		JPH::Float2 myOriginalWindowSize;
		JPH::Float2 myLastWindowSize;
		float myOriginalAspectRatio;

		std::function<void()> onClickCallback;
		std::function<void()> onClickHeldCallback;
		std::function<void()> onHoverCallback;
		std::function<void()> onHoverExitCallback;

		SpriteInstance2D* buttonHoverEffect;
		bool isHovered;

		float referenceScreenHeight = 1080.0f;

		JPH::Float2 myOriginalSizeRefScaling;

		static int nextID;
		std::string myName;
		int myID;
		int myRenderPriority; // Higher number means it will be rendered on top of other elements
		bool myIsRelative;
		bool myIsVisible;
		bool myIsAvailable; // Check if the element is available to claim
		std::vector<UI_Quad> myQuads;
		std::vector<UI_Element*> children;
		UI_Element* myParent;
		UI_Quad myParentBoxInPercent;
		UI_Quad myParentBoxInPixels;
		JPH::Float2 myPositionPercent;
		JPH::Float2 myPositionPixels;
		JPH::Float2 mySizePercent;
		JPH::Float2 mySizePixels;
		std::shared_ptr<SpriteInstance2D> mySpriteInstance;
	}
;}