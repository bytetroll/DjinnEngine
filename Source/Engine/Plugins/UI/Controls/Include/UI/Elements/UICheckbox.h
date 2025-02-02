//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Core/Delegate.h>
#include <Game/ActorEvent.h>
#include <Data/Asset.h>
#include <Core/Annotation.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::Data {
    class ITextureAsset;
}

namespace Djinn::UI {
    class UIImage;

    /// Delegates
    using UICheckboxCheckedDelegate = Core::Delegate<void(bool checked)>;

    ClassAttr(UIML::Class("checkbox"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UICheckbox : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UICheckbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Set checked state
        /// \param value
        Attr(UIML::Setter("checked"))
        void SetChecked(bool value);

        /// Is this checkbox checked?
        /// \return
        bool IsChecked() const {
            return isChecked;
        }

        /// Events
        Game::ActorEvent<UICheckboxCheckedDelegate> checked;

    private:
        UIImage* img;

        Data::Asset<Data::ITextureAsset> onTexture;
        Data::Asset<Data::ITextureAsset> offTexture;

        bool isChecked = false;
    };
}