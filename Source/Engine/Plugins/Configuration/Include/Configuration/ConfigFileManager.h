// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Config files may be used in two ways.
//      1. Pass the path to the constructor, which will automatically trigger a parse and load.
//      2. Don't call the constructor, pass the path to the "Load()" function, which will trigger a parse and load.
//
//  Once loaded, the destructor will handle the freeing of the loaded data.  If the config file needs to be
//  unloaded before the object is destroyed, call "Unload().
//
//  "Ready()" can be called to check if the config file has been loaded and parsed.

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/HashMap.h>
#include <Core/String.h>
#include <Core/HashMap.h>
#include <Core/Singleton.h>
#include <Configuration/ConfigFile.h>

#include <COM/IUnknown.h>


DJINN_BEGIN_NS2( Djinn, Configuration )

using namespace Djinn::Core;

class Pipeline;

class DJINN_EXPORT_PLUGINS_CONFIGURATION  ConfigFileManager : public COM::IUnknown {
public:
    DJINN_COM_CLASS();

public:
    ConfigFileManager(Host::IRegistry *registry, IUnknown *outer, COM::IInstanceHost* host);

    void RegisterFile( const String& Name, const String& Path );
    void RegisterDirectory( const String& Path );

    ConfigFile* Find( const String& Name );

    // Class is implemented as a singleton... might be a good idea to flush anyways.
    void Flush();

private:
    HashMap< String, ConfigFile* > Cache;
};


DJINN_END_NS2( Djinn, Configuration )