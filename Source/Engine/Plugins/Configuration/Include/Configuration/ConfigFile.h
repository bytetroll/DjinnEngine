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

#include <Platform/FileAPI.h>
#include <Platform/FileStream.h>
#include <Platform/FileSystem.h>


DJINN_BEGIN_NS2( Djinn, Configuration )

using namespace Djinn::Core;
using namespace Djinn::Platform;

class DJINN_EXPORT_PLUGINS_CONFIGURATION ConfigFile {
private:
    struct ConfigFileBlock {
    public:
        ConfigFileBlock() {
            Section = "";
            Pairs.Clear();
        }

        ConfigFileBlock( const String& Section ) : Section( Section ) {
            Pairs.Clear();
        }

        ~ConfigFileBlock() {
            Pairs.Clear();
        }

        void AddPair( const String& Key, const String& Value ) {
            Pairs.Add( Key, Value );
        }

    public:
        String Section;
        HashMap< String, String > Pairs;
    };

public:
    ConfigFile();
    ConfigFile( const String& Path );
    ~ConfigFile();

    void Load( const String& Path );
    void Unload();

    DJINN_INLINE bool Ready() const {
        return IsLoaded;
    }

    bool ReadBool( const String& Section, const String& Key, const bool DefaultValue ) const;
    void WriteBool( const String& Section, const String& Key, const bool Value );

    Int32 ReadInt( const String& Section, const String& Key, const Int32 DefaultValue ) const;
    void WriteInt( const String& Section, const String& Key, const Int32 Value );

    float ReadFloat( const String& Section, const String& Key, const float DefaultValue ) const;
    void WriteFloat( const String& Section, const String& Key, const float Value );

    double ReadDouble( const String& Section, const String& Key, const double DefaultValue ) const;
    void WriteDouble( const String& Section, const String& Key, const double Value );

    const String ReadString( const String& Section, const String& Key, const String& DefaultValue ) const;
    void WriteString( const String& Section, const String& Key, const String& Value );

private:
    // Do not allow copying on config files... at least until there is an actual need for it. -- NCY 5.5.2018
    ConfigFile( const ConfigFile& Copy ) = delete;
    ConfigFile& operator=( const ConfigFile& Rhs ) = delete;

    // Will rewrite the current config file with the mutated data.  Used for write operations.
    // Quick implementation -- rewrite the of the entire file is bad.  Caching should be implemented
    // so that a write does not rewrite the entire file, but only the entry. NCY, 5.14.2018.
    //
    // @TODO: Implement caching.
    //
    // @NOTE: Just realized that this may leave config files in an unsafe state in the event
    //        of a crash during a write operation.  This seems few and far between, but this
    //        will need to be patched!
    void RewriteFile();

private:
    Array< ConfigFileBlock* > Data;

    bool IsLoaded;

    String Name; // Name of the config file.
    String Directory; // Containing directory of the config file.
    String Path; // Absolute path of the config file.

    Platform::FileStream* Handle;
};

DJINN_END_NS2( Djinn, Configuration )