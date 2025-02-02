#pragma once

#include <Async/Detail/fcontext.h>

namespace Djinn {
    namespace Async {
        namespace API {
            using Context = void*;

            // ...
            struct ThreadContextData {
                void* user;
                Context context;
            };

            // Start thread context
            DJINN_EXPORT_PLUGINS_ASYNC extern void StartThreadContext(void(*handle)(ThreadContextData*), void* data);

            // Create a context
            DJINN_EXPORT_PLUGINS_ASYNC extern Context CreateContext(void(*handle)(void*), void* data);

            // Get current context
            DJINN_EXPORT_PLUGINS_ASYNC extern Context GetCurrentContext();

            // Get context data
            DJINN_EXPORT_PLUGINS_ASYNC extern void* GetContextData();

            // Switch to context
            DJINN_EXPORT_PLUGINS_ASYNC extern void SwitchToContext(Context to);

            // Is within a context?
            DJINN_EXPORT_PLUGINS_ASYNC extern bool IsWithinContext();

            /// Set the default context data
            /// \param data
            DJINN_EXPORT_PLUGINS_ASYNC extern void SetDefaultContextData(void* data);
        }
    }
}