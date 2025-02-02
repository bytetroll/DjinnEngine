//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <Editor/Elements/ITemplates.h>

namespace Djinn::UI {
    class UIOutputDisplay;
}

namespace Djinn::Editor {
    class ITemplatesHost;

    ClassAttr(UIML::Class("editor-templates"))
    class Templates : public ITemplates {
    public:
        DJINN_COM_CLASS();

        Templates(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    private:
        UI::UIOutputDisplay* display = nullptr;
        ITemplatesHost* host = nullptr;
    };
}
