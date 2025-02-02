//
// Created by Dev on 5/28/2018.
//
#pragma once

#include <Core/Set.h>
#include "Parser.h"
#include "MetaData.h"

namespace Djinn::DHG {
    class Reflector {
    public:
        Reflector(MetaData* md);

        void Reflect(ParserBlob* blob, const Core::String& name);

    private:
        void Reflect(ParserBlob* blob, CXCursor root);
        void ReflectIncludeEdges(ParserBlob* blob, CXFile file, const char* from);
        void ReflectClass(ParserBlob* blob, CXCursor classCursor, CX_CXXAccessSpecifier access);
        void ReflectEnum(ParserBlob* blob, CXCursor enumCursor, CX_CXXAccessSpecifier access);
        bool ReflectType(CXType type, MetaType& out);
        const char* GetTypeName(CXType type);
        MetaAnnotation ReflectAnnotation(CXCursor cursor);

        template<typename F>
        void Visit(CXCursor cursor, F&& action);

        MetaData* md;
        MetaAnnotation classAnnotations;
        Core::Set<const char*> visitedIncludes;
    };
}