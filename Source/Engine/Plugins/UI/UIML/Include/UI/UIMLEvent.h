//
// Created by Dev on 10/1/2018.
//

#include "UIMLAnnotation.h"
#include <Game/ActorEvent.h>
#include <Core/DynamicDelegate.h>

namespace Djinn::UI {
    /// UIML event helper
#define DJINN_UIML_EVENT(STR, CALLBACK, DELEGATE, NAME) \
    Attr(UIML::Event(STR)) void CALLBACK(Djinn::Game::IActorSystemInstance *instance, const Djinn::Core::DynamicDelegate &delegate) { NAME.Add(instance, delegate.AsT<DELEGATE>()); } \
    Djinn::Game::ActorEvent<DELEGATE> NAME;
}