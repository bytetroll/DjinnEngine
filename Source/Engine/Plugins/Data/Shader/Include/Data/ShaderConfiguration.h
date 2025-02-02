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
 on 11/28/2017.
//
#pragma once

#include <Core/Unique.h>
#include <Core/String.h>
#include <Core/Array.h>
#include <Data/IAssetConfiguration.h>

namespace Djinn::Data {
    class ShaderConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Add an entry point to this shader
        /// \param value
        /// \return
        ShaderConfiguration &AddEntryPoint(const Core::String &value) {
            entryPoints.Add(value);
            return *this;
        }

        /// Set all entry points
        /// \param value
        /// \return
        ShaderConfiguration &EntryPoints(const Core::Array<Core::String> &value) {
            entryPoints.Add(value);
            return *this;
        }

        /// Override
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            USize hash = 0;
            for (auto &str : entryPoints) {
                hash = CombineHash(hash, str);
            }
            return hash;
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new ShaderConfiguration(*this);
        }

        // Entry points to compile
        Core::Array<Core::String> entryPoints;
    };
}