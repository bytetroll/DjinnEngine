//
// Created by nathan on 1/12/21.
//

#pragma once

#include <ctime>

#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Base/MemoryStatistics.h>
#include <Game/Entity.h>
#include <Game/IEntitySystem.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Game/IGameSystem.h>
#include <Game/ISceneComponent.h>
#include <Game/ISceneActor.h>
#include <Game/ActorCallback.h>
#include <Game/IActorSystem.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Game/ActorComponents/InputComponent.h>
#include <Game/Actors/StaticMesh.h>
#include <Game/Actors/DirectionalLight.h>
#include <Game/Actors/Location.h>
#include <Data/GlyphConfig.h>
#include <Data/WorldConfig.h>
#include <Data/FontConfiguration.h>
#include <Data/IFontAsset.h>
#include <Data/IAssetHost.h>
#include <Data/IMeshAsset.h>
#include <Data/IMaterialAsset.h>
#include <Data/MeshCollectionAsset.h>
#include <Core/Timer.h>
#include <Data/TextureConfiguration.h>
#include <Data/ITextureAsset.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IOutputHost.h>
#include <Graphics/ScreenOutput.h>
#include <Graphics/Output.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <Platform/User.h>
#include <UI/Elements/UICanvas.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UIFpsGraph.h>
#include <UI/Elements/UIFpsLabel.h>
#include <UI/IUIInputSystem.h>
#include <UI/Elements/CallGraph.h>
#include <UI/Elements/CallStack.h>
#include <Math/Vector3I.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>

//#include <MoveComponent.h>
//#include <EditorCamera.h>

//#include <Base/Application.h>
//#include <Core/Timer.h>
//#include <Game/IGameSystem.h>
//#include <Game/IWorldHost.h>
//#include <Game/IEntitySystem.h>
//#include <Game/IInputSystem.h>
//#include <Game/ActorCallback.h>
//#include <Game/Actors/Camera.h>
//#include <Game/ActorComponents/TransformComponent.h>
//#include <Game/Actors/Camera.h>
//#include <Game/IActorSystem.h>
//#include <Game/ISceneActor.h>
//#include <Game/ActorComponents/InputComponent.h>
//#include <Game/ActorComponents/ITransformComponent.h>
//#include <Graphics/IGraphicsSystem.h>
//#include <Graphics/IOutputHost.h>
//#include <Data/WorldConfig.h>
//#include <Data/IAssetHost.h>
//#include <Host/InterfaceFinder.h>
//#include <Math/Vector3.h>

// Graphics/Stress is the most up-to-date example.

//namespace Tetris {
//    using namespace Djinn;
//    using namespace Djinn::Base;
//    using namespace Djinn::Core;
//    using namespace Djinn::Game;
//    using namespace Djinn::Graphics;
//    using namespace Djinn::Host;
//    using namespace Djinn::COM;
//    using namespace Djinn::Math;

//    class TetrisCamera : public ISceneActor {
//        // Tetris camera component.
//    public:
//        DJINN_COM_CLASS();
//
//    public:
//        TetrisCamera(IRegistry* registry, IUnknown* outer, IInstanceHost* host):
//            ISceneActor(registry, outer, host) {
//        }
//
//        void StaticInitialize() override {
//            SetTransform(CreateComponent<Game::TransformComponent>());
//            Always(GetSystem()->CreateActor(GetScene(), &camera));
//            camera->GetTransform()->SetParent(GetTransform());
//            input = CreateComponent<InputComponent>();
//        }
//
//        void Created() override {
//            callback.SetCallback(Bindff(Update));
//            callback.Register(this);
//
//            ITransformComponent* transform = GetTransform();
//
//            input->BindMapping("TetrominoRotateLeft"_id, InputState::ePressed, [=] {
//                ;;
//            });
//
//            input->BindMapping("TetrominoRotateRight"_id, InputState::ePressed, [=] {
//                ;;
//            });
//
//            input->BindMapping("TetrominoDrop"_id, InputState::ePressed, [=] {
//                ;;
//            });
//        }
//
//        void Update(double delta) override {
//            ITransformComponent* transform = GetTransform();
//
//            positionVelocity += (direction * delta) * 0.2f;
//            rotationVelocity += (rotation * delta) * 0.3f;
//
//            // Apply rotation and unwind x.
//            angle += rotationVelocity;
//            angle.y = Clamp(angle.y, (-kPI / 2.0f), (kPI / 2.0f));
//
//            if (angle.x > (kPI * 2.0f)) {
//                angle.x -= kPI * 2.0f;
//            } else if(angle.x < (-kPI * 2.0f)) {
//                angle.x = (kPI * 2.0f) - angle.x;
//            }
//
//            transform->SetPosition(transform->GetPosition() + positionVelocity);
//            transform->SetRotation(
//                Quaternion::RotateRPY({
//                    -angle.y,
//                    angle.x,
//                    0.0f
//                }) *
//                Quaternion(
//                    transform->GetForward(),
//                    (-rotationVelocity.x * 1.5f)
//                ) *
//                Quaternion({
//                    1.0f,
//                    0.0f,
//                    0.0f
//                }),
//                kPI
//            );
//
//            // Damp velocities.
//            positionVelocity = Lerp(positionVelocity, Vector3(0.0f, 0.0f, 0.0f),
//                                    (delta * 10.0f));
//            rotationVelocity = Lerp(rotationVelocity, Vector2(0.0f, 0.0f),
//                                    (delta * 10.0f));
//
//            direction = {};
//            rotation = {};
//        }
//
//        void SetOutput(IOutput* output) {
//            camera->SetOutput(output);
//        }
//
//    private:
//        InputComponent* input = nullptr;
//        Camera* camera = nullptr;
//        ActorCallback callback;
//        Vector3 direction;
//        Vector2 rotation;
//        Vector3 positionVelocity;
//        Vector2 rotationVelocity;
//        Vector2 angle; // Camera angle.
//        bool captured = false; // Is the camera captured?
//    };
//
//    class TetrisMover : public ISceneComponent {
//    // Tetris move component.
//    public:
//        DJINN_COM_CLASS();
//
//    public:
//        TetrisMover(IRegistry* registry, IUnknown* outer, IInstanceHost* host) :
//            ISceneComponent(registry, outer, host) {
//        }
//
//        void Created() override {
//            callback.SetCallback(Bindff(Update));
//            callback.Register(this);
//            offset = GetSceneActor()->GetTransform()->GetPosition();
//        }
//
//        void Update(double delta) {
//            GetSceneActor()->GetTransform()->SetPosition(offset + Vector3(
//                0.0f,
//                Cos(static_cast<float>(((elapsed += delta) + offset.x) + offset.z)) * 0.25f,
//                0.0f
//            ));
//        }
//
//    private:
//        Vector3 offset;
//        ActorCallback callback;
//        double elapsed = 0;
//    };

//    class Tetris {
//    public:
//        bool Load(Application const& app);
//        void Think();
//        void Unload();
//
//    private:
//        Application* app;
//        Timer* timer;
//
//        // Interfaces.
//    };
//};
