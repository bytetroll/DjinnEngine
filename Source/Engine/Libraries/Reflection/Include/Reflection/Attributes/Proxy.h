//
// Created by Dev on 9/12/2018.
//

#pragma once

#include <Reflection/IAttribute.h>

namespace Djinn::Attributes {
    struct Proxy : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Proxy(const char *name);

        /// Overrides
        void Attached(COM::IQueryable *container) override;
        COM::ClassID GetClassID() override;

        const char *name;
    };

    struct EnumProxy : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        EnumProxy(const char *from, const char* to);

        /// Overrides
        void Attached(COM::IQueryable *container) override;
        COM::ClassID GetClassID() override;

        const char *from;
        const char *to;
    };
}