//
// Created by nathan on 1/12/21.
//

#include <Host/IPipeline.h>

#include "Code/Common.h"
#include "TetrisHost.h"
#include "TetrisHostContext.h"


TetrisHostContext* TetrisHost::Create(Application* app) {
    // @TODO: Move initialization shit into the context class.
    TetrisHostContext* context = new TetrisHostContext;
    context->app = app;
    context->timer = new Timer;

    context->pipeline = app.GetRegistry()->GetPipeline();

    // Interfaces.
    Always(
        context->pipeline->GetInterface(&context->game) &&
        context->pipeline->GetInterface(&context->video) &&
        context->pipeline->GetInterface(&context->input) &&
        context->pipeline->GetInterface(&context->world) &&
        context->pipeline->GetInterface(&context->output) &&
        context->pipeline->GetInterface(&context->asset)
    );

    // Basic setting initialization.
    context->game->SetMaxIPS(300);

    context->config = new WorldConfig;
    Always(context->app->GetRegistry()->CreateClass(&context->app, &context->config));
    Always(context->worldHost->CreateWorld(context->config, &context->world));
    Always(context->worldHost->Register(context->world));
    Always(context->world->CreateScene(&context->scene));
    Always(context->world->GetPipeline()->GetInterface(&context->actor) &&
           context->world->GetPipeline()->GetInterface(&context->input));

    // Input system mappings here.
    Always(app.GetRegistry()->CreateClass(&context->app, &context->sceneOutput));
    Always(context->sceneOutput->Initialize(
        GHA::Size(Platform::User::GetPrimaryDisplayWidth(),
             Platform::User::GetPrimaryDisplayHeight())
        ));
    Always(context->output->Register(context->sceneOutput));

    // Setup the global camera.
    Game::Location* location;
    Always(context->actor->CreateActor(context->scene, &location));

}

void TetrisHost::Free(TetrisHostContext *context) {
    delete context->pipeline;
    delete context->timer;
    // delete context->app;


    // These need to be deleted last in the defined order.
    delete context->config;
    delete context;
}
