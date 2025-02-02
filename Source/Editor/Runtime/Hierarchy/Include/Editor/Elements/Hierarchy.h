//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <Editor/Elements/IHierarchy.h>

namespace Djinn::UI {
    class UIOutputDisplay;
}

namespace Djinn::Editor {
    class IHierarchyHost;

    ClassAttr(UIML::Class("editor-hierarchy"))
    class Hierarchy : public IHierarchy {
    public:
        DJINN_COM_CLASS();

        Hierarchy(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;

    private:
        UI::UIOutputDisplay* display = nullptr;
        IHierarchyHost* host = nullptr;
    };
}
