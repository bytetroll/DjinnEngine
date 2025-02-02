//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 15/06/2018.
//

#include <Base/Application.h>
#include <Game/Components/UICanvasComponent.h>
#include <Game/Components/UITransformComponent.h>
#include <Game/IWorld.h>
#include <Game/IWorldHost.h>
#include <Data/WorldConfig.h>
#include <Game/IEntitySystem.h>
#include <Host/InterfaceFinder.h>
#include <Graphics/ScreenOutput.h>
#include <Data/IAssetHost.h>
#include <Graphics/IOutputHost.h>
#include <Game/Components/UIImageComponent.h>
#include <Game/Components/UIFpsGraphComponent.h>
#include <Game/Components/UICGMemoryComponent.h>
#include <Game/Components/UICGTimeComponent.h>
#include <Game/Components/UIImageComponent.h>
#include <Game/Components/UILabelComponent.h>
#include <ctime>

DJINN_NS();

class AsteroidComponent : public Game::IComponent {
public:
    struct State : public Game::IComponentState {
        Math::Vector2 direction;
        Game::Component<Game::UITransformComponent> transform;
    };
    DJINN_GAME_COMPONENT(State);

    AsteroidComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IComponent(registry, outer, host) {

    }

    COM::Result Initialize() {
        Host::InterfaceFinder<Data::IAssetHost> host(registry);
        texture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Game/Textures/Rock.png"_path);
    }

    void Created(const Game::Entity &entity, const Game::OpaqueComponent &component) override {
        auto com = component.As<AsteroidComponent>();

        com->transform = entity.AddComponent<Game::UITransformComponent>();
        com->transform->SetDesiredSize({100, 100});
        com->transform->SetMargin({100, 100});

        auto img = entity.AddComponent<Game::UIImageComponent>();
        img->texture = texture;

        com->direction = Math::Vector2(Math::RandFFast(-1.0f, 1.0f), Math::RandFFast(-1.0f, 1.0f));
    }

    void Update(double delta) override {
        Async::Group group;
        host.UpdateAsync(group, [&](State &state) {
            state.transform->SetMargin(state.transform->GetMargin() + state.direction * 50 * delta);
        });
    }

    COM::Result Configure(Base::IPipeline *pipeline) override {
        return pipeline->AddDependency<Game::UITransformComponent>(this);
    }

    Game::IComponentHost *GetHost() override {
        return &host;
    }

private:
    Game::ComponentHost<State> host;
    Data::Asset<Data::ITextureAsset> texture;
};

class ShipComponent : public Game::IComponent {
public:
    struct State : public Game::IComponentState {
        Game::Component<Game::UITransformComponent> transform;
    };
    DJINN_GAME_COMPONENT(State);

    ShipComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IComponent(registry, outer, host) {

    }

    COM::Result Initialize() {
        Host::InterfaceFinder<Data::IAssetHost> host(registry);
        texture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Game/Textures/Rock.png"_path);
    }

    void Created(const Game::Entity &entity, const Game::OpaqueComponent &component) override {
        auto com = component.As<AsteroidComponent>();

        com->transform = entity.AddComponent<Game::UITransformComponent>();
        com->transform->SetDesiredSize({100, 100});
        com->transform->SetMargin({100, 100});

        auto img = entity.AddComponent<Game::UIImageComponent>();
    }

    void Update(double delta) override {
        Async::Group group;
        host.UpdateAsync(group, [&](State &state) {

        });
    }

    COM::Result Configure(Base::IPipeline *pipeline) override {
        return pipeline->AddDependency<Game::UITransformComponent>(this);
    }

    Game::IComponentHost *GetHost() override {
        return &host;
    }

private:
    Game::ComponentHost<State> host;
    Data::Asset<Data::ITextureAsset> texture;
};

class GameController : public Game::IComponent {
public:
    struct State : public Game::IComponentState { };
    DJINN_GAME_COMPONENT(State);

    GameController(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IComponent(registry, outer, host) {

    }

    COM::Result Initialize(Game::IEntitySystem* system, const Game::Component<Game::UITransformComponent>& root) {
        this->system = system;
        this->root = root;

        Always(system->CreateEntity(entity));

        auto transform = entity.AddComponent<Game::UITransformComponent>();
        transform->SetMargin({10, 150});
        transform->SetParent(root);
        transform->SetZOffset(1);

        auto label = entity.AddComponent<Game::UILabelComponent>();
        label->SetColor({0, 0, 0, 1});

        return COM::kOK;
    }

    void Update(double delta) override {
        if ((elapsed += delta) > 0.8f) {
            elapsed = 0;

            Game::Entity entity;
            Always(system->CreateEntity(entity));

            auto ast = entity.AddComponent<AsteroidComponent>();

            auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
            transform->SetParent(root);
            transform->SetMargin({Math::RandFFast(120.0f, 1800.0f), Math::RandFFast(120.0f, 1000.0f)});

            this->entity.GetFirstComponent<Game::UILabelComponent>()->SetText(Core::String("Asteroids: ") + (asteroids++));
        }
    }

    COM::Result Configure(Base::IPipeline *pipeline) override {
        if (!pipeline->AddDependency<Game::UITransformComponent>(this) && pipeline->AddDependency<Game::UILabelComponent>(this)) {
            return COM::kError;
        }
        return COM::kOK;
    }

    Game::IComponentHost *GetHost() override {
        return &host;
    }

private:
    Game::ComponentHost<State> host;
    Game::IEntitySystem* system;
    Game::Component<Game::UITransformComponent> root;
    Game::Entity entity;
    int asteroids = 0;
    double elapsed = 1;
};

void _main(Base::Application &app) {
    srand(time(0));

    // Load plugins.
    Always(app.GetRegistry()->LoadPlugins());

    // Get relevant interfaces
    Host::InterfaceFinder<Game::IWorldHost> worldHost(app.GetRegistry());
    Host::InterfaceFinder<Graphics::IOutputHost> outputHost(app.GetRegistry());
    Host::InterfaceFinder<Data::IAssetHost> assetHost(app.GetRegistry());

    // Create base configuration
    Host::ScopedClass<Data::WorldConfig> config(app.GetRegistry());

    // Create world
    Game::IWorld *world;
    Always(worldHost->CreateWorld(config, &world));
    Always(worldHost->Register(world));

    // Get relevant interfaces
    Host::InterfaceFinder<Game::IEntitySystem> entitySystem(world->GetPipeline());

    // Create output and canvas
    Game::Component<Game::UITransformComponent> root;
    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        // Create screen output
        Graphics::ScreenOutput *output;
        Always(app.GetRegistry()->CreateClass(&app, &output));

        // Create output and register
        Always(output->Initialize());
        Always(outputHost->Register(output));
        Always(outputHost->SetOutput(output));

        // Create canvas
        auto canvas = entity.AddComponent<Game::UICanvasComponent>();
        canvas->output = output;

        // Gety root transform
        root = entity.GetFirstComponent<Game::UITransformComponent>();
    }

    {
        app.GetRegistry()->InstallClass<AsteroidComponent>();
        app.GetRegistry()->InstallClass<ShipComponent>();
        app.GetRegistry()->InstallClass<GameController>();

        // Install effect
        AsteroidComponent *asteroid;
        Always (app.GetRegistry()->CreateClass(&app, &asteroid) && asteroid->Initialize() && world->GetPipeline()->AddClass(asteroid) && entitySystem->Register(asteroid));

        // Install effect
        ShipComponent *ship;
        Always (app.GetRegistry()->CreateClass(&app, &ship) && ship->Initialize() && world->GetPipeline()->AddClass(ship) && entitySystem->Register(ship));

        // Install effect
        GameController *gameController;
        Always (app.GetRegistry()->CreateClass(&app, &gameController) && gameController->Initialize(entitySystem, root) && world->GetPipeline()->AddClass(gameController) && entitySystem->Register(gameController));
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto graph = entity.AddComponent<Game::UICGTimeComponent>();

        auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
        transform->SetZOffset(2);
        transform->SetMargin({230, 10});
        transform->SetParent(root);
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto graph = entity.AddComponent<Game::UICGMemoryComponent>();

        auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
        transform->SetZOffset(2);
        transform->SetMargin({230, 30});
        transform->SetParent(root);
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto graph = entity.AddComponent<Game::UIFpsGraphComponent>();

        //entity.AddComponent<UIMoveComponent>();

        // Set canvas
        auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
        transform->SetZOffset(2);
        transform->SetMargin({10, 10});
        transform->SetParent(root);
    }
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("Minigame - Rock Shooter"), Bindg(_main));
    return 0;
}