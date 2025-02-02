#pragma once

#include <Host/IRegistry.h>
#include "ILogStream.h"
#include "ConsoleLogStream.h"
#include <Core/Assert.h>

namespace Djinn {
    namespace Base {
        class LogBuilder {
        public:
            LogBuilder() = default;
            LogBuilder(ILogStream *logStream, const Core::String &prefix, const Core::String& postfix) : logStream(logStream), prefix(prefix), postfix(postfix) {

            }

            template<typename... T>
            void Write(T &&... args) {
                if (logStream) {
                    Core::String msg = _Write(args...);
                    logStream->WriteMessage(prefix, postfix + msg);
                }
            }

        private:
            template<typename T0, typename T1, typename... TX>
            Core::String _Write(T0 &&t0, T1 &&t1, TX &&... tx) {
                return _Write(t0) + _Write(t1, tx...);
            }

            template<typename T0>
            Core::String _Write(T0 &&t0) {
                return Core::String(t0);
            }

            Core::String _Write(const char* t0) {
                return Core::String(t0 ?: "null");
            }

            Core::String _Write(char* t0) {
                return Core::String(t0 ?: "null");
            }

            ILogStream *logStream = nullptr;
            Core::String prefix;
            Core::String postfix;
        };
    }

    // Info log
    static Base::LogBuilder InfoLog(Host::IRegistry* registry) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!registry->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "   Info", "");
    }

    // Warning log
    static Base::LogBuilder WarningLog(Host::IRegistry* registry) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!registry->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "Warning", "");
    }

    // Error log
    static Base::LogBuilder ErrorLog(Host::IRegistry* registry) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!registry->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "  Error", "");
    }

    static Core::String GetLoggingTypename(COM::IUnknown* com) {
        Core::String name = com->GetClassName();
        if (name.StartsWith("Djinn::")) {
            name = Core::String(name, 7, name.Length());
        }
        name += " -> ";
        return name;
    }

    // Info log
    static Base::LogBuilder InfoLog(COM::IUnknown* com) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!com->GetRegistry()->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "   Info", GetLoggingTypename(com));
    }

    // Warning log
    static Base::LogBuilder WarningLog(COM::IUnknown* com) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!com->GetRegistry()->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "Warning", GetLoggingTypename(com));
    }

    // Error log
    static Base::LogBuilder ErrorLog(COM::IUnknown* com) {
        Base::ConsoleLogStream* logStream = nullptr;
        if (!com->GetRegistry()->GetPipeline()->GetClass(&logStream)) {
            CErr("Log stream not attached to registry\n");
            return {};
        }
        return Base::LogBuilder(logStream, "  Error", GetLoggingTypename(com));
    }

    /// Error message check helper
#define CheckMsg(CODE, TYPE, ...) {if(Djinn::COM::CheckedResult result = CODE) { Djinn::TYPE##Log(this).Write(__VA_ARGS__); return static_cast<COM::Result>(result); }}

    /// Error message check helper
#define CheckMsgVoid(CODE, TYPE, ...) {if(Djinn::COM::CheckedResult result = CODE) { Djinn::TYPE##Log(this).Write(__VA_ARGS__); return; }}
}