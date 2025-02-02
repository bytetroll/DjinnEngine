//
// Created by Dev on 8/3/2018.
//

#include <Game/EntityEvent.h>
#include <Game/IEntitySystem.h>

DJINN_NS2(Game);

void Detail::QueueFreeEntityEvent(const OpaqueEntityComponent &sender, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack) {
    DeferredEntityEvent event;
    event.delegate = delegate;
    event.stack = stack;
    Sink(sender.GetReference().entity.system->Queue(event));
}

void Detail::QueueEntityEvent(const OpaqueEntityComponent &opaque, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack) {
    DeferredEntityEvent event;
    event.id = opaque.GetReference().entity.id;
    event.delegate = delegate;
    event.stack = stack;
    Sink(opaque.GetReference().entity.system->Queue(opaque.GetReference().component, event));
}
