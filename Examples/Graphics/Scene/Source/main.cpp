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

#include <Base/Application.h>
#include <Game/Entity.h>
#include <Game/IEntitySystem.h>
#include <Base/DefaultLogs.h>
#include <Data/GlyphConfig.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Data/WorldConfig.h>
#include <Data/FontConfiguration.h>
#include <Data/IFontAsset.h>
#include <Core/Timer.h>
#include <Data/TextureConfiguration.h>
#include <Data/ITextureAsset.h>
#include <ctime>
#include <Game/IGameSystem.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <Graphics/IOutputHost.h>
#include <Data/IAssetHost.h>
#include <Data/IMeshAsset.h>
#include <Data/IMaterialAsset.h>
#include <Data/MeshCollectionAsset.h>
#include <Graphics/ScreenOutput.h>
#include <Graphics/Output.h>
#include <Platform/User.h>
#include <Base/MemoryStatistics.h>
#include <Game/ISceneComponent.h>
#include <Game/ISceneActor.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Game/ActorCallback.h>
#include <Game/IActorSystem.h>
#include <Game/ActorComponents/InputComponent.h>
#include <UI/Elements/UICanvas.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UIFpsGraph.h>
#include <UI/Elements/UIFpsLabel.h>
#include <Game/Actors/StaticMesh.h>
#include <Game/Actors/DirectionalLight.h>
#include <UI/IUIInputSystem.h>
#include <MoveComponent.h>
#include <EditorCamera.h>
#include <UI/Elements/CallGraph.h>
#include <Game/Actors/Location.h>

DJINN_NS();

void EditorCamera::Created() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    auto transform = GetTransform();

    input->BindMapping("CameraForward"_id, Game::InputState::eDown, [=] {
        direction += transform->GetForward();
    });

    input->BindMapping("CameraBackward"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetForward();
    });

    input->BindMapping("CameraRight"_id, Game::InputState::eDown, [=] {
        direction += transform->GetRight();
    });

    input->BindMapping("CameraLeft"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetRight();
    });

    input->BindMapping("CameraUp"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetUp();
    });

    input->BindMapping("CameraDown"_id, Game::InputState::eDown, [=] {
        direction += transform->GetUp();
    });

    input->BindMapping("CameraCapture"_id, Game::InputState::eDown, [=] {
        isCaptured = true;
    });

    input->BindMapping("CameraCapture"_id, Game::InputState::eUp, [=] {
        isCaptured = false;
    });

    input->BindEvent(Game::InputEventType::eMouseMoved, [=](const Game::InputEvent &event) {
        if (isCaptured) {
            rotation = {event.mouseMoved.delta.x, event.mouseMoved.delta.y};
        }
    });
}
void EditorCamera::Update(double delta) {
    auto transform = GetTransform();

    // Apply velocities
    positionVelocity += direction * delta * 0.2f;
    rotationVelocity += rotation * delta * 0.3f;

    // Apply rotation velocity
    angles += rotationVelocity;

    // Clamp y
    angles.y = Math::Clamp(angles.y, -Math::kPI / 2.0f, Math::kPI / 2.0f);

    // Unwind x
    if (angles.x > Math::kPI * 2) {
        angles.x -= Math::kPI * 2;
    } else if (angles.x < -Math::kPI * 2) {
        angles.x = Math::kPI * 2 - angles.x;
    }

    // Set transform
    transform->SetPosition(transform->GetPosition() + positionVelocity);
    transform->SetRotation(Math::Quaternion::RotateRPY({-angles.y, angles.x, 0}) * Math::Quaternion(transform->GetForward(), -rotationVelocity.x * 1.5f) * Math::Quaternion({1, 0, 0}, Math::kPI));

    // Damp velocities
    positionVelocity = Math::Lerp(positionVelocity, Math::Vector3(0, 0, 0), delta * 10.0f);
    rotationVelocity = Math::Lerp(rotationVelocity, Math::Vector2(0, 0), delta * 10.0f);

    // Reset immediates
    direction = {};
    rotation = {};
}

void _main(Base::Application &app) {
    srand(time(0));

    Core::Timer timer;

    // Load plugins.
    Always(app.GetRegistry()->LoadPlugins());

    // Default pipeline should contain our interfaces
    auto pipeline = app.GetRegistry()->GetPipeline();

    // Interfaces
    Game::IGameSystem *gameSystem;
    Graphics::IGraphicsSystem *graphicsSystem;
    Game::IWorldHost *worldHost;
    Graphics::IOutputHost *outputHost;
    Data::IAssetHost *assetHost;
    Game::IInputSystem *inputSystem;
    Always(
            pipeline->GetInterface(&gameSystem) &&
            pipeline->GetInterface(&graphicsSystem) &&
            pipeline->GetInterface(&inputSystem) &&
            pipeline->GetInterface(&worldHost) &&
            pipeline->GetInterface(&outputHost) &&
            pipeline->GetInterface(&assetHost)
    );

    // Default settings
    gameSystem->SetMaxIPS(300);

    // Create config
    Data::WorldConfig *config;
    Always(app.GetRegistry()->CreateClass(&app, &config));

    // Create world
    Game::IWorld *world;
    Always(worldHost->CreateWorld(config, &world));
    Always(worldHost->Register(world));

    // Create scene
    Game::IScene *scene;
    Always(world->CreateScene(&scene));

    // World interfaces
    Game::IActorSystem *actorSystem;
    UI::IUIInputSystem *uiInputSystem;
    Always(
            world->GetPipeline()->GetInterface(&actorSystem) &&
            world->GetPipeline()->GetInterface(&uiInputSystem)
    );

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
        Always(app.GetRegistry()->CreateClass(&app, &sceneOutput));
        Always(sceneOutput->Initialize(GHA::Size(
                Platform::User::GetPrimaryDisplayWidth(),
                Platform::User::GetPrimaryDisplayHeight()
        )));
        Always(outputHost->Register(sceneOutput));

        Game::Location* location;
        Always(actorSystem->CreateActor(scene, &location));

        {
            EditorCamera *camera;
            Always(actorSystem->CreateActor(scene, &camera));

            camera->SetOutput(sceneOutput);

            location->AddActor(camera);
        }

        // Sponza scene
        {
            Game::StaticMesh *sponzaMesh;
            Always(actorSystem->CreateActor(scene, &sponzaMesh));

            sponzaMesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/Sponza.obj"_path));
            sponzaMesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));

            auto transform = sponzaMesh->GetTransform();
            transform->SetScale({0.5f, 0.5f, 0.5f});

            location->AddActor(sponzaMesh);
        }

        Game::ITransformComponent *parent = nullptr;
        for (int i = 0; i < 20; i++) {
            Game::StaticMesh *cubeMesh;
            Always(actorSystem->CreateActor(scene, &cubeMesh));

            cubeMesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/cube.obj"_path));
            cubeMesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));

            auto transform = cubeMesh->GetTransform();
            transform->SetPosition({0, 0.2f, 0});
            if (!parent) {
                transform->SetScale({.1f, .1f, .1f});
            }
            transform->SetParent(parent);

            cubeMesh->CreateComponent<MoveComponent>();

            if (!parent) {
                location->AddActor(cubeMesh);
            }

            parent = transform;
        }

        // Create dummy directional light
        {
            Game::DirectionalLight *dirLight;
            Always(actorSystem->CreateActor(scene, &dirLight));

            location->AddActor(dirLight);
        }
    }

    UI::UICanvas *canvas;
    Always(actorSystem->CreateActor(scene, &canvas));
    {
        Graphics::ScreenOutput *output;
        Always(app.GetRegistry()->CreateClass(&app, &output));
        Always(output->Initialize());
        Always(outputHost->Register(output));
        Always(outputHost->SetOutput(output));

        canvas->SetScaling(true);
        canvas->SetOutput(output);

        uiInputSystem->Register(canvas);
    }

    {
        gameSystem->EnableAsyncRecording(true);

        UI::CallGraph* callstack;
        Always(actorSystem->CreateActor(scene, &callstack));

        auto transform = callstack->GetTransform();
        transform->SetMargin({15, 15});
        transform->SetDesiredSize({0, 100});
        transform->SetHorizontalScaling(UI::UIScaling::eFill);

        //canvas->AddElement(callstack);
    }

    {
        UI::UIOutputDisplay *display;
        Always(actorSystem->CreateActor(scene, &display));

        display->SetOutput(sceneOutput);

        auto transform = display->GetTransform();
        transform->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);

        canvas->AddElement(display);
    }

    {
        UI::UIFpsLabel *fpsLabel;
        Always(actorSystem->CreateActor(scene, &fpsLabel));

        auto transform = fpsLabel->GetTransform();
        transform->SetMargin({25, 25});

        canvas->AddElement(fpsLabel);
    }

    {
        UI::UICGTime *cgTime;
        Always(actorSystem->CreateActor(scene, &cgTime));

        auto transform = cgTime->GetTransform();
        transform->SetMargin({25, 50});

        canvas->AddElement(cgTime);
    }

    {
        UI::UIFpsGraph *fpsGraph;
        Always(actorSystem->CreateActor(scene, &fpsGraph));

        auto transform = fpsGraph->GetTransform();
        transform->SetMargin({25, 75});
        transform->SetDesiredSize({200, 100});

        canvas->AddElement(fpsGraph);
    }

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("Scene Graphics Test"), Bindg(_main));
    return 0;
}