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

#include <Core/Array.h>
#include <Core/Memory.h>
#include "IStream.h"

namespace Djinn::Core {
    class DynamicMemoryStream : public IStream {
    public:
        virtual bool IsEOS() const override {
            return false;
        }

        virtual bool IsGood() override {
            return true;
        }

        virtual void Close() override {
        }

        virtual void Flush() override {
        }

        virtual Int32 Peek() override {
            return buffer[position + 1];
        }

        virtual Int64 Read(void *buffer, USize size) override {
            Core::Memory::Copy(buffer, &this->buffer[position], size);
            position += size;
            return size;
        }

        virtual Int64 Write(const void *buffer, USize size) override {
            this->buffer.ResizePassive(position + size);
            Core::Memory::Copy(&this->buffer[position], buffer, size);
            position += size;
            return size;
        }

        virtual USize GetPosition() const override {
            return position;
        }

        virtual USize GetSize() const override {
            return buffer.Size();
        }

        virtual void SetPosition(USize n) override {
            this->buffer.ResizePassive(n);
            position = n;
        }

        virtual Int32 Read() override {
            Int32 v;
            Read(&v, sizeof(Int32));
            return v;
        }

        char* GetBuffer() {
            return buffer.Ptr();
        }

    private:
        USize position = 0;
        Core::Array<char> buffer;
    };
}