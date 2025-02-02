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
 on 5/28/2018.
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