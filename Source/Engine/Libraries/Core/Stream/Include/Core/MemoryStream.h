//
// Created by Dev on 4/7/2018.
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