#include <Graphics/Graph.h>
#include <Graphics/Pipeline.h>
#include <Graphics/RootProcess.h>
#include <GHA/IScheduler.h>
#include <GHA/ScheduledNode.h>
#include <GHA/SchedulerContext.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Graphics);

Graph::Graph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IGraph(registry, outer, host) {

}

COM::Result Graph::Initialize() {
    if (!registry->CreateClass(this, &process) || !process->Initialize()) {
        ErrorLog(this).Write("Failed to initialize root process");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result Graph::Compile(GHA::IScheduler *scheduler, double delta) {
    // Flush previous nodes
    scheduledNodePool.Flush();

    // Get context and reset
    GHA::SchedulerContext* context = scheduler->GetContext();
    context->Reset();

    // Reset context
    passContext.Clear();

    // Build root process
    if (!process->Build(PassBuilder::Make("Root", &scheduledNodePool, &passContext, context), delta)) {
        return COM::kError;
    }

    // Get objects
    const auto& objects = scheduledNodePool.GetObjects();

    // Compile nodes
    scheduledGHANodes.ResizePassive(objects.Size());
    for (USize i = 0; i < objects.Size(); i++) {
        auto& node = (scheduledGHANodes[i] = objects[i]->node);
        if (objects[i]->isInvalid) {
            node->commands.Clear();
        }
    }

    // Submit
    if (!scheduler->Schedule(static_cast<UInt32>(objects.Size()), scheduledGHANodes.Ptr())) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


IProcess *Graph::GetProcess() {
    return process;
}
