//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/PrettyStream.h>
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class IType;

    class IValue : public IUnknown {
    public:
        IValue(IType* type);

        /// Print this value
        /// \param stream
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) = 0;

        /// Pretty print this as a reference
        /// \param stream
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context);

        /// Pretty print all annotations
        /// \param stream
        /// \param context
        void PrettyPrintAnnotations(PrettyStream &stream, PrettyPrintContext &context, bool endingSpace = true);

        /// Add use case
        void AddUse() {
            usages++;
        }

        /// Remove use case
        void RemoveUse() {
            if (--usages < 0) {
                usages = 0;
            }
        }

        /// Get usage count
        /// \return
        int GetUses() const {
            return usages;
        }

        /// Get type of this value
        /// \return
        IType* GetType() const {
            return type;
        }

        /// Set type
        /// \param Ptr
        /// \param type
        void SetType(IType* type) {
            this->type = type;
        }

        /// Get all annotations
        /// \return
        PtrList<IValue>& GetAnnotations() {
            return annotations;
        }

        /// Add an annotation
        /// \param annot
        void AddAnnotation(IValue* annot) {
            annotations.push_back(annot);
        }

    private:
        // Usage count
        int usages = 0;

        // Type of this value
        IType* type = nullptr;

        // Constant annotations
        PtrList<IValue> annotations;
    };
}
