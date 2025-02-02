//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <Editor/Elements/IProperties.h>

namespace Djinn::UI {
    class UIOutputDisplay;
}

namespace Djinn::Editor {
    class IPropertiesHost;

    ClassAttr(UIML::Class("editor-properties"))
    class Properties : public IProperties {
    public:
        DJINN_COM_CLASS();

        Properties(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    private:
        UI::UIOutputDisplay* display = nullptr;
        IPropertiesHost* host = nullptr;
    };
}
