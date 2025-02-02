//
// Created by Dev on 10/20/2017.
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