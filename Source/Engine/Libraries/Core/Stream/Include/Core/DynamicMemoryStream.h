//
// Created by Dev on 4/7/2018.
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