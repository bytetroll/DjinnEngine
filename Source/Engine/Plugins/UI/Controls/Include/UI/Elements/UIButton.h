//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/23/2018.
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