//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include "IDiagnostic.h"
#include <list>
#include <vector>

namespace NyLang {
    class Diagnostic : public IDiagnostic {
    public:
        LANG_CLASSID("Lang.Diagnostic");

        /// Construct and install instance into registry
        /// \return
        static Diagnostic* Install(Registry*registry);

        /// Construct
        /// \param registry
        Diagnostic(Registry*registry);

        /// Overrides
        virtual void Push(IMessage*scope) override;
        virtual void Pop() override;
        virtual void Queue(IMessage*message) override;
        virtual void PrettyPrint(PrettyStream &stream) override;
        virtual ClassID GetClassID() override;
        virtual void PushBucket(DiagnosticBucket*scope) override;
        virtual void PopBucket() override;
        virtual void QueueBucket(DiagnosticBucket*bucket) override;
        virtual void Clear() override;

    private:
        Registry* registry = nullptr;

        // Current message scope
        std::list<IMessage*> scopeStack;

        // Buckets
        Vector<DiagnosticBucket*> buckets;
    };
}
