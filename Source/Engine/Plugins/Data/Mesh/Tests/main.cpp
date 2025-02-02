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
 on 10/22/2017.
//

#include <Core/Common.h>
#include <Core/Assert.h>
#include <Host/Registry.h>
#include <Core/IStream.h>
#include <Base/FileSystem.h>
#include <Data/AssetHost.h>
#include <Data/BaseMeshAsset.h>
#include <Serialization/SerializerHost.h>
#include <Serialization/SerializerRegistry.h>
#include <Serialization/ObjectSerializer.h>
#include <Data/BaseMeshSerializer.h>
#include <Platform/FileStream.h>
#include <Data/IMeshCompiler.h>
#include <Core/Timer.h>
#include <Data/MeshCollectionAsset.h>
#include <Data/BaseMeshAsset.h>

DJINN_NS2(Data);

Host::IRegistry* registry = new Host::Registry();
Data::AssetHost* assetHost;

void TestA() {
    // Create mesh asset
    Data::BaseMeshAsset* source;
    registry->CreateClass(nullptr, &source);

    // Load mesh data
    source->indexStream.indices = {
            0, 1, 2, 3
    };
    source->vertexStream.vertices = {
            BaseVertex {Math::Vector3(0, 1, 2), Math::Vector3(0, 1, 0), Math::Vector3(1, 0, 0), Math::Vector2(0, 0)},
            BaseVertex {Math::Vector3(0, 1, 2), Math::Vector3(0, 1, 0), Math::Vector3(1, 0, 0), Math::Vector2(0, 0)},
            BaseVertex {Math::Vector3(0, 1, 2), Math::Vector3(0, 1, 0), Math::Vector3(1, 0, 0), Math::Vector2(0, 0)},
            BaseVertex {Math::Vector3(0, 1, 2), Math::Vector3(0, 1, 0), Math::Vector3(1, 0, 0), Math::Vector2(0, 0)}
    };

    // Save asset
    Platform::FileStream outStream(Platform::FileMode::eWrite, "testAsset.dat");
    Always(assetHost->StoreAssetStream(&outStream, source));
    outStream.Close();

    // Load asset
    BaseMeshAsset* dest;
    Platform::FileStream inStream(Platform::FileMode::eRead, "testAsset.dat");
    Always(assetHost->LoadAsset(&inStream, reinterpret_cast<Data::IAsset**>(&dest)));
    inStream.Close();

    // Validate
    Always(dest->indexStream.indices.Size() == 4);
    Always(dest->indexStream.indices[0] == 0);
    Always(dest->indexStream.indices[1] == 1);
    Always(dest->indexStream.indices[2] == 2);
    Always(dest->indexStream.indices[3] == 3);
    Always(dest->vertexStream.vertices.Size() == 4);
}

void TestB() {
    IMeshCompiler* compiler;
    Always(registry->GetPipeline()->GetInterface(&compiler));

    Platform::FileStream stream(Platform::FileMode::eRead, "Data/sponza.obj");

    Core::Timer timer;

    MeshConfiguration info;
    info.format = MeshSourceFormat ::eObj;
    info.includePath = "Data";

    MeshCollectionAsset* asset;
    Always(compiler->Compile(&stream, &info, reinterpret_cast<IAsset**>(&asset)));

    std::cout << "Sponza import: " << timer.Delta() << "s\n";

    Platform::FileStream outStream(Platform::FileMode::eWrite, "testOut.dat");
    Always(assetHost->StoreAssetStream(&outStream, asset));
    outStream.Close();

    timer.Delta();
    MeshCollectionAsset* dest;
    Platform::FileStream inStream(Platform::FileMode::eRead, "testOut.dat");
    Always(assetHost->LoadAsset(&inStream, reinterpret_cast<IAsset**>(&dest)));
    inStream.Close();

    std::cout << "Sponza asset import: " << timer.Delta() << "s\n";
}

int main() {
    Always(registry->LoadPlugin("Plugins.Base.Logging", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Base.FileSystem", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Reflection", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Serialization", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Data.Asset", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Data.Mesh", Host::LoadFlag::eAll));

    Always(registry->GetPipeline()->GetInterface(&assetHost));

    //TestA();
    TestB();

    //Core::IStream* stream = fs->GetStream("shaders"_path, "gipfx.dsf");
    return 0;
}
