#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Frontend/Token/Token.h>
#include <NyLang/Core/COM/ID.h>
#include <NyLang/Core/Shared.h>
#include <NyLang/Core/PrettyStream.h>
#include <vector>
#include <map>
#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/Context.h>
#include "SymbolAnchor.h"

namespace NyLang {
    /// Symbol component
    struct SymbolComponent : public IUnknown {
    public:
        LANG_CLASSID("SymbolComponent");

        /// Initialize from class id
        /// \param cid
        explicit SymbolComponent() : IUnknown() {

        }

        /// Create a copy of this symbol component
        /// \return
        virtual SymbolComponent *Copy(Context &context) = 0;

        /// Compare to other component
        /// \return
        virtual bool Compare(SymbolComponent* other) = 0;

        /// Pretty print
        /// \param stream
        virtual void PrettyPrint(PrettyStream &stream) = 0;

        /// Set anchor
        /// \param anchor
        inline void SetAnchor(const SymbolAnchor& anchor) {
            this->anchor = anchor;
        }

        /// Get anchor
        /// \return
        inline const SymbolAnchor& GetAnchor() {
            return anchor;
        }

    private:
        // Owner anchor
        SymbolAnchor anchor;
    };

    /// Base symbol
    struct Symbol {
    public:
        LANG_CLASSID("Symbol");

        /// Initialize from anchor
        /// \param anchor
        explicit Symbol(const SymbolAnchor &anchor);

        /// Initialize from anchor
        Symbol(const SymbolAnchor& anchor, const Vector<SymbolComponent*>& components);

        /// Create a copy of this symbol
        /// \return
        Symbol* Copy(Context& context);

        /// Pretty print
        /// \param stream
        inline void PrettyPrint(PrettyStream &stream) {
            if (owningComponent) {
                owningComponent->PrettyPrint(stream);
            } else {
                stream << "<No owning component>";
            }
        }

        /// Get component
        /// \return
        template<typename T>
        inline T* GetComponent() {
            auto &&com = components.find(T::kCID.Get());
            if (com == components.end()) {
                return nullptr;
            }
            return static_cast<T*>(com->second);
        }

        /// Add component
        /// \param shared
        inline void AddComponent(SymbolComponent*shared, bool forceOwner = false) {
            if (shared) {
                components[shared->GetClassID()] = shared;
                shared->SetAnchor(anchor);
                if (!owningComponent || forceOwner) {
                    owningComponent = shared;
                }
            }
        }

        /// Get anchor
        /// \return
        inline const SymbolAnchor& GetAnchor() const {
            return anchor;
        }

        /// Is this symbol empty?
        /// \return
        inline bool IsEmpty() const {
            return components.size() == 0;
        }

    private:
        // Components
        Map<ClassID::Type, SymbolComponent*> components;

        // Owning component
        SymbolComponent* owningComponent = nullptr;

        // Anchor
        SymbolAnchor anchor;
    };
}