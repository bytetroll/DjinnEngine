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
 on 10/20/2017.
//

#include <Serialization/SerializerHost.h>
#include <Host/Registry.h>
#include <Serialization/SerializerRegistry.h>
#include <Serialization/ISerializer.h>
#include <Serialization/ObjectSerializer.h>
#include <Platform/FileStream.h>
#include <Core/Assert.h>
#include <Core/Test.h>
#include <Reflection/ReflectionRegistry.h>
#include <Core/Timer.h>
#include "Classes.h"
#include <Core/Literals.h>
#include <Reflection/IClassInfo.h>
#include <iostream>

DJINN_NS2(Serialization);

int main() {
    Host::IRegistry* registry = new Host::Registry();
    Always(registry->LoadPlugins());
    Always(registry->InstallClass<Foo>());
    Always(registry->InstallClass<Boo>());

    ISerializerRegistry* serializerRegistry;
    Always(registry->GetPipeline()->GetInterface(&serializerRegistry));

    ISerializerHost* host;
    Always(registry->GetPipeline()->GetInterface(&host));

    Always(serializerRegistry->AddSerializer(Foo::kCID, ObjectSerializer::kCID));
    Always(serializerRegistry->AddSerializer(Boo::kCID, ObjectSerializer::kCID));

    // Foo test
    {
        Core::Timer timer;

        int count = 1'000'000;

        Core::Array<Foo*> foos;
        for (int i = 0; i < count; i++) {
            Foo* source = new Foo(registry, nullptr, nullptr);
            for (UInt32 j = 0; j < 50; j++) {
                source->a[j] = 123;
            }
            source->b = 'b';
            source->c = 5.3f;
            foos.Add(source);
        }

        UInt64 cacheSize = 10_mb;

        timer.Delta();
        // Serialize sources
        {
            Platform::FileStream stream(Platform::FileMode::eWrite, "foo.blob", cacheSize);

            Archive archive(host, &stream);

            for (int i = 0; i < count; i++) {
                Always(host->Serialize(archive, foos[i]));
            }
        }
        double elapsedSer = timer.Delta();

        Core::Array<Foo*> dests;
        for (int i = 0; i < count; i++) {
            dests.Add(nullptr);
        }

        timer.Delta();
        // Deserialize source
        {
            Platform::FileStream stream(Platform::FileMode::eRead, "foo.blob", cacheSize);

            Archive archive(host, &stream);

            for (int i = 0; i < count; i++) {
                COM::IUnknown* obj;
                Always(host->Deserialize(archive, &obj));

                Always((dests[i] = COM::Cast<Foo>(obj)) != nullptr);

                Always(dests[i] != nullptr);
                for (UInt32 j = 0; j < 50; j++) {
                    Always(dests[i]->a[j] == 123);
                }
                Always(dests[i]->b == 'b');
                Always(dests[i]->c == 5.3f);
            }
        }
        double elapsedDer = timer.Delta();

        std::cout << "Serialization " << elapsedSer << ", deserialization " << elapsedDer << ", total: " << (elapsedSer + elapsedDer) << "\n";
    }

    return 0;
}