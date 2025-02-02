//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Core/Delegate.h>
#include <UI/UIMLEvent.h>
#include <Data/Asset.h>

namespace Djinn::Data {
    class ITextureAsset;
}

namespace Djinn::UI {
    class UILabel;
    class UIImage;

    /// Delegates
    using UIButtonPressedDelegate = Core::Delegate<void()>;

    ClassAttr(UIML::Class("button"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIButton : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIButton(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Set the button text
        /// \param text
        Attr(UIML::Setter("text"))
        void SetText(const Core::StringView &text);

        /// Get the button text
        /// \return
        Attr(UIML::Getter("text"))
        Core::StringView GetText() const;

        /// Set the texture of this image
        /// \param path
        Attr(UIML::Setter("path"))
        void LoadTexture(const Core::StringView &path);

        /// Set the texture of this image
        /// \param texture
        void SetTexture(const Data::Asset<Data::ITextureAsset> &texture);

        /// Get the texture
        /// \return
        const Data::Asset<Data::ITextureAsset> &GetTexture() const;

        /// Set the image geometry as a box
        void SetBoxGeometry();

        /// Set the image geometry as a margin box
        /// \param margin
        void SetMarginGeometry(const Math::Vector2 &margin);

    public:
        /// Events
        DJINN_UIML_EVENT("pressed", BindPressed, UIButtonPressedDelegate, pressed);

    private:
        UILabel *lbl;
        UIImage *img;
    };
}