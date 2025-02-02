//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <set>
#include <vector>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/Context.h>

namespace NyLang {
    struct SymbolVariable;

    class InitializationFrame : public IUnknown {
    public:
        LANG_CLASSID("InitializationFrame");

        /// Construct
        /// \param registry
        InitializationFrame();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Is a variable initialized
        /// \param value
        /// \return
        bool IsInitialized(SymbolVariable* value);

        /// Mark a variable
        /// \param value
        /// \param isInitialized
        void Mark(SymbolVariable *value, bool isInitialized);

        /// Push this frame
        /// \param alwaysInitialized 
        /// \return 
        InitializationFrame* Push(Context& context, bool alwaysInitialized = false);

        /// Set insertion frame
        /// \param frame
        void SetInsertionFrame(InitializationFrame* frame);

    private:
        InitializationFrame* parent = nullptr;
        Vector<InitializationFrame*> children;
        std::set<SymbolVariable*> flags;
        bool alwaysInitialized = false;
        InitializationFrame* insertionFrame = nullptr;
    };
}
