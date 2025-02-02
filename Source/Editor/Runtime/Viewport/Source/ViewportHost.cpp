//
// Created by Dev on 1/17/2019.
//

#include <Editor/ViewportHost.h>
#include <Editor/Elements/IViewport.h>

DJINN_NS2(Editor);

COM::Result ViewportHost::Register(IViewport *viewport) {
    viewports.Add(viewport->GetWeakPtr());
    return COM::kOK;
}

COM::Result ViewportHost::Deregister(IViewport *viewport) {
    viewports.RemoveValue(viewport->GetWeakPtr());
    return COM::kOK;
}
