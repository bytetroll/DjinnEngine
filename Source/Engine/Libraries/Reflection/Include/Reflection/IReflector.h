//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Size.h>

namespace Djinn::Reflection {
    class IClassInfo;
    class IEnumInfo;

    class IReflector {
    public:
        /// Add a reflected class
        /// \param info
        virtual void AddClass(IClassInfo* info) = 0;

        /// Add a reflected enum
        /// \param info
        virtual void AddEnum(IEnumInfo* info) = 0;
    };
}