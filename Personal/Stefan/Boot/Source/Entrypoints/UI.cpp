#include <Base/Application.h>
#include <Base/DefaultLogs.h>
#include <Data/WorldConfig.h>
#include <Game/IWorld.h>
#include <Game/IWorldHost.h>
#include <Game/IActorSystem.h>
#include <UI/IUICanvas.h>
#include <Graphics/ScreenOutput.h>
#include <Graphics/IOutputHost.h>
#include <UI/IUIInputSystem.h>
#include <UI/Elements/UILabel.h>
#include <UI/IUIMLHost.h>
#include <Data/IAssetHost.h>

#include "TestFiles/MyComponent.h"
#include "TestFiles/MyWindow.h"

DJINN_NS();

/** Notes
 * - The use of Always is discouraged, it's just very convinient for samples.
 * - Lots of this code is setting of the engine itself, only a small part is UI stuff :)
 */

Game::IWorld* CreateWorld(Base::Application &app) {
    Host::IRegistry* registry = app.GetRegistry();
    Host::IPipeline* pipeline = registry->GetPipeline();

    // Get the world worldHost
    // It's basically a container of all the active worlds
    Game::IWorldHost *worldHost;
    Always(pipeline->GetInterface(&worldHost));

    // Create the world configuration
    // This is basically initialization parameters, for the most part we dont care
    Data::WorldConfig *config;
    Always(registry->CreateClass(&app, &config));

    // Create the world
    Game::IWorld *world;
    Always(worldHost->CreateWorld(config, &world));

    // Register this world as active
    Always(worldHost->Register(world));

    return world;
}

UI::IUICanvas* CreateAndSetCanvas(Base::Application& app, Game::IWorld* world) {
    Host::IRegistry* registry = app.GetRegistry();
    Host::IPipeline* pipeline = registry->GetPipeline();

    // Woah, we're getting the pipeline of a world? Why not he registry?
    // Since each world may want a unique instance of a system (like actor systems!), they worldHost an internal pipeline
    // This is a neat way to have global like systems without the usual downsides.
    Host::IPipeline* worldPipeline = world->GetPipeline();

    // Get the output worldHost
    // This is a graphical thing, consider them targets that can be rendered to
    Graphics::IOutputHost *outputHost;
    Always(pipeline->GetInterface(&outputHost));

    // Create canvas
    UI::IUICanvas *canvas;
    Always(world->CreateScene(nullptr, &canvas));

    // Create a standard output, something we can render to
    Graphics::ScreenOutput *output;
    Always(app.GetRegistry()->CreateClass(&app, &output));
    Always(output->Initialize());

    // Register our output and mark it as the primary output (i.e. window)
    Always(outputHost->Register(output));
    Always(outputHost->SetOutput(output));

    // Tell the canvas to automatically resize to the output dimensions
    canvas->SetScaling(true);
    canvas->SetOutput(output);

    // Get the UI input system
    UI::IUIInputSystem *input;
    Always(worldPipeline->GetInterface(&input));

    // Tell the input system to route events to our canvas
    input->Register(canvas);

    return canvas;
}

void CreateElementsInCode(Base::Application& app, UI::IUICanvas* canvas) {
    // Get world pipeline
    Host::IPipeline* pipeline = canvas->GetWorld()->GetPipeline();

    // Get the actor system
    // Currently UI elements are actors, I'm not sure if this is the best design choice... Early days
    Game::IActorSystem *actorSystem;
    Always(pipeline->GetInterface(&actorSystem));

    {
        UI::UILabel* label;
        Always(actorSystem->CreateActor(canvas, &label));

        label->SetLineSeperation(3.0f);
        label->SetFontSize(20.0f);
        label->SetText("This is a label.");

        auto transform = label->GetTransform();
        transform->SetMargin({100, 150});

        canvas->AddElement(label);
    }
}

void CreateElementsInUIML(Base::Application& app, UI::IUICanvas* canvas) {
    // Get world pipeline
    Host::IPipeline* pipeline = canvas->GetWorld()->GetPipeline();

    // Get the actor system
    Game::IActorSystem *actorSystem;
    Always(pipeline->GetInterface(&actorSystem));

    {
        MyWindow* window;
        Always(actorSystem->CreateActor(canvas, &window));

        auto transform = window->GetTransform();
        transform->SetMargin({100, 250});
        transform->SetDesiredSize({300, 350});

        canvas->AddElement(window);
    }
}

void EntryPointUI(Base::Application& app) {
    InfoLog(&app).Write("[UI]");

    // Load everything else
    // Note the first parameter is defaulted to:
    //  const Core::StringView &pattern = "Plugins.*.dll"
    //  All plugins must follow the Plugins.* naming convention!
    CheckVoid(app.GetRegistry()->LoadPlugins());

    // The world is essentially a container where systems can work
    // This can be graphics, scenes, UI, etc...
    Game::IWorld* world = CreateWorld(app);

    // Next up we create the canvas, this is where UI elements are hosted
    UI::IUICanvas *canvas = CreateAndSetCanvas(app, world);

    // Create elements by code
    CreateElementsInCode(app, canvas);

    // Create elements by UIML
    CreateElementsInUIML(app, canvas);
}
