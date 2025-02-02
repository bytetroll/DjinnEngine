#pragma once

#include "ILogStream.h"

namespace Djinn::Base {
    class ConsoleLogStream : public ILogStream {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        ConsoleLogStream(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual void WriteMessage(const Core::String& header, const Core::String &message) override;
                bool IsEOS() const override;
        bool IsGood() override;
        void Close() override;
        void Flush() override;
        Int32 Peek() override;
        Int32 Read() override;
        Int64 Read(void *buffer, USize size) override;
        Int64 Write(const void *buffer, USize size) override;
        USize GetPosition() const override;
        USize GetSize() const override;
        void SetPosition(USize n) override;

        Core::String test = "abcdefgASS";
    };
}
