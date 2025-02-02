#include <iostream>
#include "Host/Pipeline.h"

DJINN_NS2(Host);

COM::Result Pipeline::RemoveClass(const COM::ClassID &id) {
    // Remove
    classes.Remove(id);

    // OK
    return COM::kOK;
}

COM::Result Pipeline::AddClass(const COM::ClassID &id, void *instance) {
    if (!instance) {
        return COM::kInvalidParameter;
    }

    // Add
    classes.Add(id, instance);

    // OK
    return COM::kOK;
}

COM::Result Pipeline::GetClass(const COM::ClassID &id, void **out) {
    // Attempt to find
    auto it = classes.Find(id);
    if (it == classes.End()) {
        return kPipelineNoClass;
    }

    // Set
    *out = (*it).second;

    // OK
    return COM::kOK;
}

COM::Result Pipeline::RemoveInterface(const COM::InterfaceID &id) {
    // Remove
    interfaces.Remove(id);

    // OK
    return COM::kOK;
}

COM::Result Pipeline::AddInterface(const COM::InterfaceID &id, void *instance) {
    if (!instance) {
        return COM::kInvalidParameter;
    }

    // Add
    interfaces.Add(id, instance);

    // OK
    return COM::kOK;
}

COM::Result Pipeline::GetInterface(const COM::InterfaceID &id, void **out) {
    // Attempt to find
    auto it = interfaces.Find(id);
    if (it == interfaces.End()) {
        return kPipelineNoInterface;
    }

    // Set
    *out = (*it).second;

    // OK
    return COM::kOK;
}
