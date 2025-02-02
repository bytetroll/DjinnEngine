// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <cfloat>

#include <exception>

#include <Configuration/ConfigFile.h>

#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/String.h>
#include <Core/StringStream.h>

#include <Core/Assert.h>

#include <Core/ExceptionFormatter.h>
#include <Core/RuntimeException.h>
#include <Core/Console.h>

#include <Core/Algorithim.h>


DJINN_BEGIN_NS2( Djinn, Configuration )

using namespace Djinn::Core;
using namespace Djinn::Platform;
using namespace Djinn::Debug;

ConfigFile::ConfigFile() : IsLoaded( false ), Name( "" ), Directory( "" ), Path( "" ) {
}

ConfigFile::ConfigFile( const String& Path ) : IsLoaded( false ), Name( "" ), Directory( "" ), Path( Path ) {
    Load( Path );
}

ConfigFile::~ConfigFile() {
    Unload();
}

void ConfigFile::Load( const String& Path ) {
    Handle = new Platform::FileStream( FileMode::eRead, Path );
    DJINN_ASSERT( Handle->IsGood(), "Unable to open config file.  Either the file is corrupt or does not exist on disk." );

    StringStream* const Stream = new StringStream( Handle );
    UInt32 Timeout = 0;
    bool PendingSection = false;
    String Line;
    while( true ) {
        if( Stream->GetStream()->IsEOS()) {
            break;
        }

        if( !PendingSection ) {
            Line = Stream->ReadLine();
        }

        PendingSection = false;

        // Timeout is needed as a fail-safe for eReadWrite on Windows.  Windows 10 has a weird issue where a file
        // declared as eReadWrite will cause an infinite loop, writing 0 to the file.
        if( Timeout == 64 ) {
            break;
        }

        // Section.
        if( Line.Contains( "[" )) {
            if( Line.Contains( "]" )) {
                ConfigFileBlock* const Block = new ConfigFileBlock;

                // We have a section.
                const String SectionName = Line.Substring( '[', ']' );
                Block->Section = SectionName;

                // Read the entries under our section until we encounter another section.
                while( true ) {
                    if( Stream->GetStream()->IsEOS()) {
                        break;
                    }

                    Line = Stream->ReadLine();

                    if( Line.Contains( "[" ) && Line.Contains( "]" )) {
                        // Multiple section configs.
                        Data.Add( Block );
                        PendingSection = true;
                        break;
                    }

                    // We have a key-value pair.
                    if( Line.Contains( "=" )) {
                        String* const Entry = Line.Splice( '=' );
                        Block->AddPair( Entry[ 0 ], Entry[ 1 ] );
                        delete[] Entry;
                    }
                }

                // Single section configs.
                if( !PendingSection ) {
                    Data.Add( Block );
                }
            } else {
                DJINN_ASSERT( true, "Config file contains malformed section token." );
            }
        } else {
            ++Timeout;
        }
    }

    delete Stream;

    this->Path = Path;
    // @TODO: Fill the rest of them here.

    IsLoaded = true;
}

void ConfigFile::Unload() {
    if( Handle != nullptr ) {
        Handle->Close();
        delete Handle;
        Handle = nullptr;
    }

    for( USize Index = 0; Index != Data.Size(); ++Index ) {
        delete Data[ Index ];
        Data[ Index ] = nullptr;
    }
}

bool ConfigFile::ReadBool( const String& Section, const String& Key, const bool DefaultValue ) const {
    for( auto SectionIter = Data.begin(); SectionIter != Data.end(); ++SectionIter ) {
        ConfigFileBlock* const Block = (*SectionIter);

        if( Section == Block->Section ) {
            for( auto PairIter = Block->Pairs.begin(); PairIter != Block->Pairs.end(); ++PairIter ) {
                const Core::Pair Pair = ( *PairIter );

                // @TODO: Regex match these.
                if( Key == Pair.first ) {
                    if( ( Pair.second == "True" ) || ( Pair.second == "true" ) || ( Pair.second == "1" ) ||
                        ( Pair.second == "Yes" ) || ( Pair.second == "yes" ) ) {
                        return true;
                    } else if( ( Pair.second == "False" ) || ( Pair.second == "false" ) || ( Pair.second == "0" ) ||
                            ( Pair.second == "No" ) || ( Pair.second == "no" ) ) {
                        return false;
                    }
                }
            }
        }  
    }

    return DefaultValue;
}

void ConfigFile::WriteBool( const String& Section, const String& Key, const bool Value ) {
}

Int32 ConfigFile::ReadInt( const String& Section, const String& Key, const Djinn::Int32 DefaultValue ) const {
    for( auto SectionIter = Data.begin(); SectionIter != Data.end(); ++SectionIter ) {
        // This cannot be const.  On GCC the pair class does behind the scenes manipulation of both pointer and object.
        // On GCC, const decl will cause objects to mutate and the main thread to crash. NCY, 5.8.2018
        ConfigFileBlock* const Block = ( *SectionIter );

        if( Section == Block->Section ) {
            for( auto PairIter = Block->Pairs.begin(); PairIter != Block->Pairs.end(); ++PairIter ) {
                const Core::Pair Pair = ( *PairIter );

                if( Key == Pair.first ) {
                    try {
                        const Int32 CeilThreshold = std::numeric_limits< Int32 >::max();
                        const Int32 FloorThreshold = std::numeric_limits< Int32 >::min();

                        const Int32 IntRepresentation = std::stoi( Pair.second.AsCharArray() );

                        if( ( IntRepresentation > CeilThreshold ) || ( IntRepresentation < FloorThreshold ) ) {
                            DJINN_THROW new RuntimeException(
                                    ExceptionFormatter() << "Value of key " << Pair.second << "exceeds numeric bounds for type Int32."
                            );
                        }

                        return IntRepresentation;
                    } catch( const std::exception& Except ) {
                        std::cout << Except.what() << std::endl;
                    }
                }
            }
        }
    }

    return DefaultValue;
}

void ConfigFile::WriteInt( const String& Section, const String& Key, const Int32 Value ) {
}

float ConfigFile::ReadFloat( const String& Section, const String& Key, const float DefaultValue ) const {
    for( auto SectionIter = Data.begin(); SectionIter != Data.end(); ++SectionIter ) {
        ConfigFileBlock* const Block = ( *SectionIter );

        if( Section == Block->Section ) {
            for( auto PairIter = Block->Pairs.begin(); PairIter != Block->Pairs.end(); ++PairIter ) {
                const Core::Pair Pair = ( *PairIter );

                if( Key == Pair.first ) {
                    try {
                        const float FloatRepresentation = std::stof( Pair.second.AsCharArray() );

//                        if( ( FloatRepresentation > FLT_MAX ) || ( FloatRepresentation < FLT_MIN ) ) {
//                            throw new RuntimeException(
//                                    ExceptionFormatter() << "Value of key " << Pair.second << "exceeds numeric bounds for type float."
//                            );
//                        }

                        return FloatRepresentation;
                    } catch( const std::exception& Except ) {
                        std::cout << Except.what() << std::endl;
                    }
                }
            }
        }
    }

    return DefaultValue;
}

void ConfigFile::WriteFloat( const String& Section, const String& Key, const float Value ) {
}

double ConfigFile::ReadDouble( const String& Section, const String& Key, const double DefaultValue ) const {
    for( auto SectionIter = Data.begin(); SectionIter != Data.end(); ++SectionIter ) {
        // This cannot be const.  On GCC the pair class does behind the scenes manipulation of both pointer and object.
        // On GCC, const decl will cause objects to mutate and the main thread to crash. NCY, 5.8.2018
        ConfigFileBlock* const Block = ( *SectionIter );

        if( Section == Block->Section ) {
            for( auto PairIter = Block->Pairs.begin(); PairIter != Block->Pairs.end(); ++PairIter ) {
                const Core::Pair Pair = ( *PairIter );

                if( Key == Pair.first ) {
                    try {
                        const double DoubleRepresentation = std::stod( Pair.second.AsCharArray() );

//                        if( ( FloatRepresentation > FLT_MAX ) || ( FloatRepresentation < FLT_MIN ) ) {
//                            throw new RuntimeException(
//                                    ExceptionFormatter() << "Value of key " << Pair.second << "exceeds numeric bounds for type float."
//                            );
//                        }

                        return DoubleRepresentation;
                    } catch( const std::exception& Except ) {
                        std::cout << Except.what() << std::endl;
                    }
                }
            }
        }
    }

    return DefaultValue;
}

void ConfigFile::WriteDouble( const String& Section, const String& Key, const double Value ) {
}

const String ConfigFile::ReadString( const String& Section, const String& Key, const String& DefaultValue ) const {
    for( auto SectionIter = Data.begin(); SectionIter != Data.end(); ++SectionIter ) {
        ConfigFileBlock* const Block = ( *SectionIter );

        if( Section == Block->Section ) {
            for( auto PairIter = Block->Pairs.begin(); PairIter != Block->Pairs.end(); ++PairIter ) {
                const Core::Pair Pair = ( *PairIter );

                if( Key == Pair.first ) {
                    return Pair.second;
                }
            }
        }
    }

    return DefaultValue;
}

void ConfigFile::WriteString( const String& Section, const String& Key, const String& Value ) {
}

void ConfigFile::RewriteFile() {
    // Reorder our cached data so that we write in the correct order.
    Reverse< ConfigFileBlock* >( Data.begin(), Data.End() );

    for( auto DataIter = Data.begin(); DataIter != Data.end(); ++DataIter ) {
        const String Section = (*DataIter)->Section;

        for( auto BlockIter = (*DataIter)->Pairs.begin(); BlockIter != (*DataIter)->Pairs.end(); ++BlockIter ) {
            const String Key = (*BlockIter).first;
            const String Value = (*BlockIter).second;

            std::cout << Section.AsCharArray() << " " << Key.AsCharArray() << " " << Value.AsCharArray() << std::endl;
            //FileAPI::Write( Handle, )
        }
    }
}


DJINN_END_NS2( Djinn, Configuration )