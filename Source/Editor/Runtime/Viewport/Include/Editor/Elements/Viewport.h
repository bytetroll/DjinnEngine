//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <Editor/Elements/IViewport.h>

namespace Djinn::UI {
    class UIOutputDisplay;
}

namespace Djinn::Editor {
    class IViewportHost;

    ClassAttr(UIML::Class("editor-viewport"))
    class Viewport : public IViewport {
    public:
        DJINN_COM_CLASS();

        Viewport(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    private:
        UI::UIOutputDisplay* display = nullptr;
        IViewportHost* host = nullptr;
    };
}
