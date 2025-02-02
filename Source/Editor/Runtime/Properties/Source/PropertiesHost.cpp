//
// Created by Dev on 1/17/2019.
//

#include <Editor/PropertiesHost.h>
#include <Editor/Elements/IProperties.h>

DJINN_NS2(Editor);

COM::Result PropertiesHost::Register(IProperties *properties) {
    instances.Add(properties->GetWeakPtr());
    return COM::kOK;
}

COM::Result PropertiesHost::Deregister(IProperties *properties) {
    instances.RemoveValue(properties->GetWeakPtr());
    return COM::kOK;
}
