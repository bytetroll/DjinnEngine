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
 on 2/24/2018.
//

/*
#include <Base/Application.h>
#include <Game/Entity.h>
#include <Host/InterfaceFinder.h>
#include <Game/IEntitySystem.h>
#include <Game/Components/InputComponent.h>
#include <Base/DefaultLogs.h>
#include <Game/Components/UITransformComponent.h>
#include <Game/Components/UILabelComponent.h>
#include <Data/GlyphConfig.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Data/WorldConfig.h>
#include <Game/Components/CameraComponent.h>
#include <Data/FontConfiguration.h>
#include <Data/IFontAsset.h>
#include <Core/Timer.h>
#include <Data/TextureConfiguration.h>
#include <Data/ITextureAsset.h>
#include <Game/Components/UIImageComponent.h>
#include <Game/Components/UIGraphComponent.h>
#include <ctime>
#include <Game/IGameSystem.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <Game/Components/UIInputComponent.h>
#include <Game/Components/UITextboxComponent.h>
#include <Game/Components/UISplineComponent.h>
#include <Game/Components/UIButtonComponent.h>
#include <Game/Components/UIFpsGraphComponent.h>
#include <Game/Components/UIFpsLabelComponent.h>
#include <Game/Components/UIWindowComponent.h>
#include <Game/Components/UICanvasComponent.h>
#include <Game/Components/System/DefaultComponentSystem.h>
#include <Game/Components/ComponentDependencies.h>
#include <Game/Components/UIEffectComponent.h>
#include <Game/Components/UICGTimeComponent.h>
#include <Graphics/IOutputHost.h>
#include <Game/Components/MeshComponent.h>
#include <Game/Components/UIOutputDisplayComponent.h>
#include <Data/IAssetHost.h>
#include <Data/IMeshAsset.h>
#include <Data/IMaterialAsset.h>
#include <Data/MeshCollectionAsset.h>
#include <Game/Components/DirectionalLightComponent.h>
#include <Graphics/ScreenOutput.h>
#include <Graphics/Output.h>
#include <Platform/User.h>
#include <Game/Components/UICGMemoryComponent.h>
#include <Game/Components/UITableComponent.h>
#include <Game/Components/UISliderComponent.h>
#include <Game/Components/UICheckboxComponent.h>
#include <Game/Components/UIListboxComponent.h>
#include <Game/Components/UINodeGraphComponent.h>
#include <Game/Components/UINodeComponent.h>
#include <Game/Components/UINodeConnectionComponent.h>
#include <Game/Components/UIMenubarComponent.h>
#include <Game/Components/UIMenuItemComponent.h>
#include <Base/MemoryStatistics.h>

DJINN_NS();

struct UIMoveComponent : public Game::IComponent<UIMoveComponent>, public Game::ComponentDependencies<Game::UITransformComponent> {
    DJINN_GAME_COMPONENT();

    void Update(MetaData meta, double delta) {
        auto t = meta.entity.GetFirstComponent<Game::UITransformComponent>();
        t->SetUserTransform(Math::Matrix4::RotateRPY({0, 0, elapsed}));
        elapsed += delta;
    }

    double elapsed = 0;
};

struct MoveComponent : public Game::IComponent<MoveComponent>, public Game::ComponentDependencies<Game::TransformComponent> {
    DJINN_GAME_COMPONENT();

    void Update(MetaData meta, double delta) {
        auto t = meta.entity.GetFirstComponent<Game::TransformComponent>();
        t->SetRotation(Math::Quaternion::RotateRPY({0, 0, Math::Cos(elapsed)*0.1f}));
        elapsed += delta;
    }

    // ...
    double elapsed = 0;
};

struct EditorCameraComponent : public Game::IComponent<EditorCameraComponent>, public Game::ComponentDependencies<Game::TransformComponent> {
    DJINN_GAME_COMPONENT();

    COM::Result Create(MetaData meta) {
        auto input = meta.entity.GetOrAddFirstComponent<Game::InputComponent>();
        transform = meta.entity.GetOrAddFirstComponent<Game::TransformComponent>();

        input->BindMapping("CameraForward"_id, Game::InputState::eDown, [=] {
            meta.component->direction += meta.component->transform->GetForward();
        });

        input->BindMapping("CameraBackward"_id, Game::InputState::eDown, [=] {
            meta.component->direction -= meta.component->transform->GetForward();
        });

        input->BindMapping("CameraRight"_id, Game::InputState::eDown, [=] {
            meta.component->direction += meta.component->transform->GetRight();
        });

        input->BindMapping("CameraLeft"_id, Game::InputState::eDown, [=] {
            meta.component->direction -= meta.component->transform->GetRight();
        });

        input->BindMapping("CameraUp"_id, Game::InputState::eDown, [=] {
            meta.component->direction -= meta.component->transform->GetUp();
        });

        input->BindMapping("CameraDown"_id, Game::InputState::eDown, [=] {
            meta.component->direction += meta.component->transform->GetUp();
        });

        input->BindMapping("CameraCapture"_id, Game::InputState::eDown, [=] {
            meta.component->isCaptured = true;
        });

        input->BindMapping("CameraCapture"_id, Game::InputState::eUp, [=] {
            meta.component->isCaptured = false;
        });

        input->BindEvent(Game::InputEventType::eMouseMoved, [=](const Game::InputEvent& event) {
            if (meta.component->isCaptured) {
                meta.component->rotation = {event.mouseMoved.delta.x, event.mouseMoved.delta.y};
            }
        });

        return COM::kOK;
    }

    void Update(MetaData meta, double delta) {
        velocity += direction * delta;
        rotationVelocity += rotation * delta * 0.4f;

        transform->SetPosition(transform->GetPosition() + velocity);
        transform->SetRotation(transform->GetRotation() * Math::Quaternion(Math::Vector3(transform->GetUp()), rotationVelocity.x) * Math::Quaternion(Math::Vector3(transform->GetRight()), -rotationVelocity.y));

        velocity = Math::Lerp(velocity, Math::Vector3(0, 0, 0), delta * 10.0f);
        rotationVelocity = Math::Lerp(rotationVelocity, Math::Vector2(0, 0), delta * 10.0f);
        direction = {};
        rotation = {};
    }

    Game::Component<Game::TransformComponent> transform;
    Math::Vector3 direction;
    Math::Vector3 velocity;
    Math::Vector2 rotation;
    Math::Vector2 rotationVelocity;
    bool isCaptured = false;
};

void _main(Base::Application &app) {
    srand(time(0));

    Core::Timer timer;

    // Load plugins.
    Always(app.GetRegistry()->LoadPlugins());

    // Get game system
    Host::InterfaceFinder<Game::IGameSystem> gameSystem(app.GetRegistry());
    gameSystem->SetMaxIPS(300);

    // Get graphics system
    Host::InterfaceFinder<Graphics::IGraphicsSystem> graphicsSystem(app.GetRegistry());

    // Get world worldHost
    Host::InterfaceFinder<Game::IWorldHost> worldHost(app.GetRegistry());

    // Create config
    Host::ScopedClass<Data::WorldConfig> config(app.GetRegistry());

    // Create world
    Game::IWorld *world;
    Always(worldHost->CreateWorld(config, &world));
    Always(worldHost->Register(world));

    Host::InterfaceFinder<Game::IEntitySystem> entitySystem(world->GetPipeline());
    Host::InterfaceFinder<Graphics::IOutputHost> outputHost(app.GetRegistry());
    Host::InterfaceFinder<Data::IAssetHost> assetHost(app.GetRegistry());
    Host::InterfaceFinder<Game::IInputSystem> inputSystem(app.GetRegistry());

    Game::DefaultComponentSystem<MoveComponent>::Install(world);
    Game::DefaultComponentSystem<UIMoveComponent>::Install(world);
    Game::DefaultComponentSystem<EditorCameraComponent>::Install(world);

    /////////////////////////////////////////

    inputSystem->AddMapping("CameraForward"_id, Game::InputMapping().Key(Platform::KeyCode::eW));
    inputSystem->AddMapping("CameraRight"_id, Game::InputMapping().Key(Platform::KeyCode::eD));
    inputSystem->AddMapping("CameraLeft"_id, Game::InputMapping().Key(Platform::KeyCode::eA));
    inputSystem->AddMapping("CameraBackward"_id, Game::InputMapping().Key(Platform::KeyCode::eS));
    inputSystem->AddMapping("CameraUp"_id, Game::InputMapping().Key(Platform::KeyCode::eE));
    inputSystem->AddMapping("CameraDown"_id, Game::InputMapping().Key(Platform::KeyCode::eQ));
    inputSystem->AddMapping("CameraCapture"_id, Game::InputMapping().Key(Platform::VirtualKeyCode::eMouseRight));

    // Prepare dummy scene
    Graphics::Output *sceneOutput;
    {
        Always(app.GetRegistry()->CreateClass(nullptr, &sceneOutput));
        Always(sceneOutput->Initialize(GHA::Size(
                Platform::User::GetPrimaryDisplayWidth(),
                Platform::User::GetPrimaryDisplayHeight()
        )));
        Always(outputHost->Register(sceneOutput));

        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        // Create camera component
        auto cam = entity.AddComponent<Game::CameraComponent>();
        cam->SetOutput(sceneOutput);
        cam->Lookat({0, 5, 0}, {0, 0, 0});

        entity.AddComponent<EditorCameraComponent>();

        // Sponza scene
        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto transform = entity.AddComponent<Game::TransformComponent>();
            transform->SetScale({0.2f, 0.2f, 0.2f});

            auto mesh = entity.AddComponent<Game::MeshComponent>();
            mesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/Sponza.obj"_path));
            mesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));

            //auto mover = entity.AddComponent<MoveComponent>();
        }

        // Sponza scene
        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto transform = entity.AddComponent<Game::TransformComponent>();
            transform->SetScale({0.2f, 0.2f, 0.2f});
            transform->SetRotation(Math::Quaternion({1, 0, 0}, Math::kPI));
            transform->SetPosition({0, 5, 0});

            auto mesh = entity.AddComponent<Game::MeshComponent>();
            mesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/Sponza.obj"_path));
            mesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));

            //auto mover = entity.AddComponent<MoveComponent>();
        }

        Game::Component<Game::TransformComponent> parent = nullptr;
        {
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto transform = entity.AddComponent<Game::TransformComponent>();
            transform->SetScale({0.05f, 0.05f, 0.05f});
            transform->SetPosition({0, 1, 0});
            transform->SetParent(parent);
            parent = transform;
        }

        for (int i = 0; i < 20; i++)  {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto transform = entity.AddComponent<Game::TransformComponent>();
            transform->SetPosition({0, 0.05, 0});
            transform->SetParent(parent);
            parent = transform;

            auto mesh = entity.AddComponent<Game::MeshComponent>();
            mesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/cube.obj"_path));
            mesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));

            auto mover = entity.AddComponent<MoveComponent>();
        }

        // Create dummy directional light
        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto light = entity.AddComponent<Game::DirectionalLightComponent>();


        }
    }

    Game::Component<Game::UITransformComponent> root;
    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        Graphics::ScreenOutput *output;
        Always(app.GetRegistry()->CreateClass(nullptr, &output));
        Always(output->Initialize());
        Always(outputHost->Register(output));
        Always(outputHost->SetOutput(output));

        // Create canvas
        auto canvas = entity.AddComponent<Game::UICanvasComponent>();
        canvas->SetOutput(output);

        // ...
        root = entity.GetFirstComponent<Game::UITransformComponent>();
        root->SetDesiredSize({Platform::User::GetPrimaryDisplayWidth(), Platform::User::GetPrimaryDisplayHeight()});
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        // Create transform component
        auto transform = entity.AddComponent<Game::UITransformComponent>();
        transform->SetScaling(Game::UIScaling::eFill, Game::UIScaling::eFill);

        // Create output display component
        auto pnl = entity.AddComponent<Game::UIOutputDisplayComponent>();
        pnl->SetOutput(sceneOutput);

        // Set canvas
        entity.GetFirstComponent<Game::UITransformComponent>()->SetParent(root);
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

    Base::ConsoleLogStream *logStream = nullptr;
    Always(app.GetRegistry()->GetPipeline()->GetClass(&logStream));
    Base::Memory::PrettyPrint(logStream);

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");
}
 */

/*
int main() {
    Base::Launch(Base::ApplicationInfo().Name("Scene Graphics Test"), Bindg(_main));
    return 0;
}*/