//
// Created by Dev on 5/13/2018.
//

#include <Base/MemoryStatistics.h>
#include <Core/Common.h>
#include <Core/IStream.h>
#include <Core/StringStream.h>
#include <Base/Detail/PluginMemory.h>

DJINN_NS2(Base);

void Memory::PrettyPrint(Core::IStream* stream) {
    Core::StringStream ss(stream);

    ss.Write("Memory pretty print\n");

    USize size = 0;
    for (auto memory : Detail::pluginMemory) {
        ss.Write("\t");
        ss.Write(memory->library.GetName());
        ss.Write(" - ");
        ss.Write(Core::String(memory->allocated.Load() / 1e6));
        ss.Write("mb\n");

        size += memory->allocated.Load();
    }

    ss.Write("Total tracked: ");
    ss.Write(Core::String(size / 1e6));
    ss.Write("mb\n");
}