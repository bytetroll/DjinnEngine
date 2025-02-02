//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <UI/IControl.h>

namespace Djinn::Editor {
    ClassAttr(UIML::Class("editor-content-preview"))
    class ContentPreview : public UI::IControl {
    public:
        DJINN_COM_CLASS();

        ContentPreview(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    public:
        /// Set the item text
        /// \param text
        Attr(UIML::Setter("text"))
        void SetText(const Core::StringView &text);
    };
}
