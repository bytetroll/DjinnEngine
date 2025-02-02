#pragma once

#include <COM/IUnknown.h>
#include <Core/IStream.h>
#include <Core/String.h>

namespace Djinn {
    namespace Base {
        class ILogStream : public Core::IStream, public COM::TUnknown<ILogStream> {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            ILogStream(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Write message
            /// \param message
            virtual void WriteMessage(const Core::String& header, const Core::String& message) = 0;

            /// Override
            COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
                switch (iid) {
                    case kIID:
                        *out = static_cast<ILogStream*>(this);
                        return COM::kOK;
                    case COM::IUnknown::kIID:
                        *out = static_cast<COM::IUnknown*>(this);
                        return COM::kOK;
                }
                return COM::kUnknownInterface;
            }
        };
    }
}