//
// Created by Dev on 10/22/2017.
//

#include <Core/Common.h>
#include <Core/Assert.h>
#include <Host/Registry.h>
#include <Core/IStream.h>
#include <Base/FileSystem.h>
#include <Base/FileHandle.h>

DJINN_NS2(Base);

int main() {
    Host::IRegistry *registry = new Host::Registry();
    Always(registry->LoadPlugin("Plugins.Base.Logging", Host::LoadFlag::eAll));
    Always(registry->LoadPlugin("Plugins.Base.FileSystem", Host::LoadFlag::eAll));

    FileSystem* fs;
    Always(registry->GetPipeline()->GetInterface(&fs));

    FileHandle handle;
    Always(fs->OpenStream("Plugins.Base.Logging.dll"_path, Platform::FileMode::eRead, &handle));

    bool b = false;
}
