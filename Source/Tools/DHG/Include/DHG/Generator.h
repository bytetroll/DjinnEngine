//
// Created by Dev on 5/28/2018.
//
#pragma once

#include <Core/StringStream.h>
#include "MetaData.h"

namespace Djinn::DHG {
    struct GeneratorJob {
        MetaData* md = nullptr;
        Core::String output = nullptr;
        Core::String graphOutput = nullptr;
        Core::String graphShortOutput = nullptr;
        Core::Array<Core::String> headers;
    };
    
    class Generator {
    public:
        bool Generate(const GeneratorJob& job);

    private:
        bool GenerateGraph(const GeneratorJob& job);
        bool GenerateDHG(const GeneratorJob& job);
        Core::String GenerateType(const MetaType& type);
        void GenerateTypeInfo(const MetaType& type, Core::StringStream& out, bool ptr = false);
    };
}