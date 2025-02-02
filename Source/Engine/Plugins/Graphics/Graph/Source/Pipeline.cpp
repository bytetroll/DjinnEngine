#include <Graphics/Pipeline.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPass.h>
#include <Base/PipelineConfigurationMode.h>
#include <Base/ISystem.h>

DJINN_NS2(Graphics);

Pipeline::Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPipeline(registry, outer, host) {

}


COM::Result Pipeline::Build(PassBuilder builder, double delta) {
    if (isDirty) {
        if (COM::CheckedResult result = Configure()) {
            return result;
        }
        isDirty = false;
    }

    // Preprocess
    for (auto state : passStates) {
        if (!state->pass->Preprocess(builder)) {
            return COM::kError;
        }
    }

    // Build
    for (auto state : passStates) {
        if (!state->pass->Build(state->builder = builder.InlinedPass(state->pass->GetClassName()), delta)) {
            return COM::kError;
        }
    }

    // Configure dependencies
    for (auto state : passStates) {
        for (auto dependency : state->dependencies) {
            state->builder.AddDependency(dependency->builder);
        }
    }

    return COM::kOK;
}

COM::Result Pipeline::Register(IPass *pass) {
    auto state = statePool.Pop();
    state->pass = pass;
    state->dependencies.Clear();
    passStates.Add(state);
    passMap[pass->GetClassID()] = pass;
    isDirty = true;
    return COM::kOK;
}

COM::Result Pipeline::Deregister(IPass *pass) {
    passStates.RemoveAll([&](PassState *state) { return state->pass == pass; });
    passMap.Remove(pass->GetClassID());
    isDirty = true;
    return COM::kOK;
}

COM::Result Pipeline::GetPass(const COM::ClassID &cid, IPass **out) {
    auto it = passMap.Find(cid);
    if (it != passMap.End()) {
        *out = (*it).second;
        return COM::kOK;
    }
    return COM::kError;
}

COM::Result Pipeline::GetPass(const COM::InterfaceID &iid, IPass **out) {
    for (auto &state : passStates) {
        void* dummy;
        if (state->pass->QueryInterface(iid, &dummy) && state->pass->QueryInterface(out)) {
            return COM::kOK;
        }
    }
    return COM::kError;
}

COM::Result Pipeline::Configure() {
    for (auto state : passStates) {
        if (COM::CheckedResult result = state->pass->Configure()) {
            ErrorLog(this).Write("Pipeline failed to configure, one or more of the registered passes failed to configure");
            return result;
        }
    }
    return COM::kOK;
}

COM::Result Pipeline::AddDependency(IPass *dependent, IPass *target) {
    for (auto state : passStates) {
        if (state->pass == dependent) {
            for (auto _state : passStates) {
                if (state->pass == target) {
                    state->dependencies.Add(_state);
                    return COM::kOK;
                }
            }
        }
    }
    return COM::kError;
}
