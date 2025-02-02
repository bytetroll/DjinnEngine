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
 on 10/19/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Reflection/ID.h>
#include <Core/Visitor.h>

namespace Djinn::Reflection {
    class IClassInfo;
    class IEnumInfo;
    class IAttribute;

    // Common results
    DJINN_COM_RESULT(UnknownClass, true, "Reflection");
    DJINN_COM_RESULT(UnknownInterface, true, "Reflection");

    class IReflectionRegistry : public COM::TUnknown<IReflectionRegistry> {
    public:
        DJINN_COM_INTERFACE();

        IReflectionRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add a class to the reflection registry
        /// \param _class
        /// \return
        virtual COM::Result AddClass(IClassInfo *_class) = 0;

        /// Add an enum to the reflection registry
        /// \param _enum
        /// \return
        virtual COM::Result AddEnum(IEnumInfo *_enum) = 0;

        /// Get a class from the reflection registry into out
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result GetClass(const ClassID &id, IClassInfo **out) = 0;

        /// Get an enum from the reflection registry into out
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result GetEnum(const ClassID &id, IEnumInfo **out) = 0;

        /// Visit all derived classes of a class id
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result VisitDerivedClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *)> &visitor) = 0;

        /// Visit all classes with a given attribute
        /// \param id
        /// \param visitor
        /// \return
        virtual COM::Result VisitAttributeClasses(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute *)> &visitor) = 0;

        /// Visit all enums with a given attribute
        /// \param id
        /// \param visitor
        /// \return
        virtual COM::Result VisitAttributeEnums(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor) = 0;

        /// Remove a class from the reflection registry
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result RemoveClass(const ClassID &id) = 0;

        /// Remove an enum from the reflection registry
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result RemoveEnum(const ClassID &id) = 0;

        /// Bind a visitor when a class attribute is installed
        /// \param id
        /// \param visitor
        /// \return
        virtual COM::Result BindClassAttributeInstalled(const ClassID &id, const Core::Visitor<void(IClassInfo *, IAttribute *)> &visitor, bool invoke = true) = 0;

        /// Bind a visitor when an enum attribute is installed
        /// \param id
        /// \param visitor
        /// \return
        virtual COM::Result BindEnumAttributeInstalled(const ClassID &id, const Core::Visitor<void(IEnumInfo *, IAttribute *)> &visitor, bool invoke = true) = 0;

        /// Helper
        COM::Result GetClass(const char *name, IClassInfo **out) {
            return GetClass(ToID(name), out);
        }

        /// Helper
        COM::Result GetEnum(const char *name, IEnumInfo **out) {
            return GetEnum(ToID(name), out);
        }

        /// Helper
        template<typename T>
        COM::Result GetClass(IClassInfo **out) {
            return GetClass(ToID<T>(), out);
        }

        /// Helper
        template<typename T>
        COM::Result GetEnum(IEnumInfo **out) {
            return GetEnum(ToID<T>(), out);
        }
    };
}