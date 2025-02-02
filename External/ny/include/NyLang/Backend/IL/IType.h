//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/PrettyStream.h>
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class IValue;

    class IType : public IUnknown {
    public:
        IType(int bitWidth);

        /// Print this value
        /// \param stream
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) = 0;

        /// Pretty print all annotations
        /// \param stream
        /// \param context
        void PrettyPrintAnnotations(PrettyStream &stream, PrettyPrintContext &context, bool endingSpace = true);

        /// Get bit width
        /// \return
        int GetBitWidth() const {
            return bitWidth;
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
        int bitWidth;

        // Constant annotations
        PtrList<IValue> annotations;
    };
}
