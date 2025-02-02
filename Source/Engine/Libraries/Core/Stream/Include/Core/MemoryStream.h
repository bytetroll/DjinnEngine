//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 4/7/2018.
//
#pragma once

#include "IStream.h"
#include <Core/Memory.h>

namespace Djinn::Core {
    class MemoryStream : public IStream {
    public:
        MemoryStream(char *buffer) : buffer(buffer) {
            
        }

        /// Override
        bool IsEOS() const override {
            return false;
        }

        /// Override
        bool IsGood() override {
            return buffer != nullptr;
        }

        /// Override
        void Close() override {
            buffer = nullptr;
        }

        /// Override
        void Flush() override {

        }

        /// Override
        Int32 Peek() override {
            return buffer[position + 1];
        }

        /// Override
        Int64 Read(void *buffer, USize size) override {
            Core::Memory::Copy(buffer, &this->buffer[position], size);
            position += size;
            return size;
        }

        /// Override
        Int64 Write(const void *buffer, USize size) override {
            Core::Memory::Copy(&this->buffer[position], buffer, size);
            position += size;
            return size;
        }

        /// Override
        USize GetPosition() const override {
            return position;
        }

        /// Override
        USize GetSize() const override {
            return 0;
        }

        /// Override
        void SetPosition(USize n) override {
            position = n;
        }

        /// Override
        Int32 Read() override {
            Int32 v;
            Read(&v, sizeof(Int32));
            return v;
        }

    private:
        USize position = 0;
        char *buffer = nullptr;
    };
}