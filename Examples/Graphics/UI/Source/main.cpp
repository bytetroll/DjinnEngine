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
#include <UI/IUICanvas.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <UI/Elements/UILabel.h>
#include <UI/Elements/UICGMemory.h>
#include <UI/Elements/CallGraph.h>
#include <UI/Elements/UIImage.h>
#include <UI/Elements/UIGraph.h>
#include <UI/Elements/UITextbox.h>
#include <UI/Elements/UISpline.h>
#include <UI/Elements/UIButton.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/ElementComponents/UIEffectComponent.h>
#include <UI/Elements/UIFpsGraph.h>
#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UIFpsLabel.h>
#include <UI/IUIInputSystem.h>
#include <UI/Elements/UIWindow.h>
#include <UI/Elements/TransformDebug.h>
#include <UI/Elements/UIDiv.h>
#include <UI/Elements/UITable.h>
#include <UI/Elements/UICheckbox.h>
#include <UI/Elements/UISlider.h>
#include <Game/ISpatialScene.h>

DJINN_NS();

class UIMoveComponent : public UI::IUIElementComponent {
public:
    UIMoveComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

    }

    void StaticInitialize() override {
        callback.SetCallback(Bindff(Update));
        callback.Register(this);
    }

    void Update(double delta) {
        GetElement()->GetTransform()->SetUserRotation(Math::Quaternion({0, 0, 1}, static_cast<float>(elapsed)));
        elapsed += delta;
    }

private:
    Game::ActorCallback callback;
    double elapsed = 0;
};

class MoveComponent : public Game::ISceneComponent {
public:
    MoveComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneComponent(registry, outer, host) {

    }

    void StaticInitialize() override {
        callback.SetCallback(Bindff(Update));
        callback.Register(this);
    }

    void Update(double delta) {
        GetSceneActor()->GetTransform()->SetUserTransform(Math::Matrix4::RotateRPY({0, 0, static_cast<float>(elapsed)}));
        elapsed += delta;
    }

private:
    Game::ActorCallback callback;
    double elapsed = 0;
};

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

    // World interfaces
    Game::IActorSystem *actorSystem;
    UI::IUIInputSystem *uiInputSystem;
    Always(
            world->GetPipeline()->GetInterface(&actorSystem) &&
            world->GetPipeline()->GetInterface(&uiInputSystem)
    );

    // Create scene
    Game::ISpatialScene *spatialScene;
    Always(world->CreateScene(nullptr, &spatialScene));

    // Create canvas
    UI::IUICanvas *canvas;
    Always(world->CreateScene(nullptr, &canvas));

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

    // Prepare dummy canvas
    Graphics::Output *sceneOutput;
    {
        Always(app.GetRegistry()->CreateClass(&app, &sceneOutput));
        Always(sceneOutput->Initialize(GHA::Size(
                Platform::User::GetPrimaryDisplayWidth(),
                Platform::User::GetPrimaryDisplayHeight()
        )));
        Always(outputHost->Register(sceneOutput));

        // Create camera
        Game::Camera *camera;
        Always(actorSystem->CreateActor(spatialScene, &camera));
        {
            camera->SetOutput(sceneOutput);
            camera->Lookat({5, 5, 5}, {0, 0, 0});
        }

        // Create mesh
        Game::StaticMesh *mesh;
        Always(actorSystem->CreateActor(spatialScene, &mesh));
        {
            mesh->SetMaterial(assetHost->CompileAssetAsync<Data::IMaterialAsset>("Runtime/Engine/Materials/Dummy.dmf"_path));
            mesh->SetMesh(assetHost->CompileAssetAsync<Data::MeshCollectionAsset>("Runtime/Engine/Models/cube.obj"_path));
        }

        // Create directional light
        Game::DirectionalLight *light;
        Always(actorSystem->CreateActor(spatialScene, &light));
    }

    {
        UI::UIOutputDisplay* display;
        Always(actorSystem->CreateActor(canvas, &display));

        display->SetOutput(sceneOutput);

        auto transform = display->GetTransform();
        transform->SetMargin({730, 350});
        transform->SetDesiredSize({170, 100});

        canvas->AddElement(display);
    }

    {
        UI::UILabel* label;
        Always(actorSystem->CreateActor(canvas, &label));

        label->SetLineSeperation(3.0f);
        label->SetFontSize(20.0f);
        label->SetText(R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis vitae mi dui. Proin a tristique lectus, id dapibus mi.
Donec vel diam quis neque ultrices ornare ac eget metus. Morbi rutrum erat et nisi molestie,
eget facilisis lectus lobortis. Nulla varius hendrerit nunc at pretium. Suspendisse egestas elit vitae metus
faucibus accumsan. Mauris a erat eros. Morbi tristique nibh at tellus venenatis sagittis. Vivamus sollicitudin mollis pharetra.
Integer condimentum nulla quis risus commodo, sed vestibulum dolor tincidunt. Nunc nunc velit,
bibendum luctus semper id, convallis eu augue. Nulla vitae nunc a felis imperdiet maximus vitae ut metus.)");

        auto transform = label->GetTransform();
        transform->SetMargin({100, 150});

        canvas->AddElement(label);
    }

    {
        UI::UILabel* label;
        Always(actorSystem->CreateActor(canvas, &label));

        label->SetText("No...");

        auto transform = label->GetTransform();
        transform->SetMargin({100, 100});

        canvas->AddElement(label);
    }

    {
        gameSystem->EnableAsyncRecording(true);

        UI::CallGraph* callstack;
        Always(actorSystem->CreateActor(canvas, &callstack));

        auto transform = callstack->GetTransform();
        transform->SetMargin({15, 15});
        transform->SetDesiredSize({0, 100});
        transform->SetHorizontalScaling(UI::UIScaling::eFill);

        canvas->AddElement(callstack);
    }

    {
        UI::UICGMemory* cgmem;
        Always(actorSystem->CreateActor(canvas, &cgmem));

        auto transform = cgmem->GetTransform();
        transform->SetMargin({100, 800});

        canvas->AddElement(cgmem);
    }

    {
        UI::UIImage* img;
        Always(actorSystem->CreateActor(canvas, &img));

        img->SetMarginGeometry({4, 4});

        auto transform = img->GetTransform();
        transform->SetMargin({330, 330});
        transform->SetDesiredSize({50, 50});

        canvas->AddElement(img);
    }

    {
        UI::UIGraph* graph;
        Always(actorSystem->CreateActor(canvas, &graph));

        graph->SetGridUnitSnap(10.0f);

        for (UInt32 k = 0; k < 4; k++) {
            float scale = Math::RandFFast(1.f, 2.f);

            // Properties
            UI::UIPlotProperties properties;
            properties.brush = Graphics::UIBrushGradient(
                    {0, 0},
                    {0, 1, 0, 1},
                    {0, 1},
                    {1, 0, 0, 1}
            );
            switch (k) {
                case 0:
                    properties.brush = Graphics::UIBrushSolid({56 / 255.f, 66 / 255.f, 93 / 255.f, 1});
                    break;
                case 1:
                    properties.brush = Graphics::UIBrushSolid({197 / 255.f, 55 / 255.f, 71 / 255.f, 1});
                    break;
                case 2:
                    properties.brush = Graphics::UIBrushSolid({191 / 255.f, 219 / 255.f, 56 / 255.f, 1});
                    break;
                case 3:
                    properties.brush = Graphics::UIBrushSolid({54 / 255.f, 182 / 255.f, 59 / 255.f, 1});
                    break;
            }

            // Prepare plot
            auto data = graph->MapPlotData(graph->AddPlot(properties));
            for (UInt32 i = 0; i < 30; i++) {
                data->data.Add({static_cast<float>(i), Math::RandFFast(20.0f, 200.0f) * scale});
            }
            graph->UnmapPlotData(data);
        }

        auto transform = graph->GetTransform();
        transform->SetMargin({530, 530});
        transform->SetDesiredSize({200, 100});

        canvas->AddElement(graph);
    }

    {
        UI::UITextbox* textbox;
        Always(actorSystem->CreateActor(canvas, &textbox));

        auto transform = textbox->GetTransform();
        transform->SetMargin({830, 630});

        canvas->AddElement(textbox);
    }

    {
        UI::UISpline* spline;
        Always(actorSystem->CreateActor(canvas, &spline));

        spline->SetSubdivisions(20);

        auto points = spline->MapControlPoints();
        points->Clear();
        points->Add({0, 0});
        points->Add({50, 40});
        points->Add({150, -100});
        points->Add({200, 0});
        spline->UnmapControlPoints();


        auto transform = spline->GetTransform();
        transform->SetMargin({830, 730});

        canvas->AddElement(spline);
    }

    {
        UI::UIButton* button;
        Always(actorSystem->CreateActor(canvas, &button));
        button->SetName("test");

        auto effects = button->CreateComponent<UI::UIEffectComponent>();

        button->GetFirstComponent<UI::UIInputComponent>()->mousePressed.Add(button, [=](const Game::InputEvent::MousePressed &event, UI::UIInputActionBuffer *actions) {
            effects->Set(UI::UIEffect::Stretch(10.0f, 5.0f));
        });

        auto transform = button->GetTransform();
        transform->SetMargin({830, 470});

        canvas->AddElement(button);
    }

    {
        UI::UIButton* button;
        Always(actorSystem->CreateActor(canvas, &button));

        auto effects = button->CreateComponent<UI::UIEffectComponent>();

        button->GetFirstComponent<UI::UIInputComponent>()->mousePressed.Add(button, [=](const Game::InputEvent::MousePressed &event, UI::UIInputActionBuffer *actions) {
            effects->Set(UI::UIEffect::Wiggle(1.0f));
        });

        auto transform = button->GetTransform();
        transform->SetMargin({830, 530});

        canvas->AddElement(button);
    }

    {
        UI::UIButton* button;
        Always(actorSystem->CreateActor(canvas, &button));

        auto effects = button->CreateComponent<UI::UIEffectComponent>();
        effects->Add(UI::UIEffect::Rotate(0, 0.45f));

        button->GetFirstComponent<UI::UIInputComponent>()->mousePressed.Add(button, [=](const Game::InputEvent::MousePressed &event, UI::UIInputActionBuffer *actions) {
            effects->Add(UI::UIEffect::Bubble(1.0f));
        });

        auto transform = button->GetTransform();
        transform->SetMargin({950, 530});

        canvas->AddElement(button);
    }

    {
        UI::UIFpsGraph* fpsGraph;
        Always(actorSystem->CreateActor(canvas, &fpsGraph));

        auto transform = fpsGraph->GetTransform();
        transform->SetMargin({530, 730});
        transform->SetDesiredSize({200, 100});

        canvas->AddElement(fpsGraph);
    }

    {
        UI::UIFpsLabel* fpsLabel;
        Always(actorSystem->CreateActor(canvas, &fpsLabel));

        auto transform = fpsLabel->GetTransform();
        transform->SetMargin({100, 850});

        canvas->AddElement(fpsLabel);
    }

    if(0) {
        UI::TransformDebug *debug;
        Always(actorSystem->CreateActor(canvas, &debug));
        canvas->AddElement(debug);
    }

    {
        UI::UICGTime* cgTime;
        Always(actorSystem->CreateActor(canvas, &cgTime));

        auto transform = cgTime->GetTransform();
        transform->SetMargin({100, 550});

        canvas->AddElement(cgTime);
    }

    {
        UI::UIWindow* window;
        Always(actorSystem->CreateActor(canvas, &window));

        auto transform = window->GetTransform();
        transform->SetMargin({300, 450});
        transform->SetDesiredSize({200, 300});

        canvas->AddElement(window);

        {
            UI::UITable* table;
            Always(actorSystem->CreateActor(canvas, &table));

            table->Configure(4, 2);
            table->SetColumnScaling(0, UI::UIScaling::eFill);
            table->SetColumnScaling(1, UI::UIScaling::eFill);
            table->SetRowScaling(0, UI::UIScaling::eContent);
            table->SetRowScaling(1, UI::UIScaling::eContent);
            table->SetRowScaling(2, UI::UIScaling::eContent);
            table->SetRowScaling(3, UI::UIScaling::eContent);

            auto transform = table->GetTransform();
            transform->SetHorizontalScaling(UI::UIScaling::eFill);
            window->AddChild(transform);

            {
                UI::UILabel* label;
                Always(actorSystem->CreateActor(canvas, &label));
                label->SetText("Name:");
                label->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eLeft);
                label->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});
                table->SetCell(0, 0, label->GetTransform());

                UI::UITextbox* textbox;
                Always(actorSystem->CreateActor(canvas, &textbox));
                textbox->SetHint("Name...");
                textbox->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                textbox->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eRight);
                table->SetCell(0, 1, textbox->GetTransform());
            }

            {
                UI::UILabel* label;
                Always(actorSystem->CreateActor(canvas, &label));
                label->SetText("Button:");
                label->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eLeft);
                label->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});
                table->SetCell(1, 0, label->GetTransform());

                UI::UIButton* button;
                Always(actorSystem->CreateActor(canvas, &button));
                button->SetText("Click Me");
                button->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                button->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eRight);
                table->SetCell(1, 1, button->GetTransform());
            }

            {
                UI::UILabel* label;
                Always(actorSystem->CreateActor(canvas, &label));
                label->SetText("Slider:");
                label->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eLeft);
                label->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});
                table->SetCell(2, 0, label->GetTransform());

                UI::UISlider* slider;
                Always(actorSystem->CreateActor(canvas, &slider));
                slider->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                slider->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eRight);
                table->SetCell(2, 1, slider->GetTransform());
            }

            {
                UI::UILabel* label;
                Always(actorSystem->CreateActor(canvas, &label));
                label->SetText("Checkbox:");
                label->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eLeft);
                label->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});
                table->SetCell(3, 0, label->GetTransform());

                UI::UICheckbox* checkbox;
                Always(actorSystem->CreateActor(canvas, &checkbox));
                checkbox->GetTransform()->SetVerticalAlignment(UI::UIVerticalAlignment::eCenter);
                checkbox->GetTransform()->SetHorizontalAlignment(UI::UIHorizontalAlignment::eRight);
                table->SetCell(3, 1, checkbox->GetTransform());
            }
        }
    }

    /*{
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto window = entity.AddComponent<Game::UIWindowComponent>();

        entity.GetFirstComponent<Game::UITransformComponent>()->SetMargin({1100, 300});

        auto body = window->bodyTransform;

        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create table
            auto table = entity.AddComponent<Game::UITableComponent>();
            table->Configure(4, 2);

            auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
            transform->SetHorizontalScaling(Game::UIScaling::eFill);
            transform->SetParent(body);

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create label component
                auto label = entity.AddComponent<Game::UILabelComponent>();
                label->SetText("Name:");
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(0, 0));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create textbox component
                auto textbox = entity.AddComponent<Game::UITextboxComponent>();
                textbox->SetText("Text");

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetHorizontalScaling(Game::UIScaling::eFill);
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(0, 1));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create label component
                auto label = entity.AddComponent<Game::UILabelComponent>();
                label->SetText("Button:");
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(1, 0));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                auto button = entity.AddComponent<Game::UIButtonComponent>();

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetHorizontalScaling(Game::UIScaling::eFill);
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(1, 1));

                auto effects = entity.AddComponent<Game::UIEffectComponent>();

                button->input->mousePressed.AddFree([=](const Game::InputEvent::MousePressed &event, Game::UIInputActionBuffer *actions) {
                    effects->Set(Game::UIEffect::Wiggle(1.0f));
                });
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create label component
                auto label = entity.AddComponent<Game::UILabelComponent>();
                label->SetText("Slider:");
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(2, 0));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                entity.AddComponent<Game::UISliderComponent>();

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetHorizontalScaling(Game::UIScaling::eFill);
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(2, 1));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create label component
                auto label = entity.AddComponent<Game::UILabelComponent>();
                label->SetText("Checkbox:");
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(3, 0));
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                entity.AddComponent<Game::UICheckboxComponent>();

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(table->GetCell(3, 1));
            }
        }


        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create listbox
            auto listbox = entity.AddComponent<Game::UIListboxComponent>();

            auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
            transform->SetHorizontalScaling(Game::UIScaling::eFill);
            transform->SetParent(body);

            listbox->CreateItems(500);

            for (USize i = 0; i < listbox->GetItemCount(); i++) {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                // Create label component
                auto label = entity.AddComponent<Game::UILabelComponent>();
                label->SetText("Gay Frogs " + Core::String(i));
                label->SetColor({0.3f, 0.3f, 0.3f, 1.0f});

                auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
                transform->SetVerticalAlignment(Game::UIVerticalAlignment::eCenter);
                transform->SetParent(listbox->GetItem(i));
            }
        }

        // Set canvas
        entity.GetFirstComponent<Game::UITransformComponent>()->SetParent(root);
    }


    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto window = entity.AddComponent<Game::UIWindowComponent>();
        window->SetTitle("Node Graph");

        entity.GetFirstComponent<Game::UITransformComponent>()->SetMargin({1350, 280});
        entity.GetFirstComponent<Game::UITransformComponent>()->SetDesiredSize({400, 300});

        auto body = window->bodyTransform;

        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create node graph
            auto graph = entity.AddComponent<Game::UINodeGraphComponent>();

            Game::Component<Game::UINodeSocketComponent> out;
            Game::Component<Game::UINodeSocketComponent> in;

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                auto node = entity.AddComponent<Game::UINodeComponent>();
                node->SetName("Lambert");
                node->AddInputSocket("Color");
                node->AddInputSocket("Normal");
                out = node->AddOutputSocket("Diffuse");

                entity.GetFirstComponent<Game::UITransformComponent>()->SetMargin({50, 50});

                // Set graph
                node->SetGraph(graph);
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                auto node = entity.AddComponent<Game::UINodeComponent>();
                node->SetName("Output");
                in = node->AddInputSocket("Diffuse");

                entity.GetFirstComponent<Game::UITransformComponent>()->SetMargin({200, 90});

                // Set graph
                node->SetGraph(graph);
            }

            {
                // Create entity
                Game::Entity entity;
                Always(entitySystem->CreateEntity(entity));

                auto node = entity.AddComponent<Game::UINodeConnectionComponent>();
                node->SetSocket(in, out);

                // Set graph
                node->SetGraph(graph);
            }

            // Set canvas
            auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
            transform->SetVerticalScaling(Game::UIScaling::eFill);
            transform->SetHorizontalScaling(Game::UIScaling::eFill);
            transform->SetParent(body);
        }

        // Set canvas
        entity.GetFirstComponent<Game::UITransformComponent>()->SetParent(root);
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        // Create menu bar
        auto bar = entity.AddComponent<Game::UIMenuBarComponent>();

        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create menu item
            auto item = entity.AddComponent<Game::UIMenuItemComponent>();
            item->SetText("File");
            bar->AddItem(entity.GetFirstComponent<Game::UITransformComponent>());
        }

        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create menu item
            auto item = entity.AddComponent<Game::UIMenuItemComponent>();
            item->SetText("Edit");
            bar->AddItem(entity.GetFirstComponent<Game::UITransformComponent>());
        }

        {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            // Create menu item
            auto item = entity.AddComponent<Game::UIMenuItemComponent>();
            item->SetText("Tools");
            bar->AddItem(entity.GetFirstComponent<Game::UITransformComponent>());
        }

        // Set canvas
        entity.GetFirstComponent<Game::UITransformComponent>()->SetParent(root);
    }

    {
        // Create entity
        Game::Entity entity;
        Always(entitySystem->CreateEntity(entity));

        auto treeView = entity.AddComponent<Game::UITreeViewComponent>();

        for (int i = 0; i < 10; i++) {
            // Create entity
            Game::Entity entity;
            Always(entitySystem->CreateEntity(entity));

            auto treeItem = entity.AddComponent<Game::UITreeItemComponent>();

            auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
            transform->SetParent(treeView->content);
        }

        auto transform = entity.GetFirstComponent<Game::UITransformComponent>();
        transform->SetMargin({1200, 750});
        transform->SetParent(root);
    }*/

    // Done loading
    InfoLog(app.GetRegistry()).Write("Loading took ", timer.Delta(), "s");
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("UI Graphics Test"), Bindg(_main));
    return 0;
}