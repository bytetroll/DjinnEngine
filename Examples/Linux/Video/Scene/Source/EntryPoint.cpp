// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

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
#include <Game/Components/UICanvasComponent.h>
#include <Game/Components/UIEffectComponent.h>
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
#include <Game/Components/UITableComponent.h>
#include <Game/Components/UISliderComponent.h>
#include <Game/Components/UICheckboxComponent.h>
#include <Game/Components/UIListboxComponent.h>
#include <Game/Components/UINodeGraphComponent.h>
#include <Game/Components/UINodeComponent.h>
#include <Game/Components/UINodeConnectionComponent.h>
#include <Base/MemoryStatistics.h>

#include <iostream>
#include <csignal>

DJINN_NS();

//class MoveComponenet : public Game::IComponent< MoveComponenet >,
//                       public Game::ComponentDependencies< Game::TransformComponent > {
//public:
//    DJINN_GAME_COMPONENT();
//
//public:
//    void Update( MetaData meta, double delta ) {
//        auto transform = meta.entity.GetFirstComponent< Game::TransformComponent >();
//        transform->SetRotation( Math::Quaternion::RotateRPY( {
//                0, 0, ( Math::Cos( elapsed ) * 0.1f )
//        } ) );
//        elapsed += delta;
//    }
//
//public:
//    double elapsed = 0;
//};
//
//class CameraComponenet : public Game::IComponent< CameraComponenet >,
//                         public Game::ComponentDependencies< Game::TransformComponent > {
//public:
//    DJINN_GAME_COMPONENT();
//
//public:
//    COM::Result Create( MetaData meta ) {
//        auto input = meta.entity.GetOrAddFirstComponent< Game::InputComponent >();
//        transform = meta.entity.GetOrAddFirstComponent< Game::TransformComponent >();
//
//        input->BindMapping( "CameraForward"_id, Game::InputState::eDown, [ = ] {
//            meta.component->direction += meta.component->transform->GetForward();
//        } );
//
//        input->BindMapping(" CameraBackward"_id, Game::InputState::eDown, [ = ] {
//            meta.component->direction -= meta.component->transform->GetForward();
//        } );
//
//        input->BindMapping( "CameraRight"_id, Game::InputState::eDown, [ = ] {
//            meta.component->direction += meta.component->transform->GetRight();
//        } );
//
//        input->BindMapping( "CameraLeft"_id, Game::InputState::eDown, [ = ] {
//            meta.component->direction -= meta.component->transform->GetRight();
//        } );
//
//        input->BindMapping( "CameraUp"_id, Game::InputState::eDown, [ = ]  {
//            meta.component->direction -= meta.component->transform->GetUp();
//        } );
//
//        input->BindMapping( "CameraDown"_id, Game::InputState::eDown, [ = ] {
//            meta.component->direction += meta.component->transform->GetUp();
//        } );
//
//        input->BindMapping( "CameraCapture"_id, Game::InputState::eDown, [ = ] {
//            meta.component->isCaptured = true;
//        } );
//
//        input->BindMapping( "CameraCapture"_id, Game::InputState::eUp, [ = ] {
//            meta.component->isCaptured = false;
//        } );
//
//        input->BindEvent( Game::InputEventType::eMouseMoved, [ = ]( const Game::InputEvent& event ) {
//            if( meta.component->isCaptured)  {
//                meta.component->rotation = {
//                        event.mouseMoved.delta.x,
//                        event.mouseMoved.delta.y
//                };
//            }
//        } );
//
//        return COM::kOK;
//    }
//
//    void Update( MetaData meta, double delta ) {
//        velocity += direction * delta;
//        rotationVelocity += ( rotation * delta ) * 0.4f;
//
//        transform->SetPosition( transform->GetPosition() + velocity) ;
//        transform->SetRotation( transform->GetRotation() * Math::Quaternion( Math::Vector3( transform->GetUp() ),
//                                rotationVelocity.x ) * Math::Quaternion( Math::Vector3( transform->GetRight() ),
//                                 -rotationVelocity.y ) );
//
//        velocity = Math::Lerp( velocity, Math::Vector3( 0.0f, 0.0f, 0.0f ), ( delta * 10.0f ) );
//        rotationVelocity = Math::Lerp( rotationVelocity, Math::Vector2( 0.0f, 0.0f ), ( delta * 10.0f ));
//        direction = {};
//        rotation = {};
//    }
//
//public:
//    Game::Component< Game::TransformComponent > transform;
//
//    Math::Vector3 direction;
//    Math::Vector3 velocity;
//    Math::Vector2 rotation;
//    Math::Vector2 rotationVelocity;
//
//    bool isCaptured = false;
//};
//
//void _Main( Base::Application& app ) {
//    // Initialize the scene timer backend and create the timer.
//    std::srand( time( 0 ) );
//    Core::Timer timer;
//
//    // Load our plugins and initialize.
//    Always( app.GetRegistry()->LoadPlugins() );
//
////    // Get our game system.
////    Host::InterfaceFinder< Game::IGameSystem > gameSystem( app.GetRegistry() );
////    gameSystem->SetMaxIPS( 300 );
////
////    // Get graphics system.
////    Host::InterfaceFinder< Graphics::IGraphicsSystem > graphicsSystem( app.GetRegistry() );
////
////    // Get world worldHost.
////    Host::InterfaceFinder< Game::IWorldHost > worldHost( app.GetRegistry() );
////
////    // Create world config.
////    Host::ScopedClass< Data::WorldConfig > config( app.GetRegistry() );
////
////    // Create the world.
////    Game::IWorld* world;
////    Always( worldHost->CreateWorld( config, &world ) );
////    Always( worldHost->Register( world ) );
////
////    Host::InterfaceFinder< Game::IEntitySystem > entitySystem( world->GetPipeline() );
////    Host::InterfaceFinder< Graphics::IOutputHost > outputHost( app.GetRegistry() );
////    Host::InterfaceFinder< Data::IAssetHost > assetHost( app.GetRegistry() );
////    Host::InterfaceFinder< Game::IInputSystem > inputSystem( app.GetRegistry() );
//
////    Game::DefaultComponentSystem< MoveComponenet >::Install( world );
////    Game::DefaultComponentSystem< CameraComponenet >::Install( world );
////
////    inputSystem->AddMapping( "CameraForward"_id, Game::InputMapping().Key( Platform::KeyCode::eW ) );
////    inputSystem->AddMapping( "CameraRight"_id, Game::InputMapping().Key( Platform::KeyCode::eD ) );
////    inputSystem->AddMapping( "CameraLeft"_id, Game::InputMapping().Key( Platform::KeyCode::eA ) );
////    inputSystem->AddMapping( "CameraBackward"_id, Game::InputMapping().Key( Platform::KeyCode::eS ) );
////    inputSystem->AddMapping( "CameraUp"_id, Game::InputMapping().Key( Platform::KeyCode::eE ) );
////    inputSystem->AddMapping( "CameraDown"_id, Game::InputMapping().Key( Platform::KeyCode::eQ ) );
////    inputSystem->AddMapping( "CameraCapture"_id, Game::InputMapping().Key( Platform::VirtualKeyCode::eMouseRight ) );
//
//
//    // Prepare dummy scene
////    Graphics::Output *sceneOutput;
////    {
////        Always(app.GetRegistry()->CreateClass(nullptr, &sceneOutput));
////        Always(sceneOutput->Initialize(GHA::Size(
////                Platform::User::GetPrimaryDisplayWidth(),
////                Platform::User::GetPrimaryDisplayHeight()
////        )));
////        Always(outputHost->Register(sceneOutput));
////
////        // Create entity
////        Game::Entity entity;
////        Always(entitySystem->CreateEntity(entity));
////
////        // Create camera component
////        auto cam = entity.AddComponent<Game::CameraComponent>();
////        cam->SetOutput(sceneOutput);
////        cam->Lookat({0, 5, 0}, {0, 0, 0});
////
////        entity.AddComponent<CameraComponenet>();
////
////        // Sponza scene
////        {
////            // Create entity
////            Game::Entity entity;
////            Always(entitySystem->CreateEntity(entity));
////
////            auto transform = entity.AddComponent<Game::TransformComponent>();
////            transform->SetScale({0.2f, 0.2f, 0.2f});
////
////            auto mesh = entity.AddComponent<Game::MeshComponent>();
////            mesh->SetMesh(
////                    assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/Sponza.obj"_path));
////            mesh->SetMaterial(
////                    assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));
////
////            //auto mover = entity.AddComponent<MoveComponent>();
////        }
////
////        // Sponza scene
////        {
////            // Create entity
////            Game::Entity entity;
////            Always(entitySystem->CreateEntity(entity));
////
////            auto transform = entity.AddComponent<Game::TransformComponent>();
////            transform->SetScale({0.2f, 0.2f, 0.2f});
////            transform->SetRotation(Math::Quaternion({1, 0, 0}, Math::kPI));
////            transform->SetPosition({0, 5, 0});
////
////            auto mesh = entity.AddComponent<Game::MeshComponent>();
////            mesh->SetMesh(
////                    assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/Sponza.obj"_path));
////            mesh->SetMaterial(
////                    assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));
////
////            //auto mover = entity.AddComponent<MoveComponent>();
////        }
////
////        Game::Component<Game::TransformComponent> parent = nullptr;
////        {
////            Game::Entity entity;
////            Always(entitySystem->CreateEntity(entity));
////
////            auto transform = entity.AddComponent<Game::TransformComponent>();
////            transform->SetScale({0.05f, 0.05f, 0.05f});
////            transform->SetPosition({0, 1, 0});
////            transform->SetParent(parent);
////            parent = transform;
////        }
////
////        for (int i = 0; i < 20; i++) {
////            // Create entity
////            Game::Entity entity;
////            Always(entitySystem->CreateEntity(entity));
////
////            auto transform = entity.AddComponent<Game::TransformComponent>();
////            transform->SetPosition({0, 0.05, 0});
////            transform->SetParent(parent);
////            parent = transform;
////
////            auto mesh = entity.AddComponent<Game::MeshComponent>();
////            mesh->SetMesh(
////                    assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/cube.obj"_path));
////            mesh->SetMaterial(
////                    assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));
////
////            auto mover = entity.AddComponent<MoveComponenet>();
////        }
////
////        // Create dummy directional light
////        {
////            // Create entity
////            Game::Entity entity;
////            Always(entitySystem->CreateEntity(entity));
////
////            auto light = entity.AddComponent<Game::DirectionalLightComponent>();
////
////
////        }
////    }
//}

int TestStack() {
    return 5 + 5;
}

void _Main( Base::Application& app ) {
    std::raise( 11 );
}

int main() {
    Base::Launch( Base::ApplicationInfo().Name( "Linux.Video.Scene" ), Bindg( _Main ) );
    return 0;
}