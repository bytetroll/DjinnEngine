// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#include <Configuration/ConfigFileManager.h>

#include <Core/RuntimeException.h>
#include <Core/ExceptionFormatter.h>
#include <Core/Console.h>
#include <Platform/FileSystem.h>
#include <Core/Array.h>
#include <Host/Pipeline.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>

DJINN_NS2(Configuration);

ConfigFileManager::ConfigFileManager(Host::IRegistry *registry, IUnknown *outer, COM::IInstanceHost *host) : IUnknown(registry, outer, host) {
    registry->GetPipeline()->AddClass(this);
}

void ConfigFileManager::Flush() {
    for (auto Iter = Cache.begin(); Iter != Cache.end(); ++Iter) {
        delete (*Iter).second;
    }
}

void ConfigFileManager::RegisterFile(const String &Name, const String &Path) {
    ConfigFile *const File = new ConfigFile(Path);
    Cache.Add(Name, File);
}

void ConfigFileManager::RegisterDirectory(const Djinn::Core::String &Path) {
    Array<String> Files;
    FileSystem::Glob(Files, Path);

    try {
        if (Files.Size() != 0) {
            for (auto Iter = Files.begin(); Iter != Files.end(); ++Iter) {
                // @TODO:
                // Name of the file is the name of the file.
                Debug::Console::Print((*Iter).AsCharArray());

                const String Path = (*Iter);
                const String FileName = Path.Substring(static_cast<UInt32>(Path.LastIndexOf('/')), static_cast<UInt32>(Path.LastIndexOf('.')));

                ConfigFile *Config = new ConfigFile(Path);
                Cache.Add(Path, Config);
            }
        } else {
            DJINN_THROW Debug::RuntimeException(Debug::ExceptionFormatter() << "Failed to parse config files in directory" << Path);
        }
    } catch (const std::exception &Except) {
        Debug::Console::Print(Except.what());
    }
}

ConfigFile *ConfigFileManager::Find(const Djinn::Core::String &Name) {
    for (auto Iter = Cache.begin(); Iter != Cache.end(); ++Iter) {
        if ((*Iter).first == Name) {
            return (*Iter).second;
        }
    }

    return nullptr;
}
