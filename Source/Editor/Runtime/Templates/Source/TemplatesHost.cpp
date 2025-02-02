//
// Created by Dev on 1/17/2019.
//

#include <Editor/TemplatesHost.h>
#include <Editor/Elements/ITemplates.h>

DJINN_NS2(Editor);

COM::Result TemplatesHost::Register(ITemplates *templates) {
    instances.Add(templates->GetWeakPtr());
    return COM::kOK;
}

COM::Result TemplatesHost::Deregister(ITemplates *templates) {
    instances.RemoveValue(templates->GetWeakPtr());
    return COM::kOK;
}
