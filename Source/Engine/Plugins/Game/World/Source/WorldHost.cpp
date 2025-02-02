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
 on 2/28/2018.
//

#include <Game/WorldHost.h>
#include <Host/IRegistry.h>
#include <Game/World.h>
#include <Game/IWorldPackage.h>
#include <Game/IWorldFeature.h>
#include <Base/DefaultLogs.h>
#include <Data/WorldConfig.h>
#include <Game/WorldFeature.h>

DJINN_NS2(Game);

WorldHost::WorldHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldHost(registry, outer, host) {

}

COM::Result WorldHost::Initialize() {
    // Create default feature
    IWorldFeature* feature;
    if (!CreateFeature(kDefaultFeature, &feature) || !Register(feature)) {
        return COM::kError;
    }

    // Create the execution pipeline
    if (!registry->CreateClass(this, &pipeline)) {
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");
    return COM::kOK;
}

COM::Result WorldHost::CreateWorld(Data::WorldConfig *config, IWorld** out) {
    // Diagnostic
    InfoLog(this).Write("World creation begun");

    // Create world
    World* world;
    if (!registry->CreateClass(this, &world)) {
        return COM::kError;
    }

    // Initialize the world
    if (!world->Initialize(config)) {
        return COM::kOK;
    }

    // Run all default features
    for (auto feature : features) {
        for (auto cid : feature->EnumeratePackages()) {
            Check(InitializePackage(world, cid));
        }
    }

    // Run all opt-in features
    for (auto featureID : config->features) {
        auto feature = featureMap.GetNullable(featureID);
        if (!feature) {
            ErrorLog(this).Write("Unregistered feature with id '", featureID.Get(), "'");
            continue;
        }

        for (auto cid : (*feature)->EnumeratePackages()) {
            Check(InitializePackage(world, cid));
        }
    }

    // Diagnostic
    InfoLog(this).Write("Created and initialized new world");

    // OK
    *out = world;
    return COM::kOK;
}

COM::Result WorldHost::LoadWorld(Data::IWorldAsset *asset, IWorld **out) {
    return COM::kError;
}

COM::Result WorldHost::SaveWorld(Data::IWorldAsset *asset, IWorld *world) {
    return COM::kError;
}


COM::Result WorldHost::EnumerateWorlds(UInt32 &count, IWorld **&out) {
    count = static_cast<UInt32>(worlds.Size());
    out = worlds.Ptr();
    return COM::kOK;
}

COM::Result WorldHost::Register(IWorld *world) {
    Check(pipeline->Register(world));

    worlds.Add(world);
    return COM::kOK;
}

COM::Result WorldHost::Deregister(IWorld *world) {
    Check(pipeline->Deregister(world));

    worlds.RemoveValue(world);
    return COM::kOK;
}

COM::Result WorldHost::Register(IWorldFeature *feature) {
    features.Add(feature);
    return COM::kOK;
}

COM::Result WorldHost::Deregister(IWorldFeature *feature) {
    features.RemoveValue(feature);
    return COM::kOK;
}

COM::Result WorldHost::InitializePackage(IWorld *world, const COM::ClassID &cid) {
    // Attempt to create package
    IWorldPackage* package;
    if (!registry->CreateClass(cid, world, &package)) {
        ErrorLog(this).Write("Failed to create package with class id '", cid.Get(), "'");
        return COM::kError;
    }

    // Attempt to initialize package
    if (!package->Initialize(world)) {
        ErrorLog(this).Write("Failed to initialize package [world] '", package->GetClassName(), "'");
        return COM::kError;
    }

    // ...
    world->Register(package);

    // OK
    return COM::kOK;
}

COM::Result WorldHost::CreateFeature(Data::WorldFeatureID id, IWorldFeature **out) {
    WorldFeature* feature;
    if (!registry->CreateClass(this, &feature)) {
        ErrorLog(this).Write("Failed to create feature");
        return COM::kError;
    }

    // Set map
    featureMap[id] = feature;

    // ...
    *out = feature;
    return COM::kOK;
}

IWorldFeature *WorldHost::GetFeature(Data::WorldFeatureID id) {
    auto feature = featureMap.GetNullable(id);
    if (!feature) {
        return nullptr;
    }
    return *feature;
}

Base::IPipeline *WorldHost::GetPipeline() {
    return pipeline;
}
