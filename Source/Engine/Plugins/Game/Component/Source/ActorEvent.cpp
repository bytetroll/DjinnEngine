//
// Created by Dev on 8/3/2018.
//

#include <Game/ActorEvent.h>
#include <Game/IActorSystem.h>

DJINN_NS2(Game);

void Detail::QueueActorEvent(const Core::WeakPtr<IActorSystemInstance>& instance, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack) {
    DeferredActorEvent event;
    event.instance = instance;
    event.delegate = delegate;
    event.stack = stack;
    Sink(instance->GetSystem()->Queue(event));
}

