#include <Game/IActorSystemInstance.h>
#include <Game/IScene.h>

DJINN_NS2(Game);

void IActorSystemInstance::InitializeSystemInstance(IActorSystem *system, IScene* scene) {
    this->system = system;
    this->scene = scene;
    this->world = scene->GetWorld();
}
