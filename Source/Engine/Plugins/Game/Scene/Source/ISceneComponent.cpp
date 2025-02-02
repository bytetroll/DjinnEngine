//
// Created by Dev on 8/30/2018.
//

#include <Game/ISceneComponent.h>
#include <Game/ISceneActor.h>

DJINN_NS2(Game);

ISceneActor *ISceneComponent::GetSceneActor() {
    return static_cast<ISceneActor*>(GetActor());
}
