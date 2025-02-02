//
// Created by Dev on 1/17/2019.
//

#include <Editor/HierarchyHost.h>
#include <Editor/Elements/IHierarchy.h>

DJINN_NS2(Editor);

COM::Result HierarchyHost::Register(IHierarchy *hierarchy) {
    hierarchies.Add(hierarchy->GetWeakPtr());
    return COM::kOK;
}

COM::Result HierarchyHost::Deregister(IHierarchy *hierarchy) {
    hierarchies.RemoveValue(hierarchy->GetWeakPtr());
    return COM::kOK;
}
