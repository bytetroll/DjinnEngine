//
// Created by Dev on 2/24/2018.
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
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/IUIElementComponent.h>
#include <UI/IUIElement.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Game/ISceneComponent.h>
#include <Game/ISceneActor.h>
#include <Game/ActorCallback.h>
#include <Game/Actors/Camera.h>
#include <Game/Actors/StaticMesh.h>
#include <Game/IActorSystem.h>
#include <Game/Actors/DirectionalLight.h>
#include <UI/Elements/UICanvas.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <UI/Elements/UILabel.h>
#include <UI/Elements/UICGMemory.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UIGraph.h>
#include <UI/Elements/UITextbox.h>
#include <UI/Elements/UISpline.h>
#include <UI/Elements/UIButton.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/ElementComponents/UIEffectComponent.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/Elements/UIFpsGraph.h>
#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UIFpsLabel.h>
#include <UI/IUIInputSystem.h>
#include <UI/Elements/UIWindow.h>
#include <UI/IUIMLHost.h>
#include <UI/Elements/TransformDebug.h>

DJINN_NS();

void _main(Base::Application &app) {
    srand(time(0));

    Core::Timer timer;

    // Load plugins
    Always(app.GetRegistry()->LoadPlugins());

    // Default pipeline should contain our interfaces
    auto pipeline = app.GetRegistry()->GetPipeline();

    // Interfaces
    Game::IGameSystem *gameSystem;
    Graphics::IGraphicsSystem *graphicsSystem;
    Game::IWorldHost *worldHost;
    Graphics::IOutputHost *outputHost;
    Data::IAssetHost *assetHost;
    Always(
            pipeline->GetInterface(&gameSystem) &&
            pipeline->GetInterface(&graphicsSystem) &&
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
    UI::IUIMLHost *uimlHost;
    Always(
            world->GetPipeline()->GetInterface(&actorSystem) &&
            world->GetPipeline()->GetInterface(&uiInputSystem) &&
            world->GetPipeline()->GetInterface(&uimlHost)
    );

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
        UI::TransformDebug *debug;
        Always(actorSystem->CreateActor(scene, &debug));
        //canvas->AddElement(debug);
    }

    {
        uimlHost->Build(assetHost->CompileAssetAsync<Data::IUIMLAsset>("Runtime/Editor.uiml"_path).Get(), COM::Cast<UI::UILayoutTransformComponent>(canvas->GetTransform()));
    }

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("UI Graphics Test"), Bindg(_main));
    return 0;
}